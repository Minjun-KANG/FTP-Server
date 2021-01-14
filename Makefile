#2017707007 Minjun_Kang
#Main forder makefile

DIRS = lib server client
.PHONY: all clean

all:
	@for d in $(DIRS); \
	do \
		$(MAKE) -C $$d; \
	done

clean:
	@for d in $(DIRS); \
	do \
		$(MAKE) -C $$d clean; \
	done
