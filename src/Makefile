CLIENT= rdict_client
SERVER= rdict_server

SRC=rdict.x

.PHONY: all
all:
	@echo -n "Available make targets"
	@echo ":"
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | sed -e 's/^/   /'

build:
	make -f Makefile.rdict

rpcgen:
	rpcgen -a -M $(SRC)

run_client:
	./$(CLIENT) localhost

run_server:
	./$(SERVER)

clean:
	$(RM) $(SERVER) $(CLIENT) *.o

clean_destructive:
	$(RM) Makefile.rdict rdict.h rdict_clnt.c rdict_svc.c rdict_client.c rdict_server.c rdict_xdr.c $(SERVER) $(CLIENT) *.o
