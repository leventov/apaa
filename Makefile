CXX = g++
# -O2 for asm preferable
# -O1, -O2 or -O3 for cpp
CXXFLAGS = -O3 

# cpp - OBigInt, NBigInt
# asm - ABigInt, EABigInt
BIR = NBigInt

main: main.o BI_c.o $(BIR).o
	$(CXX) $(CXXFLAGS) -o main BI_c.o $(BIR).o main.cpp -lrt -lgmp

$(BIR).o: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c $(BIR).cpp

BI_c.o: BigInt_common.cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c -o BI_c.o BigInt_common.cpp

# too see assembly
as: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -S $(BIR).cpp


