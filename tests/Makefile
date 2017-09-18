CFLAGS = -Wall
LDFLAGS = 

target = ipc_client
srcs = main_client.c list.c shm.c msgq.c libipc.c
objs = $(srcs:.c=.o)
headers = $(wildcard *.h)


.PHONY: all
all: $(target)

$(target): $(objs) $(headers) FORCE
	$(CC) -o $@ $(objs) $(LDFLAGS)  -lrt -g -lpthread
	@-rm -f *.o

$(objs):%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<  -lrt -g

.PHONY: FORCE
FORCE: 

clean:
	rm -f $(target) *.o

