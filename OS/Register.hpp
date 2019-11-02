#pragma once

class Register {
private:
	int A;
	int B;
	int C;
	int D;
public:
	Register() : A(0), B(0), C(0), D(0) {}
	Register(int A, int B, int C, int D) : A(A), B(B), C(C), D(D) {}
	void setA(int A) { this->A = A; }
	void setB(int B) { this->B = B; }
	void setC(int C) { this->C = C; }
	void setD(int D) { this->D = D; }
	int* getPointerA() { return &this->A; }
	int* getPointerB() { return &this->B; }
	int* getPointerC() { return &this->C; }
	int* getPointerD() { return &this->D; }
	int getA() { return this->A; }
	int getB() { return this->B; }
	int getC() { return this->C; }
	int getD() { return this->D; }

};