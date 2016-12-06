# Simple Network Time Server and Client in C

Submission for teaching block 1 CNOS coursework

## Synopsis

An implementation of the SNTP client and server specified here in [RFC4330](https://tools.ietf.org/html/rfc4330)

## Code Example

### Server

Run the server with `./sntp-server` accepted arguments are: 
<br/>`-help`, lists all options.
<br/>`-port`, sets the port number.
<br/>`-multicast`, puts the server in multicast mode, will listen on multicast group `224.0.1.1`
<br/><br/>Multiple arguments can be passed. 
 
### Client

Run the client with `./sntp-client`, accepted arguments are: 
<br/>`-help`, lists all options.
<br/>`-port`, sets the port number.
<br/>`-host`, sets the hostname of the server to contact.
<br/>`-multicast`, puts the client in multicast mode, will request on multicast group `224.0.1.1`
<br/><br/>Multiple arguments can be passed. 

## Installation

Use `CMAKE` to generate the executables and test suite.

## Tests

The test suite uses the googletest framework to acheive test coverage, load the project into an IDE to run the test suite or run `checks.cpp` from the command line. `CMAKE` will handle the set up.  

## Contributors

Nathaniel Steers (SN 10018627)

