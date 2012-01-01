#pragma once

class BigInt
{
public:
	BigInt(int);
	BigInt(int, int);
	BigInt(int, void*);
	~BigInt(void);
	BigInt(const BigInt &s);
	BigInt &operator=(const BigInt &s);
	BigInt &operator+=(const BigInt &rhs);
	BigInt &operator-=(const BigInt &rhs);
	BigInt &operator+(const BigInt &rhs);
	BigInt &operator-(const BigInt &rhs);
	char* hex();

	static BigInt ONE;
	static BigInt ZERO;

	static int WS; //word size
	
	void* words;
private:
	
	int wc;

	void grow(int owc);
	void alloc(int nwc);
};

