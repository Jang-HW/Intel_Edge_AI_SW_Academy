#CC:=arm-linux-gcc
CC:=gcc

TARGET_CLN=iot_client
OBJECT_CLN=$(TARGET_CLN).o 

#LDFLAGS=-D_REENTRANT -pthread -lmysqlclient
LDFLAGS=-D_REENTRANT -pthread

all : $(TARGET_CLN)

$(TARGET_CLN):$(OBJECT_CLN)
	$(CC) -o $@ $(OBJECT_CLN) $(LDFLAGS)
%.o:%.c
	$(CC) -c -o $@ $<
clean:
	rm -f *.o $(TARGET_CLN)
