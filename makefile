# Please implement your Makefile rules and targets below.
main: bin/main.o bin/Facility.o bin/SelectionPolicy.o
	@echo 'Building target: main'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/main bin/main.o bin/Facility.o bin/SelectionPolicy.o
	@echo 'Finished building target: main'
	@echo ' '	

bin/main.o : src/main.cpp include/Simulation.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/SelectionPolicy.o : src/SelectionPolicy.cpp include/SelectionPolicy.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp	

bin/Facility.o : src/Facility.cpp include/Facility.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

clean:
	rm -f bin/*	
# Customize this file to define how to build your project.