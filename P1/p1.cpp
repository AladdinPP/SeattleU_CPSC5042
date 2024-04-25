/**
 * @file ice2_wiggler.cpp - in-class exercise to have two worms each in their own thread
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 5042, Spring 2024"
 * This will continue on from ICE1 where we had one wriggler worm. Here we'll have two
 * worms: The Wriggler and The Wiggler. Each will run in its own thread and we'll use
 * the main thread to paint the terminal window. This adds the use of a mutex to protect
 * the pixel matrix and a barrier to synchronize the generations.
 */

#include <iostream>
#include <future>
#include <barrier>
#include "Terminal.h"
#include "Wriggler.h"
#include "Monster.h"
#include "Cannon.h"
#include "CannonBall.h"

using namespace std;

/**
 * @class ScreenManager handles the game play, synchronizing with the other threads
 */
class ScreenManager {
public:
    ScreenManager(Wriggler *worm, PixelMatrix *pxm, mutex *m, barrier<>
            *b,
                  int t = 0)
            : time(t), worm(worm), pxm(pxm), lock(m), meetup(b) {}

    /**
     * This is the method running in the threads.
     * @return number of generations drawn
     */
    int play() {
        int generations = 0;
        while (!stop.test()) {
            generations++;

            // paint myself on the canvas (but avoid "mixed bodies" with the mutex)
            lock->lock();
            if (generations >= time) {
                worm->render(*pxm);
            }
            lock->unlock();

            // once we've all met up, all the rendering is done; wait for resumption point
            meetup->arrive_and_wait();

            // once the previous generation has been painted, move worm to next state
            meetup->arrive_and_wait();
            if (generations >= time) {
                worm->move();
            }
        }
        meetup->arrive_and_drop();
        return generations;
    }

public:
    atomic_flag stop;

private:
    int time;
    Wriggler *worm;
    PixelMatrix *pxm;
    mutex *lock;
    barrier<> *meetup;
};

class CannonManager {
public:
    CannonManager(Cannon *gun, PixelMatrix *pxm, mutex *m, barrier<> *b)
                            : gun(gun), pxm(pxm), lock(m), meetup(b) {};

    int display() {
        int generations = 0;
        while (!stop.test()) {
            generations++;

            // paint myself on the canvas (but avoid "mixed bodies" with the mutex)
            lock->lock();
            gun->render(*pxm);
            lock->unlock();

            // once we've all met up, all the rendering is done; wait for resumption point
            meetup->arrive_and_wait();

            // once the previous generation has been painted, move worm to next state
            meetup->arrive_and_wait();
        }
        meetup->arrive_and_drop();
        return generations;
    }

public:
    atomic_flag stop;

private:
    Cannon *gun;
    PixelMatrix *pxm;
    mutex *lock;
    barrier<> *meetup;
};

class CannonBallManager {
public:
    CannonBallManager(CannonBall *ball, PixelMatrix *pxm, mutex *m, barrier<> *b)
            : ball(ball), pxm(pxm), lock(m), meetup(b), stop(false) {};

    int shot() {
        try {
            while (!stop) { // Continue until the stop flag is set
                // Lock the mutex before rendering
                lock_guard<mutex> guard(*lock);
                ball->render(*pxm); // Render the cannonball

                // Move the cannonball
                ball->move();

                if (ball->isStoppedStatus()) {
                    stop = true; // Stop the ScreenManager if the cannonball is stopped
                }

                // Synchronize with the barrier
                meetup->arrive_and_wait();
            }

        } catch (const std::exception &e) {
            cerr << "Error: " << e.what() << endl; // Log errors
        }

        return 0;
    }

    void terminate() {
        stop = true; // Signal to stop
    }

private:
    CannonBall *ball; // Reference to the cannonball
    PixelMatrix *pxm; // Pixel matrix for rendering
    mutex *lock; // Mutex for thread safety
    barrier<> *meetup; // Barrier for synchronization
    atomic<bool> stop; // Atomic flag to indicate stop
};

class MyWriggler {
public:
    Wriggler wiggler;
    ScreenManager manager;
    future<int> handle;

    MyWriggler(
            int startRow, int startCol, int limitR, int limitC,
            PixelMatrix *pxm, mutex *m, barrier<> *meetup, int time = 0
    ) : wiggler(startRow, startCol, limitR, limitC),
        manager(&wiggler, pxm, m, meetup, time),
        handle(async(&ScreenManager::play,
                     &manager)) {}
};

/**
 * main thread
 */
int main() {
    const RGB BG_COLOR = RGB::BLACK;
    const int WAIT_TIME = 1;  // seconds to wait before starting
    const int THROTTLE = 50;  // milliseconds of pause between generations

    cout << "Stop the wrigglers by typing Q key." << endl;
    cout << "Try making them wiggle exactly 100 times!" << endl;
    this_thread::sleep_for(chrono::seconds(WAIT_TIME));

    // set up the terminal and the pixel matrix feeding it
    auto *t = new Terminal(false);  // false: makes hasKey work as expected
    int rows, cols;
    t->getSize(rows, cols);
    PixelMatrix pxm(rows, cols, BG_COLOR);

    // set up the synchronization
    barrier meetup(7);
    barrier cannonSystem(2);
    mutex lock;
    mutex cannonLock;

    // set up the cannon
    Cannon cannon(cols / 2, rows - 1);
    CannonManager gunManager(&cannon, &pxm, &cannonLock, &cannonSystem);
    future<int> gunHandle = async(&CannonManager::display, &gunManager);

    // set up the limit of the cannonball
    int cannonBallLimit = 0;
    bool isCannonBallActive = false;
    CannonBall *cannonball = nullptr;
    CannonBallManager *ballManager = nullptr;
    future<int> cannonBallHandle;

    // set up wriggler instances
    vector<MyWriggler *> myWrigglers;
    for (int i = 0; i < 5; i++) {
        myWrigglers.push_back(
                new MyWriggler(rows / 7 * (i + 1), 5, rows, cols, &pxm, &lock,
                               &meetup, 10 * i));
    }
    Monster jill(rows / 5 * 2, cols / 5 * 2, rows, cols);
    ScreenManager jillManager(&jill, &pxm, &lock, &meetup);
    future<int> jillHandle = async(&ScreenManager::play, &jillManager);

    bool continueGame = true;
    while (continueGame) {
        if (t->hasKey()) {
            char ch = t->getKey();
            // only quit if there is no active cannonball
            if (ch == 'q' && !isCannonBallActive) {
                continueGame = false;
            }

            if (ch == ' ' && !isCannonBallActive) {
                cannonball = new CannonBall(cols / 2, rows - 1,
                                            cannonBallLimit);
                ballManager = new CannonBallManager(cannonball, &pxm,
                                                    &cannonLock, &cannonSystem);
                cannonBallHandle = async(&CannonBallManager::shot, ballManager);
                isCannonBallActive = true;
            }
        } else {
            for (int i = 0; i < 5; i++) {
                // Make sure these worms cross each other
                if (myWrigglers[i]->wiggler.getCol() + 9 > jill.getCol() &&
                        myWrigglers[i]->wiggler.getCol() <= jill.getCol() + 8 &&
                        jill.getRow() + 3 == myWrigglers[i]->wiggler.getRow()) {
                    myWrigglers[i]->manager.stop.test_and_set();
                }
            }

            if (cannonball && cannonball->isStoppedStatus()) {
                isCannonBallActive = false;
            }

            this_thread::sleep_for(chrono::milliseconds(
                    THROTTLE)); // Pause for human visibility
            meetup.arrive_and_wait();  // Main thread waits for everyone to render
            cannonSystem.arrive_and_wait();
            t->paint(pxm); // Paint the rendered pixel matrix
            pxm.paint(0, 0, rows, cols,
                      BG_COLOR);  // Clear screen for next render
            cannonSystem.arrive_and_wait();
            meetup.arrive_and_wait();  // Now release everyone from their second wait
        }
    }

    // tell the wrigglers to stop
    for (int i = 0; i < 5; i++) {
        myWrigglers[i]->manager.stop.test_and_set();
    }
    jillManager.stop.test_and_set();
    gunManager.stop.test_and_set();

    // make sure nobody is stuck at the barrier
    cannonSystem.arrive_and_wait();
    cannonSystem.arrive_and_wait();
    meetup.arrive_and_wait(); // first stop
    meetup.arrive_and_wait(); // second stop

    // keep final screen up for a second
    this_thread::sleep_for(chrono::seconds(WAIT_TIME));

    // get the wrigglers' values
    int jillPlayed = jillHandle.get();
    int gunDisplayed = gunHandle.get();

    delete t;  // destruction of the Terminal object returns the screen to normal shell mode

    cout << "Jill wiggled " << jillPlayed << " times!" << endl;
    for (int i = 0; i < 5; i++) {
        int wrigglerPlayed = myWrigglers[i]->handle.get();
        cout << "Wriggler " << i << " wriggled " << wrigglerPlayed <<
        " times!" << endl;
    }
    cout << "Cannon displayed " << gunDisplayed << " times!" << endl;
    return EXIT_SUCCESS;
}
