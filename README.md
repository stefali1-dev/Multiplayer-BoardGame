# Multiplayer-BoardGame


## Info
This project consists of a Server that allows clients (maximum 4 simultaneously) to play competitively the game [Don't Get Mad Bro](https://en.wikipedia.org/wiki/Mensch_%C3%A4rgere_Dich_nicht), which is similar to [Ludo](https://en.wikipedia.org/wiki/Ludo).

The Server is TCP Concurrent and handles multiple clients in parallel using Threads.


SFML 2.5.1 has been used for the client's graphic interface.

If the client wants to connect with the server that is run on the same machine, the `IP-adress` can be set to `0`.

The Server PORT is `2908`

## Build
### Prerequisites
- g++
- SFML 2.5.1

To build client executable run `g++ Interface.cpp -o client-app -lsfml-graphics -lsfml-window -lsfml-system`.

To build server executable run `g++ Server.cpp -o server-app`.



## Run
The binary files are named **client-app** and **server-app**, therefore you can run them using `./client-app` and `./server-app`.


## Screenshots
![screenshot](https://raw.githubusercontent.com/stefali1-dev/NTSF/main/src/schrenshot.png?token=GHSAT0AAAAAAB4I7F5HLTHEHN6W7YFMGAI2Y56VDEQ)
