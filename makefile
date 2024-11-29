# Please implement your Makefile rules and targets below.
main: bin/main.o bin/Facility.o bin/SelectionPolicy.o bin/Simulation.o bin/Plan.o bin/Action.o bin/Settlement.o bin/Auxiliary.o
	@echo 'Building target: main'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/main bin/main.o bin/Facility.o bin/SelectionPolicy.o bin/Simulation.o bin/Plan.o bin/Action.o bin/Settlement.o bin/Auxiliary.o
	@echo 'Finished building target: main'
	@echo ' '	

bin/main.o : src/main.cpp include/Simulation.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/SelectionPolicy.o : src/SelectionPolicy.cpp include/SelectionPolicy.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp	

bin/Facility.o : src/Facility.cpp include/Facility.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

bin/Simulation.o: src/Simulation.cpp include/Simulation.h 
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

bin/Plan.o : src/Plan.cpp include/Plan.h	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp	

bin/Action.o : src/Action.cpp include/Action.h	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Settlement.o : src/Settlement.cpp include/Settlement.h	
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp	

bin/Auxiliary.o : src/Auxiliary.cpp include/Auxiliary.h
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp	

clean:
	rm -f bin/*	
# Customize this file to define how to build your project.