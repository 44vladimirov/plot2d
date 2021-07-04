COMP    ?= c++
FLAGS   += -Wall
INCLUDE ?= -I /usr/local/include

plot2d.a : plot2d.o
	ar -rsc plot2d.a plot2d.o

plot2d.o : plot2d.cpp plot2d.hpp
	$(COMP) $(FLAGS) $(INCLUDE) -c -o plot2d.o plot2d.cpp

install : plot2d.a
	cp plot2d.a libplot2d.a

clean :
	rm -f plot2d.a plot2d.o
