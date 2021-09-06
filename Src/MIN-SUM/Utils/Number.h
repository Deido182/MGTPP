#ifndef NUMBER_HEADER
#define NUMBER_HEADER

typedef long long ll;

class Number {
	public:
		ll value;
		bool inf = false;
		
		Number(ll v);
		Number();
		
		void operator=(const Number& n);
};

const Number& operator+(const Number& n1, const Number& n2);
const Number& operator-(const Number& n1, const Number& n2);	
bool operator<(const Number& n1, const Number& n2);
bool operator==(const Number& n1, const Number& n2);
bool operator<=(const Number& n1, const Number& n2);
bool operator>(const Number& n1, const Number& n2);
bool operator!=(const Number& n1, const Number& n2);
bool operator>=(const Number& n1, const Number& n2);

#endif
