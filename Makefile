CXX = g++
# -O3 for asm preferable
# -O1, -O2 or -O3 for cpp
CXXFLAGS = -O3 

# cpp - OBigInt, NBigInt
# asm - ABigInt, EABigInt
BIR = ABigInt

main: main.o BI_c.o $(BIR).o
	$(CXX) $(CXXFLAGS) -o main BI_c.o $(BIR).o main.cpp -lrt

$(BIR).o: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c $(BIR).cpp

BI_c.o: BigInt_common.cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c -o BI_c.o BigInt_common.cpp

# to see assembly
as: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -S $(BIR).cpp
	
	
# to see assembly and bytes
dump: $(BIR).o
	objdump -d $(BIR).o > $(subst BigInt,,$(BIR)).s 
	



