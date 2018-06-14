COMP = g++-5.3.0
#put all your object files here
OBJS = main.o

#The executabel filename DON'T CHANGE
EXEC = ex4

CPP_COMP_FLAG = -std=c++14 -DNDEBUG -g 
#-Wall -Wextra -pedantic-errors -Werror

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@


#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies

main.o: main.cpp unit_test_util.h ex4_header.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)

