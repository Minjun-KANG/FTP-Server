#2017707007 Minjun_Kang
#Client makefile

TARGET = Client
CC = gcc
OBJS = client.o Myclient_func.o
CFLAGS = -Wall

#INC = '-I/home/kang/Desktop/Source/Project(0729)/Header'
#HEADER = $(INC)'/Main_header.h'



$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -L../lib -lCMN_func

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ -I../header

clean :
	rm -f $(OBJS)
	rm -f $(TARGET)



