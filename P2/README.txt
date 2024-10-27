@author: Hongru He

==========READ ME==========
***********
PARTICIPANT
-----------
Command line: ./participant <port> <account_file> <log_file>

This will initialize a participant using the given post. It will read and write
to the given account file and log file.

After one round of transaction, the connection will end and the participant will
 be terminated.
***********

***********
COORDINATOR
-----------
Command line: ./coordinator <log_file> <transaction_amount> <source_host>
<source_port> <source_account> <dest_host> <dest_port> <dest_account>

note: the transaction amount should be positive.

This will initialize a coordinator starting a 2PC transaction, requesting the
given amount, from the source account of the source host at the source port, to
the destination account of the destination host at the destination port. This
coordinator will record all the process to the given log file.

After one round of transaction, the connection will end and the coordinator will
 be terminated.
***********