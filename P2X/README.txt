@author: Hongru He

==========READ ME==========

Extra credit (a) claimed: The TCPClient and coordinator have been modified to
deal with any number of participants in one transaction.

**********
COORDINATOR
**********
----------
Command line: ./coordinator <log_file> <amount_1> <host_1> <port_1> <account_1>
                                       <amount_2> <host_2> <port_2> <account_2>
                                       [......]

If the amount is out_bound, it should be negative. Else, it should be positive.
Make sure the sum of out_bound amount and in_bound amount is 0. Or else, this
transaction is invalid. The coordinator will be terminated. You need input
another command line to initialize a new transaction. However, invalid
transaction will not terminate participants. So, you only need to initialize
coordinator after one invalid transaction.

After one round of transaction, the coordinator will also be terminated.
----------


**********
PARTICIPANT
**********
----------
Command line: ./participant <port> <account_file> <log_file>

This will initialize a participant using the given post. It will read and write
to the given account file and log file.

After one round of transaction, the connection will end and the participant will
 be terminated.
----------

**********
FILES
**********
----------
Four account files and log files are provided to test. If you want, please
create more account files and log files. Each participant should have its own
account file and log file. The coordinator should have a separate log file.
----------