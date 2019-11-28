#pragma once

class Register {
private:
	char A;
	char B;
	char C;
	char D;
public:
	Register() : A(0), B(0), C(0), D(0) {}
	Register(char A, char B, char C, char D) : A(A), B(B), C(C), D(D) {}
	void setA(char A) { this->A = A; }
	void setB(char B) { this->B = B; }
	void setC(char C) { this->C = C; }
	void setD(char D) { this->D = D; }
	char* getPointerA() { return &this->A; }
	char* getPointerB() { return &this->B; }
	char* getPointerC() { return &this->C; }
	char* getPointerD() { return &this->D; }
	char getA() { return this->A; }
	char getB() { return this->B; }
	char getC() { return this->C; }
	char getD() { return this->D; }

};