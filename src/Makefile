
CXX=g++
RM=rm -rf
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)

CLIENT= rdict_client
SERVER= rdict_server

SRC=rdict.x

build:
	make -f Makefile.rdict

run_client:
	./$(CLIENT) localhost 100

run_server:
	./$(SERVER)

clean:
	$(RM) $(SERVER) $(CLIENT) *.o

clean_destructive:
	$(RM) Makefile.rdict rdict.h rdict_clnt.c rdict_svc.c rdict_client.c rdict_server.c rdict_xdr.c $(SERVER) $(CLIENT) *.o
