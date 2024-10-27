/**
 * @author  Hongru He
 * @file    coordinator.cpp
 * @version 2.0
 * @date    05/31/2024
 * @func    coordinator for handling the 2PC transaction, for extra credit a)
 *          - dealing with any number of participants
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <arpa/inet.h>
#include "TCPClient.h"

using namespace std;

class coordinator {
public:
    // Constructor to deal with any number of participants
    coordinator(const string& logFile, int argc, char *argv[]){
        log_file = logFile;
        transactions = parse_arguments(argc, argv);
        print_transaction_details();
    }

    // Main function to execute the 2PC transaction among any number of
    // participants
    void run() {
        // Make sure the in-bound amount is equal to the out-bound amount,
        // otherwise, this transaction is invalid
        int sum = 0;
        for (const auto& txn : transactions) {
            sum += txn.amount;
        }

        // If this transaction is invalid, terminate this round of vote request
        if (sum != 0) {
            cerr << "Total amount of those transaction is not correct. Be sure "
                    "the out-bound amount is equal to the in-bound amount.\n";
            return;
        }

        // If this transaction is valid, execute the 2PC transaction
        try {
            unordered_map<string, TCPClient> clients;

            // Initialize every participants
            for (const auto& txn : transactions) {
                string key = txn.host + ":" + to_string(txn.port);
                if (clients.find(key) == clients.end()) {
                    clients.emplace(piecewise_construct, forward_as_tuple(key), forward_as_tuple(txn.host, static_cast<u_short>(txn.port)));
                    cout << "Connected to participant on port " << txn.port << " for account " << txn.account << endl;
                }
            }

            // Sending vote requests to each participant and receiving
            // response, if receiving one abort response, record the boolean
            // as false - should send global abort then
            bool all_commit = true;
            for (const auto& txn : transactions) {
                string vote_request = "VOTE-REQUEST";
                vote_request += " " + to_string(txn.amount) + " " + txn.account;
                string key = txn.host + ":" + to_string(txn.port);
                clients[key].send_request(vote_request);
                cout << "Sending message '" << vote_request << "' to " << key << endl;
                string response = clients[key].get_response();
                log("Response: " + response);
                if (response != "VOTE-COMMIT") {
                    all_commit = false;
                }
            }

            // Sending global message to each participant and receiving ACK back
            for (const auto& txn : transactions) {
                string global_message = all_commit ? "GLOBAL-COMMIT" : "GLOBAL-ABORT";
                global_message += " " + to_string(txn.amount) + " " + txn.account;
                string key = txn.host + ":" + to_string(txn.port);
                clients[key].send_request(global_message);
                cout << "Sending message '" << global_message << "' to " << key << endl;
                string ack = clients[key].get_response();
                log("ACK: " + ack);
            }

            if (all_commit) {
                cout << "Transaction committed" << endl;
            } else {
                cout << "Transaction aborted" << endl;
            }
        } catch (const std::runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
private:
    string log_file;
    // Holds participant's information
    struct Transaction {
        double amount;
        string host;
        int port;
        string account;
    };
    vector<Transaction> transactions;

    // Record to log file
    void log(const string &message) {
        ofstream log(log_file, ios_base::app);
        log << message << endl;
    }

    // Parse the command line and create a vector of participants' information
    void parse_arguments(int argc, char *argv[]) {
        for (int i = 2; i < argc; i += 4) {
            Transaction txn;
            txn.amount = stod(argv[i]);
            txn.host = argv[i + 1];
            txn.port = stoi(argv[i + 2]);
            txn.account = argv[i + 3];
            transactions.push_back(txn);
        }
    }

    // Display the transaction information
    void print_transaction_details() {
        cout << "Transaction Details:" << endl;
        for (const auto &txn: transactions) {
            cout << "  " << (txn.amount >= 0 ? "+" : "") << txn.amount << " "
                 << txn.host << ":" << txn.port << " account #" << txn.account
                 << endl;
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 10 || (argc - 2) % 4 != 0) {
        cerr << "Usage: " << argv[0] << " <log_file> <amount1> <host1> <port1> "
                                        "<account1> [<amount2> <host2> <port2> "
                                        "<account2> ...]" << endl;
        return 1;
    }

    string log_file = argv[1];

    // Initialize the coordinator
    coordinator coBank(log_file, argc, argv);

    // Execute the 2PC transaction
    coBank.run();

    return EXIT_SUCCESS;
}