CS176A Homework 2 Programming Assignment

----------------------------------------------------

Project Description:
This project is part of a homework assignment from the UCSB CS176A course. It involves creating both a UDP and a TCP 
client and server. The prompt for the assignment is as follows and accurately represents the function of the project:

The server will start in passive mode listening on a specified port for a transmission from a client.
Separately, the client will be started and will contact the server on a given IP address and port
number that must be entered via the command line. The client will pass the server a string
consisting of a sequence of characters. If the string contains anything but numbers, the server will
respond with "Sorry, cannot compute!" and close the connection. If the string contains all
numbers, the individual digits will be added together and returned as a string (see below for an
example). If the server receives a string of numbers, it will (1) add the digits together, (2) send
the value back to the client, and (3) does not close the connection unless the response is a single
digit. The server repeats this process by adding the digits of the new number together and sending
the response to the client until there is only one digit remaining in the sum. The server sends that
final one digit sum, and then closes the connection. The maximum string length you need to
accommodate is 128 characters. 

Due to the requirements of the assignment, the project was coded using C. 

---------------------------------------------------

Project Files:

Makefile          - Allows for compilation of code
client_c_tcp.c    - TCP client implementation
server_c_tcp.c    - TCP server implementation
client_c_udp.c    - UDP client implementation
server_c_udp.c    - UDP server implementation

Building the Project:
To compile all programs, type: 
    make all

This creates four executable files:
    client_c_tcp
    server_c_tcp
    client_c_udp
    server_c_udp

---------------------------------------------------

Running the Programs:

TCP Version:
1. Start the TCP server:
    ./server_c_tcp <port>
    Example: ./server_c_tcp 9999

2. Run the TCP client:
    ./client_c_tcp <hostname> <port>
    Example: ./client_c_tcp localhost 9999

UDP Version:
1. Start the UDP server:
    ./server_c_udp <port>
    Example: ./server_c_udp 9999

2. Run the UDP client:
    ./client_c_udp <hostname> <port>
    Example: ./client_c_udp localhost 9999

---------------------------------------------------

Example Usage:
------------
Input: 276
Output:
    From server: 276
    From server: 15
    From server: 6

Input: abc
Output:
    From server: Sorry, cannot compute!

----------------------------------------------------

Credits:
https://www.linuxhowtos.org/data/6/server.c
    Provided skeleton code for server_c_tcp.c
https://www.linuxhowtos.org/data/6/client.c
    Provided skeleton code for client_c_tcp.c
https://www.linuxhowtos.org/data/6/client_udp.c
    Provided skeleton code for client_c_udp.c
https://www.linuxhowtos.org/data/6/server_udp.c
    Provided skeleton code for server_c_udp.c
 https://www.freecodecamp.org/news/how-to-write-a-good-readme-file/
    Provided guidelines for writing a README