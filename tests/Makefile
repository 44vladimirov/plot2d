COMP    ?= c++
FLAGS   += -Wall
INCLUDE ?= -I /usr/local/include
LIB     ?= -L /usr/local/lib -l X11 -l GL -l m -L .. -l plot2d

sin : sin.cpp
	$(COMP) $(FLAGS) $(INCLUDE) $(LIB) -o sin sin.cpp

clean :
	rm -f sin
