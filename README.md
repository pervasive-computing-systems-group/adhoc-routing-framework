# Ad hoc Network Routing Protocol Implementations

### NASA SmallSat Project - Colorado School of Mines
### Classes designed for physical implementation and simulated implementation. 

Author: Josh Rands

## To Do
1. Add functions for generating RREQ
2. Add functions for generating RREP
  - Create struct for packet format (section 5.2)
  - Add check in RREQ if time for RREP (section 6.6)
  - Unicast RREP message back to orig (section 6.7)
3. Add functions for generating RERR
  - Create struct for packet format (section 5.3)
  - Generate RERR in all 3 situations (section 6.11)
4. Add TTL for RREQ dissemination (section 6.4)
5. Add AODVRoutingTable updates 

## Running the code

This code is currently stand alone classes, but I added test.cc to so we can test while we develop. 

1. Clone this repository 
2. Using a terminal, cd into the src directory and type 'make'
3. Code should compile and you can run ./test to run the tests

I use Visual Studio Code to develop. It has a nice built in terminal. 

## AODV Protocol: 
https://www.ietf.org/rfc/rfc3561.txt
