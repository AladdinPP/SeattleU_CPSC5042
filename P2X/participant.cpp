/**
 * @author  Hongru He
 * @file    participant.cpp
 * @version 1.0
 * @date    05/20/2024
 * @func    participant for handling the 2PC transaction
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "TCPServer.h"

using namespace std;

class Participant : public TCPServer {
public:
    // Constructor
    Participant(u_short port, const string& accountFile, const string&
    logFile) : TCPServer(port) {
        accounts_file = accountFile;
        log_file = logFile;
        state = INIT;

        // Display the information
        cout << "Transaction service on port " << to_string(port)
             << " (Ctrl-C to stop)" << endl;
    }

    // Main function to execute 2PC transaction
    bool process(const string &request) override {
        // Load from the account file
        load_accounts(accounts_file);

        // Receiving the vote requests and sending response
        if (request.find("VOTE-REQUEST") == 0) {
            istringstream iss(request);
            string msg_type, account;
            double amount;
            iss >> msg_type >> amount >> account;

            // Displaying the account information
            cout << "Accepting coordinator connection. State: INIT" << endl;
            cout << "Holding " << amount << " from account " << account << endl;

            // Construct the response and change the state
            string response;
            if (accounts.find(account) != accounts.end() &&
                accounts[account] + amount >= 0) {
                response = "VOTE-COMMIT";
                state = READY;
            } else {
                response = "VOTE-ABORT";
                state = ABORT;
            }

            // Sending response and record to log file
            respond(response);
            log(response);

            cout << "Got VOTE-REQUEST, replying " << response << ". State: " << (state == READY ? "READY" : "ABORT") << endl;

            return true; // Remain connection to wait for global message
        }
            // Receiving the global messages and sending ACK
        else if (request.find("GLOBAL-COMMIT") == 0 || request.find("GLOBAL-ABORT") == 0) {

            // Rewrite the account file and reply ACK
            if (request.find("GLOBAL-COMMIT") == 0) {
                istringstream iss(request);
                string global_response, account;
                double amount;
                iss >> global_response >> amount >> account;

                // Change the account amount and the state
                accounts[account] += amount;
                save_accounts(accounts_file);
                log("Transaction committed");
                state = COMMIT;
                cout << "Got GLOBAL-COMMIT, replying ACK. State: COMMIT" << endl;
                if (amount < 0) {
                    cout << "Committing " << amount << " from account "
                         << account << endl;
                }
            } else if (request == "GLOBAL-ABORT") {
                log("Transaction aborted");
                state = ABORT;
                cout << "Got GLOBAL-ABORT, replying ACK. State: ABORT" << endl;
            }

            // Sending ACK and record to log file
            respond("ACK");
            log("ACK sent");

            return false; // Close connection after global response
        }

        return false;    // Close connection if no case matched
    }

private:
    unordered_map<string, double> accounts;
    string log_file;
    string accounts_file;
    enum State { INIT, READY, COMMIT, ABORT };
    State state;

    // Record to log file
    void log(const string &message) {
        ofstream log(log_file, ios_base::app);
        log << message << endl;
    }

    // Read information from the account file
    void load_accounts(const string &filename) {
        ifstream file(filename);
        string line, account;
        double balance;
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> balance >> account;
            accounts[account] = balance;
        }
    }

    // Rewrite changes to account file
    void save_accounts(const string &filename) {
        ofstream file(filename);
        for (const auto &pair : accounts) {
            file << pair.second << " " << pair.first << endl;
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <port> <accounts_file> <log_file>" << endl;
        return 1;
    }

    // Parse the argv
    u_short port = static_cast<u_short>(stoi(argv[1]));
    string accounts_file = argv[2];
    string log_file = argv[3];

    // Initialize the participant
    Participant participant(port, accounts_file, log_file);

    // Execute the 2PC transaction
    participant.serve();

    return EXIT_SUCCESS;
}