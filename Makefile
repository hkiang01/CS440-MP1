
#
EXE = mp1
OBJS = main.o

COMPILER = g++
COMPILER_OPTS = -c -g -O0 -Wall -Werror
LINKER = g++
#LINKER_OPTS = -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

main.o : main.cpp
	$(COMPILER) $(COMPILER_OPTS) main.cpp
	
clean :
	-rm -f *.o $(EXE)

