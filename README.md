# Pogo
This is a todo list with 3 components pogo-daemon, pogo-cli and pogo-webserver

## Pogo-daemon
Is the program which manages the current state of the todo list with details on how programs other than pogo-cli and pogo-webserver can communicate with it in PROTOCOL.md

## Pogo-cli
Is a cli program which can be used to communicate with the daemon without having to directly deal with the mechanism that communicates with pogo-daemon

## Pogo-webserver
Is a webserver that serves up a webpage what can be used to update the todo list

##Building
can be done by running
```sh
mkdir build
cd build
cmake ..
make
```
