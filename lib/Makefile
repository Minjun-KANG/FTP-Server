#2017707007 Minjun_Kang
#Lib makefile

CC = gcc
LIB = libCMN_func.a 
OBJS = CMN_func.o
CFLAGS = -Wall

#INC = '-I/home/kang/Desktop/Source/Project(0729)/Header'
#HEADER = $(INC)'/Main_header.h'



$(LIB): $(OBJS)
	$(AR) rv $@ $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) -I../header $< -o $@

clean :
	rm -f $(OBJS) $(LIB)



