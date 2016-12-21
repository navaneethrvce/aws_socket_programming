all: servers.cpp servers.h client.cpp
	g++ servers.cpp -lnsl -lresolv -lsocket -o servers
	g++ client.cpp -lnsl -lresolv -lsocket -o client

Servers : 
	./servers
