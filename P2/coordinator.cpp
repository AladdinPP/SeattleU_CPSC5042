/**
 * @author  Hongru He
 * @file    coordinator.cpp
 * @version 1.0
 * @date    05/20/2024
 * @func    coordinator for handling the 2PC transaction
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "TCPClient.h"

using namespace std;

class coordinator {
public:
    // Constructor
    coordinator(const string& logFile, double transactionAmount,
                const string& sourceHost, u_short sourcePort,
                const string& sourceAccount, const string& destHost,
                u_short destPort, const string& destAccount) {
        log_file = logFile;
        amount = transactionAmount;
        source_host = sourceHost;
        source_port = sourcePort;
        source_account = sourceAccount;
        dest_host = destHost;
        dest_port = destPort;
        dest_account = destAccount;
    }

    // Main function to execute 2PC mechanism
    void run() {
        // Display the transaction information
        print_transaction_details();

        // Construct vote messages
        string source_vote_request = "VOTE-REQUEST -" + to_string(amount) + " " +
                                     source_account;
        string dest_vote_request = "VOTE-REQUEST " + to_string(amount) + " " +
                                   dest_account;

        // 2PC process
        try {
            // Initialize two clients / participants
            TCPClient source_client(source_host, static_cast<u_short>(source_port));
            cout << "Connected to participant " << source_host << ":" << source_port << endl;
            TCPClient dest_client(dest_host, static_cast<u_short>(dest_port));
            cout << "Connected to participant " << dest_host << ":" << dest_port << endl;

            // Sending vote requests
            source_client.send_request(source_vote_request);
            cout << "Sending message 'VOTE-REQUEST " << source_account << " -" << amount << "' to " << source_host << ":" << source_port << endl;
            string source_response = source_client.get_response();
            log("Source response: " + source_response);

            dest_client.send_request(dest_vote_request);
            cout << "Sending message 'VOTE-REQUEST " << dest_account << " " << amount << "' to " << dest_host << ":" << dest_port << endl;
            string dest_response = dest_client.get_response();
            log("Destination response: " + dest_response);

            // Receiving responses and construct global messages
            string global_message, source_global_message, dest_global_message;
            if (source_response == "VOTE-COMMIT" && dest_response == "VOTE-COMMIT") {
                source_global_message = "GLOBAL-COMMIT -" + to_string(amount) + " "
                                        + source_account;
                dest_global_message = "GLOBAL-COMMIT " + to_string(amount) + " "
                                      + dest_account;
                global_message = "GLOBAL-COMMIT";
            } else {
                source_global_message = "GLOBAL-ABORT";
                dest_global_message = "GLOBAL-ABORT";
                global_message = "GLOBAL-ABORT";

            }

            // Sending global messages
            cout << "Sending message '" << global_message << "' to " << source_host << ":" << source_port << endl;
            source_client.send_request(source_global_message);

            cout << "Sending message '" << global_message << "' to " << dest_host << ":" << dest_port << endl;
            dest_client.send_request(dest_global_message);

            // Recording global messages to log file
            log("Global message: " + global_message);

            // Receiving ACK messages and record to log file
            string source_ack = source_client.get_response();
            string dest_ack = dest_client.get_response();
            log("Source ACK: " + source_ack);
            log("Destination ACK: " + dest_ack);

            if (global_message == "GLOBAL-COMMIT") {
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
    double amount;
    string source_host, source_account, dest_host, dest_account;
    u_short source_port, dest_port;

    // Record messages to log file
    void log(const string &message) {
        ofstream log(log_file, ios_base::app);
        log << message << endl;
    }

    // Display the transaction information
    void print_transaction_details() {
        cout << "Transaction: $" << amount << endl;
        cout << "  From: " << source_host << ":" << source_port << " account #"
             << source_account << endl;
        cout << "  To:   " << dest_host << ":" << dest_port << " account #"
             << dest_account << endl;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 9) {
        cerr << "Usage: " << argv[0] << " <log_file> <amount> <source_host> <source_port> <source_account> <dest_host> <dest_port> <dest_account>" << endl;
        return 1;
    }

    // Parse the argv
    string log_file = argv[1];
    double amount = stod(argv[2]);
    string source_host = argv[3];
    int source_port = stoi(argv[4]);
    string source_account = argv[5];
    string dest_host = argv[6];
    int dest_port = stoi(argv[7]);
    string dest_account = argv[8];

    // Initialize coordinator
    coordinator coBank(log_file, amount, source_host, source_port,
                       source_account, dest_host, dest_port, dest_account);

    // Execute the 2PC transaction
    coBank.run();

    return EXIT_SUCCESS;
}