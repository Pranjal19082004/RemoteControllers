CC = g++
CFLAGS = -c -Wall -O2
LDFLAGS_CONSOLE = -lws2_32          # Console app (Client)
LDFLAGS_GUI = -mwindows -municode -lws2_32 # GUI app (Server)

all: server client

server: serverPc.exe

serverPc.exe: serverPc.o ServerNetworking.o
	$(CC) serverPc.o ServerNetworking.o -o serverPc.exe $(LDFLAGS_GUI)

serverPc.o: serverPc.cpp ServerNetworking.h
	$(CC) $(CFLAGS) serverPc.cpp

ServerNetworking.o: ServerNetworking.cpp ServerNetworking.h
	$(CC) $(CFLAGS) ServerNetworking.cpp

client: clientPc.exe

clientPc.exe: clientPc.o mouseEvents.o
	$(CC) clientPc.o mouseEvents.o -o clientPc.exe $(LDFLAGS_CONSOLE)

clientPc.o: clientPc.cpp mouseEvents.h
	$(CC) $(CFLAGS) clientPc.cpp

mouseEvents.o: mouseEvents.cpp mouseEvents.h
	$(CC) $(CFLAGS) mouseEvents.cpp

clean:
	rm -f *.o clientPc.exe serverPc.exe
