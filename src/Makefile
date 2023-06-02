CC = g++

CFLAGS = -Wall -I/usr/include/python3.7
PYTHONFLAGS = -lwiringPi -lpython3.7m

BIN_DIR = bin/

main: main.cpp kernel.o module.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)main main.cpp $(BIN_DIR)kernel.o $(BIN_DIR)module.o $(PYTHONFLAGS)

kernel.o: kernel.cpp kernel.h module.h
	mkdir -p $(BIN_DIR) && $(CC) $(CFLAGS) -c kernel.cpp -o $(BIN_DIR)kernel.o

module.o: module.cpp module.h
	$(CC) $(CFLAGS) -c module.cpp -o $(BIN_DIR)module.o

clean: 
	rm -f $(BIN_DIR)main $(BIN_DIR)*.o

