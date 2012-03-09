SHELL=/bin/bash
CC=/usr/bin/g++
DELETE_CMD=/bin/rm -f

LIBRARY_PATH=-L. -L../cmdLineParser
INCLUDE_PATH=-I. -I../cmdLineParser

CCFLAGS=-g
OBJ_EXTENSION=.o

EXE_NAME=loanCalculator
OBJS=LoanCalculator$(OBJ_EXTENSION) LoanCalculatorMain$(OBJ_EXTENSION)

#
# Targets
#

all: $(EXE_NAME) $(OBJS)

$(EXE_NAME): $(OBJS)
	$(CC) $(CCFLAGS) $(LIBRARY_PATH) -o $(EXE_NAME) $(OBJS) -lCmdLineParser

LoanCalculator$(OBJ_EXTENSION): LoanCalculator.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c LoanCalculator.cpp -o LoanCalculator$(OBJ_EXTENSION)

LoanCalculatorMain$(OBJ_EXTENSION): LoanCalculatorMain.cpp
	$(CC) $(CCFLAGS) $(INCLUDE_PATH) -c LoanCalculatorMain.cpp -o LoanCalculatorMain$(OBJ_EXTENSION)

clean:
	$(DELETE_CMD) $(OBJS) $(EXE_NAME)
