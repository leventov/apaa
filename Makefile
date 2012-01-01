CXX = g++
CXXFLAGS = -O2

BIR = EABigInt

main: main.o BI_c.o $(BIR).o
	$(CXX) $(CXXFLAGS) -o main BI_c.o $(BIR).o main.cpp -lrt

$(BIR).o: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c $(BIR).cpp

BI_c.o: BigInt_common.cpp BigInt.h
	$(CXX) $(CXXFLAGS) -c -o BI_c.o BigInt_common.cpp


A.s: $(BIR).cpp BigInt.h
	$(CXX) $(CXXFLAGS) -o A.s -S $(BIR).cpp


