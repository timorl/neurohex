Neurohex
========

A web game based on the great board game Neuroshima Hex.

Installation instructions
-------------------------

This program requires a linux distribution with the boost library at version at
least 1.54 and cmake version at least 2.8 installed. If you are using Ubuntu or
some similar system `sudo apt-get install libboost-all-dev` followed by
`sudo apt-get install cmake` should install those, if you are using any other
distribution you probably know how to do this.

To install neccessary data files run the following commands:

`mkdir -p ~/.local/share/neurohex`

`cp -R data/* ~/.local/share/neurohex`

To compile the program run `cmake -G "Unix Makefiles"` followed by `make all`.
Afterwards running `bin/NeurohexServer` runs the server, while `bin/NeurohexClient`
runs the client. They have internal documentation, if you need to know any commands
available at some point just type `help`.
