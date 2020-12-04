PROGRAM=tejadede

CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -Wextra -g -Wno-long-long -O0 -ggdb
LIBS=-lncurses

all: compile doc

run: compile
	./$(PROGRAM)

compile: $(PROGRAM)

doc: src/main.cpp src/CTable.h src/CCell.h src/CString.h src/CExpression.h src/CParser.h src/CRun.h src/CControllers.h src/CStartControl.h src/CHelpControl.h src/CTableControl.h src/CTable.cpp src/CCell.cpp src/CString.cpp src/CExpression.cpp src/CParser.cpp src/CRun.cpp src/CControllers.cpp src/CStartControl.cpp src/CHelpControl.cpp src/CTableControl.cpp
	doxygen Doxyfile

$(PROGRAM): objs/main.o objs/CTable.o objs/CCell.o objs/CString.o objs/CExpression.o objs/CParser.o objs/CRun.o objs/CControllers.o objs/CStartControl.o objs/CHelpControl.o objs/CTableControl.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

objs/main.o: src/main.cpp src/CRun.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CRun.o: src/CRun.cpp src/CRun.h src/CControllers.h src/CStartControl.h src/CTableControl.h src/CHelpControl.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CControllers.o: src/CControllers.cpp src/CControllers.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CStartControl.o: src/CStartControl.cpp src/CStartControl.h src/CControllers.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHelpControl.o: src/CHelpControl.cpp src/CHelpControl.h src/CControllers.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CTableControl.o: src/CTableControl.cpp src/CTableControl.h src/CControllers.h src/CTable.h src/CRun.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CCell.o: src/CCell.cpp src/CCell.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CTable.o: src/CTable.cpp src/CTable.h src/CCell.h src/CParser.h src/CString.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CString.o: src/CString.cpp src/CString.h src/CCell.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CExpression.o: src/CExpression.cpp src/CExpression.h src/CCell.h src/CParser.h src/CTable.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CParser.o: src/CParser.cpp src/CParser.h src/CTable.h src/CCell.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs:
	mkdir objs

clean:
	rm -rf $(PROGRAM) objs/ doc/ 2>/dev/null
	rm -f examples/*
