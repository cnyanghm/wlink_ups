

GCC = ${prefix}gcc

CFLAGS = -x c -Wall -fPIC -g
CFLAGS += -std=gnu99
CFLAGS += -Wno-unused-but-set-variable

	
INCPATH = -I. 


SOURCES = main.c



OBJECTS = ${SOURCES:%.c=%.o}



all : ${OBJECTS}
	@echo [L] $@
	@${GCC} ${LINKFLAGS} -o t main.o

${OBJECTS}: %.o: %.c
	@echo [C] $<
	@${GCC}  ${CFLAGS} ${INCPATH} -c $< -o $@

clean :
	rm -f *.o *~
