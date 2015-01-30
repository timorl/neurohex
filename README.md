Neurohex
========

A web game based on the great board game Neuroshima Hex.

Installation instructions
-------------------------

This program requires a linux distribution with the boost library at version at
least 1.54 installed. If you are using Ubuntu or some similar system `sudo
apt-get install libboost-all-dev` should install it, if you are using any other
distribution you probably know how to do this.

To install neccessary data files run the following commands:

`mkdir -p ~/.local/share/neurohex`

`cp -R data/* ~/.local/share/neurohex`

To compile the program run `make all`. Afterwards running `bin/NeurohexServer`
runs the server, while `bin/NeurohexClient` runs the client. They have internal
documentation, if you need to know any commands available at some point just
type `help`.
