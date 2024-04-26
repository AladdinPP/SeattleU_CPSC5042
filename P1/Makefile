CPPFLAGS = -std=c++20 -Wall -Werror -pedantic -ggdb -pthread
OBJS = MovingObj.o Monster.o Terminal.o PixelMatrix.o RGB.o Cannon.o CannonBall.o

%.o : %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

p1 : $(OBJS) Monster.o Cannon.o CannonBall.o p1.o
	g++ -lcurses -lpthread $^ -o $@

all : p1

clean :
	rm -f *.o p1