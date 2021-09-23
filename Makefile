# Makefile for code in refactoring assignment

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

EXECUTABLES= socialNetworkRF$(EXECEXT)

all : $(EXECUTABLES)

linkedListQueue.o :	linkedListQueue.c abstractQueue.h
	gcc -c linkedListQueue.c

linkedListGraphRF.o :	linkedListGraphRF.c abstractGraph.h abstractQueue.h
	gcc -c linkedListGraphRF.c

menu.o : 	menu.c abstractSocialNetwork.h abstractMenu.h
	gcc -c menu.c

networkBuilder.o :	networkBuilder.c abstractGraph.h abstractNetworkBuilder.h abstractSocialNetwork.h
	gcc -c networkBuilder.c

socialNetworkRF.o :	socialNetworkRF.c abstractGraph.h abstractNetworkBuilder.h
	gcc -c socialNetworkRF.c


socialNetworkRF$(EXECEXT) : socialNetworkRF.o menu.o networkBuilder.o linkedListGraphRF.o linkedListQueue.o
	gcc -o socialNetworkRF$(EXECEXT) socialNetworkRF.o menu.o networkBuilder.o linkedListGraphRF.o linkedListQueue.o


clean : 
	-rm *.o
	-rm $(EXECUTABLES) 
