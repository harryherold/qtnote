# DbNote

.PHONY: clean
CORE:= ../core
CXX := g++
CXXFLAGS := -c -g -Wall
OBJS := test_date.o $(CORE)/date.o 
USE := boost_unit_test_framework

#rule one
test_date : $(OBJS)
	$(CXX)  -o $@ $^  -l$(USE)
#rule two
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $(CORE)/date.o
