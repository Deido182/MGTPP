#include "Number.h"
#include <iostream>

using namespace std;

typedef long long ll;

Number::Number(ll v) {
	value = v;
}

Number::Number() {
	minus_inf = true;
}

void Number::operator=(const Number& n) {
	value = n.value;
	minus_inf = n.minus_inf;
}

const Number& operator+(const Number& n1, const Number& n2) {
	return *(n1.minus_inf || n2.minus_inf ? new Number() : new Number(n1.value + n2.value));
}
		
const Number& operator-(const Number& n1, const Number& n2) {
	return *(n1.minus_inf || n2.minus_inf ? new Number() : new Number(n1.value - n2.value));
}
				
bool operator<(const Number& n1, const Number& n2) {
	return n1.minus_inf || (!n2.minus_inf && n1.value < n2.value);
}
		
bool operator==(const Number& n1, const Number& n2) {
	return n1.minus_inf && n2.minus_inf || !n1.minus_inf && !n2.minus_inf && n1.value == n2.value;
}
		
bool operator<=(const Number& n1, const Number& n2) {
	return n1 < n2 || n1 == n2;
}
		
bool operator>(const Number& n1, const Number& n2) {
	return !(n1 <= n2);
}
		
bool operator!=(const Number& n1, const Number& n2) {
	return !(n1 == n2);
}
		
bool operator>=(const Number& n1, const Number& n2) {
	return !(n1 < n2);
}
