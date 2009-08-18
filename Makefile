CPP = g++
OFLAG = -o
.SUFFIXES : .o .cpp .c
.cpp.o :
	$(CPP) $(CPPFLAGS) -c $<
.c.o :
	$(CPP) $(CPPFLAGS) -c $<

all: fretcalc

test: all 
	fretcalc

clean:
	rm -f *.o fretcalc
	rm -f calc.out

bugs: 
	@echo No compiler bugs in this directory!

fretcalc: fretcalc.o FretCalculator.o 
	$(CPP) $(OFLAG) fretcalc -lm fretcalc.o FretCalculator.o 

FretCalculator.o: FretCalculator.cpp FretCalculator.h 
fretcalc.o: fretcalc.cpp FretCalculator.h 
