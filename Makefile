SUBDIRS=CodeGen \
RPC_client \
RPC_server \
example

all:$(SUBDIRS)

$(SUBDIRS):ECHO
	cd $(PWD)/$@ && make

ECHO:
	mkdir -p $(PWD)/RPC_client/lib
	mkdir -p $(PWD)/RPC_server/lib

.PHONY:clean
clean:
	cd $(PWD)/CodeGen && make clean
	cd $(PWD)/RPC_client && make clean
	cd $(PWD)/RPC_server && make clean
	cd $(PWD)/example && make clean	
