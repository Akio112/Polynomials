#include "polynomials.h"
#include <vector>
#include <list>
#include <iostream>
#include<string>
#include <ostream>
#include <string>



long double BinPow(long double x, int pow) {
    if (pow == 0) return 1;
    if (pow == 1) return x;

    long double temp = BinPow(x, pow / 2);
    temp = temp * temp;
    if (pow%2) {
        temp *= x;
    }
    return temp;
}



constexpr long double eps = 1e-11;


//Monomials functions

Monomial::Monomial():degrees(std::vector<int>(26)), factor(0){}

Monomial::Monomial(long double factor_init, const std::vector<int>& degrees_init):degrees(degrees_init),
factor(factor_init){}

Monomial::Monomial(const Monomial& other):degrees(other.degrees), factor(other.factor) {}

long double Monomial::GetFactor() {
    return this->factor;
}

Monomial& Monomial::operator=(const Monomial& other) {
    Monomial temp(other);
    std::swap(*this, temp);
    return *this;
}

Monomial Monomial::operator*(const Monomial& other) {
    Monomial temp(*this);
    temp.factor *= other.factor;
    for (int i = 0;i < 26; ++i) {
        temp.degrees[i] += other.degrees[i];
    }
    return temp;
}

bool Monomial::operator <(const Monomial& other) const{
    return (this->degrees<other.degrees); // сортируем по степеням всех переменных одночлена
}

bool Monomial::operator >(const Monomial& other) const {
    return other<*this;
}

bool Monomial::operator >=(const Monomial& other) const {
    return !(*this<other);
}
bool Monomial::operator <=(const Monomial& other) const {
    return !(other<*this);
}
bool Monomial::operator==(const Monomial& other) const {
    return (*this>=other && *this <= other);
}

bool Monomial::operator!=(const Monomial& other) const {
    return !(*this==other);
}

void Monomial::Clear() {
    degrees = std::vector<int>(26);
    factor = 0;
}

std::ostream& operator << (std::ostream& os, const Monomial& this_mono)
{
    std::string temp = "";
    temp += std::to_string(this_mono.factor);
    for (int i = 0;i < 26; ++i) {
        if (this_mono.degrees[i] != 0) {
            temp.push_back('a'+i);
            if (this_mono.degrees[i] > 1) {
                temp.push_back('^');
                temp += std::to_string(this_mono.degrees[i]);
            }
        }
    }
    return os<<temp;
}


int CharToInd(char c) {
    return (c - 'a');
}


// Polynomial functions

Polynomial::Polynomial():monomials({}), variables(0){}


Polynomial::Polynomial(std::string s) {
    std::string now_factor = "", now_degree = "";
    char last_char = '0';
    Monomial now_mono;
    s.push_back('&');
    for (int i = 0;i < s.size(); ++i) {
        if (std::isdigit(s[i]) || s[i] == '.') {
            if (last_char == '-' || last_char == '+' || last_char == '0') {
                now_factor.push_back(s[i]);
            } else {
                now_degree.push_back(s[i]);
            }
        }
        if (std::isalpha(s[i]) || s[i] == '+' || s[i] == '-' || s[i] == '&') {
            if (i != 0) {
                if (last_char == '-' || last_char == '+' || last_char == '0') {
                    if (now_factor.size()) {
                        now_mono.factor = std::stold(now_factor);
                        now_factor = "";
                    } else {
                        now_mono.factor = 1;
                        now_factor = "";
                    }
                    if (last_char == '-') {
                        now_mono.factor *= -1;
                    }
                } else if (now_degree.size()) {
                        now_mono.degrees[CharToInd(last_char)] += std::stoi(now_degree);
                        now_degree = "";
                } else {
                    now_mono.degrees[CharToInd(last_char)]++;
                }
                if (s[i] == '+' || s[i] == '-' || s[i] == '&') {
                    this->monomials.push_back(now_mono);
                    now_mono.Clear();
                } else {
                    variables[CharToInd(s[i])] = 1;
                }
            }
            last_char = s[i];
            
        }
    }
    Stable();
}

void Polynomial::AddMono(const Monomial& other) {
    if (monomials.empty()) {
        monomials.push_back(other);
        return;
    }
    if (monomials.back() ==(other)) {
        monomials.back().factor += other.factor;
    } else {
        monomials.push_back(other);
    }
    if (abs(monomials.back().factor) < eps) {
        monomials.pop_back();
    }
}

void Polynomial::Stable() {
    monomials.sort();
    std::list<Monomial> temp = monomials;
    monomials.clear();
    for (Monomial mono_now:temp) {
        AddMono(mono_now);
    }
}

Polynomial Polynomial::operator+(const Polynomial& other) {
    Polynomial temp;
    auto it_this = this->monomials.begin();
    auto it_other = other.monomials.begin();
    while (it_this != this->monomials.end() && it_other != other.monomials.end()) {
        if (*it_this < *it_other) {
            temp.AddMono(*it_this);
            ++it_this;
        } else {
            temp.AddMono(*it_other);
            ++it_other;
        }
    }
    while (it_this != this->monomials.end()) {
        temp.AddMono(*it_this);
        ++it_this;
    }
    while (it_other != other.monomials.end()) {
        temp.AddMono(*it_other);
        ++it_other;
    }
    temp.variables = this->variables|other.variables;
    return temp;
}


Polynomial& Polynomial::operator=(const Polynomial& other) {
    this->monomials = other.monomials;
    return *this;
}



Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = (*this + other);
    return *this;
}

Polynomial Polynomial::operator*(const Monomial& other) {
    Polynomial temp;
    for (Monomial mono_now:monomials) {
        temp.AddMono(mono_now * other);
    }
    return temp;
}

Polynomial Polynomial::operator*(const Polynomial& other) {
    Polynomial temp;
    for (Monomial mono_other:other.monomials) {
        temp += (*this) * mono_other;
    }
    return temp;
}

bool Polynomial::operator==(const Polynomial& other) const {
    if (other.monomials.size() != this->monomials.size() || other.variables != this->variables) {
        return false;
    }
    auto it_this = this->monomials.begin();
    auto it_other = other.monomials.begin();
    while (it_other != other.monomials.end()) {
        if ((*it_other)!= (*it_this)) {
            return false;
        }
        ++it_other;
        ++it_this;
    }
    return true;
}

long double Polynomial::Get() {
    long double res = 0;
    std::vector<int> params(26);
    for (int i = 0;i < 26; ++i) {
        if (variables[i]) {
            std::cout << "Введите значение для переменной " << char(i+'a') << ":" << std::endl;
            std::cin >> params[i];
        }
    }
    for (Monomial mono_now:monomials) {
        long double temp = mono_now.factor;
        for (int i = 0;i < 26; ++i) {
            temp *= BinPow(params[i], mono_now.degrees[i]);
        }
        res += temp;
    }
    return res;
}

std::ostream& operator << (std::ostream& os, const Polynomial& this_poly)
{
    for (auto now_mono:this_poly.monomials) {
        if (now_mono.GetFactor() >= 0) {
            os << '+';
        }
        os << now_mono;
    }
    return os;
}