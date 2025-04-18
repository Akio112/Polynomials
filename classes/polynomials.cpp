#include "polynomials.h"
#include "list.h"
#include <math.h>
#include <vector>
#include <iostream>
#include<string>
#include <ostream>
#include <string>


constexpr long double eps = 1e-11;


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





//Monomials functions

Monomial::Monomial():degrees(std::vector<int>(26)), factor(0){}

Monomial::Monomial(long double factor_init, const std::vector<int>& degrees_init = std::vector<int>(26)):degrees(degrees_init),
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
    if (abs(this_mono.factor - ((int)this_mono.factor)) < eps) {
        temp += std::to_string((int)this_mono.factor);
    } else {
        temp += std::to_string(this_mono.factor);
    }
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
    char last_char = '0', last_var= '0';
    Monomial now_mono;
    bool was_factor = 0;
    s.push_back('&');
    for (int i = 0;i < s.size(); ++i) {
        if (isdigit(s[i]) || s[i] == '.') {
            if (last_char == '-' || last_char == '+' || last_char == '0') {
                now_factor.push_back(s[i]);
            } else {
                now_degree.push_back(s[i]);
            }
        }
        if (isalpha(s[i]) || s[i] == '+' || s[i] == '-' || s[i] == '&') {
            if (i != 0) {
                if (!was_factor) {
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
                    was_factor = 1;
                }
                if (last_var != '0') {
                    if (now_degree.size()) {
                        now_mono.degrees[CharToInd(last_var)] += std::stoll(now_degree);
                        now_degree = "";
                    } else {
                        now_mono.degrees[CharToInd(last_var)]++;
                    }
                }
                if (s[i] == '+' || s[i] == '-' || s[i] == '&') {
                    this->monomials.push_back(now_mono);
                    last_var = '0';
                    was_factor = 0;
                    now_mono.Clear();
                } else {
                    variables[CharToInd(s[i])] = 1;
                }
            }
        }
        if (!isdigit(s[i]) && s[i] != ' ' && s[i] != '.') {
            if (isalpha(s[i])) {
                last_var = s[i];
            } else {
                last_char = s[i];
            }
        }
    }
    Stable();
}

Polynomial::Polynomial(const Polynomial& other):monomials(other.monomials), variables(other.variables) {}

void Polynomial::AddMono(const Monomial& other) {
    if (monomials.empty()) {
        monomials.push_back(other);
    } else if (monomials.back() ==(other)) {
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
    List temp = monomials;
    monomials.clear();
    for (Monomial mono_now:temp) {
        AddMono(mono_now);
    }
    variables = 0;
    for (Monomial mono_now:this->monomials) {
        for (int i = 0;i < 26; ++i) {
            if (mono_now.degrees[i]) {
                this->variables[i] = 1;
            }
        }
    }
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
    Polynomial temp;
    auto it_this = this->monomials.begin();
    auto it_other = other.monomials.begin();
    while (it_this != this->monomials.end() && it_other != other.monomials.end()) {
        if (*it_this < *it_other) {
            temp.AddMono(Monomial(*it_this));
            ++it_this;
        } else {
            temp.AddMono(Monomial(*it_other));
            ++it_other;
        }
    }
    while (it_this != this->monomials.end()) {
        temp.AddMono(Monomial(*it_this));
        ++it_this;
    }
    while (it_other != other.monomials.end()) {
        temp.AddMono(Monomial(*it_other));
        ++it_other;
    }
    temp.variables = 0;
    for (Monomial mono_now:temp.monomials) {
        for (int i = 0;i < 26; ++i) {
            if (mono_now.degrees[i]) {
                temp.variables[i] = 1;
            }
        }
    }
    return temp;
}
Polynomial Polynomial::operator-() const{
    Polynomial res(*this);
    for (Monomial& mono_now:res.monomials) {
        mono_now.factor = -mono_now.factor;
    }
    return res;
}


Polynomial& Polynomial::operator=(const Polynomial& other) {
    monomials = other.monomials;
    variables = other.variables;
    return *this;
}



Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = (*this + other);
    return *this;
}

Polynomial Polynomial::operator*(const Monomial& other) const{
    Polynomial temp;
    for (Monomial mono_now:monomials) {
        temp.AddMono(mono_now * other);
    }
    return temp;
}

Polynomial Polynomial::operator*(const Polynomial& other) const{
    Polynomial temp;
    for (Monomial mono_other:other.monomials) {
        temp += (*this) * mono_other;
    }
    return temp;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    return *this + (-other);
}

std::pair<Polynomial, Polynomial> Polynomial::operator/(const Polynomial& other) {
    if (((this->variables)|other.variables).count() !=1) {
        throw "Нельзя делить многочлены с не одной пееременной!";
    }

    int x = 0;
    for (int i = 0;i < 26;++i) {
        if (this->variables[i] || other.variables[i]) {
            x = i;
        }
    }
    Polynomial res, rem(*this);
    while (!rem.monomials.empty() && !other.monomials.empty() && rem.monomials.back().degrees[x] >=
    other.monomials.back().degrees[x]) {
        Monomial last_rem = rem.monomials.back(), last_div = other.monomials.back();
        Monomial new_mono(last_rem.factor/last_div.factor, std::vector<int>(26));
        new_mono.degrees[x] = last_rem.degrees[x] - last_div.degrees[x];
        res.AddMono(new_mono);
        rem = rem-other * new_mono;
    }
    rem.Stable();
    res.Stable();
    return {res, rem};
}

Polynomial Polynomial::Derivative(int k, int var) {
    if (var > 25 || var < 0) {
        throw "Нельзя посчитать производную не от переменной";
    }
    Polynomial res(*this);
    for (int i = 0;i < k && res.size(); ++i) {
        std::vector<List<Monomial>::Iterator> iters_del;
        for (auto it = res.monomials.begin(); it != res.monomials.end(); ++it) {
            if (it->degrees[var]== 0) {
                iters_del.push_back(it);
            } else {
                it->factor *= it->degrees[var];
                it->degrees[var]--;
            }
        }
        for (List<Monomial>::Iterator it_del:iters_del) {
            res.monomials.erase(it_del);
        }
    }
    res.Stable();
    if (res.empty()) {
        res.AddMono(Monomial(0));
    }
    return res;
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

bool Polynomial::Availible(int x) const {
    return variables[x];
}

int Polynomial::size()const{
    return monomials.size();
}

bool Polynomial::empty() const{
    return (size()==0);
}

long double Polynomial::Get(const std::vector<int>& params) {
    long double res = 0;
    for (Monomial mono_now:monomials) {
        long double temp = mono_now.factor;
        for (int i = 0;i < 26; ++i) {
            temp *= BinPow(params[i], mono_now.degrees[i]);
        }
        res += temp;
    }
    return res;
}

std::vector<int> Polynomial::Roots() {
    if (variables.count() != 1) {
        throw "нельзя искать корень у многочлена не с одной переменной!";
    }
    if (monomials.empty()) {
        throw "нельзя найти корни у пустого многочлена!";
    }
    int ind_var = 0;
    for (int i = 0;i < 26;++i) {
        if (variables[i]) {
            ind_var = i;
        }
    }
    std::vector<int> ans;
    std::vector<int> params(26);
    long double free_factor = monomials.front().factor;
    if (abs(free_factor - (int)free_factor) >= eps) {
        return ans;
    }
    int int_free_fact = free_factor;
    for (int div = 1;div <= int_free_fact; ++div) {
        if (int_free_fact%div == 0) {
            params[ind_var] = div;
            if (abs(Get(params)) < eps) {
                ans.push_back(div);
            }
            params[ind_var] = -div;
            if (abs(Get(params)) < eps) {
                ans.push_back(-div);
            }
            params[ind_var] = 0;
        }
    }
    if (Get(params) < eps) {
        ans.push_back(0);
    }
    return ans;
}



std::ostream& operator << (std::ostream& os, const Polynomial& this_poly)
{
    bool first = 1;
    for (auto now_mono:this_poly.monomials) {
        if (now_mono.GetFactor() >= 0 && !first) {
            os << '+';
        }
        os << now_mono;
        first = 0;
    }
    if (this_poly.empty()) {
        os << 0;
    }
    return os;
}


void DFAPolynomial::CheckSymbol(char symb) {
    switch (current_state)
    {
        case INVALID:
            break;
        case START:
            if (symb == ' '){
            //
            } else if (symb == '+' || symb == '-') {
                current_state = SIGN;
            } else if (isdigit(symb)) {
                current_state = DIGIT_FACT;
            } else if(isalpha(symb)&& islower(symb)){
                current_state = ALPHA;
            }else {
                current_state = INVALID;
                throw "В начале могут быть только символы, цифры или переменные!";
            } 
            break;
        case SIGN:
            if (symb == ' ') {
                //
            } else if (isalpha(symb) && islower(symb)) {
                current_state = ALPHA;
            } else if(isdigit(symb)) {
                current_state = DIGIT_FACT;
            } else {
                current_state = INVALID;
                throw "После знака может быть цифра или переменная!";
            }
            break;
        case DIGIT_FACT:
            if (isdigit(symb)) {
                //
            } else if (symb == '.') {
                current_state = DOT;
            } else if (symb == ' ') {
                current_state = DIGIT_SPACE;
            } else if (isalpha(symb)&& islower(symb)){
                current_state = ALPHA;
            } else if (symb == '+' || symb == '-') {
                current_state = SIGN;
            } else {
                current_state = INVALID;
                throw "после коэффицента должен быть либо многочлен, либо знак!";
            }
            break;
        case DOT:
            if (isdigit(symb)) {
                current_state = AFTER_DOT;
            } else {
                current_state = INVALID;
                throw "После точки не может быть ничего, кроме продолжения многочлена!";
            }
            break;
        case AFTER_DOT:
            if (isdigit(symb)) {
                // 
            } else if (symb == ' ') {
                current_state = DIGIT_SPACE;
            } else if (isalpha(symb)&& islower(symb)) {
                current_state = ALPHA;
            } else if (symb == '+' || symb == '-'){
                current_state = SIGN;
            } else {
                current_state = INVALID;
                throw "после коэффицента должен быть либо многочлен, либо знак!";
            }
            break;
        case DIGIT_SPACE:
            if (symb == ' ') {
                //
            } else if (symb == '+' || symb == '-') {
                current_state = SIGN;
            } else {
                current_state = INVALID;
                throw "После отделенного пробелом числа может стоять только знак!";
            }
            break;
        case ALPHA:
            if (isalpha(symb) && islower(symb)) {
                //
            } else if (symb == ' ') {
                current_state = ALPHA_SPACE;
            } else if (symb == '+' || symb == '-') {
                current_state = SIGN;
            } else if (symb == '^') {
                current_state = DEGREE;
            } else {
                current_state = INVALID;
                throw "после переменной не должно быть цифр или иных неподходящих знаков!";
            }
            break;
        case ALPHA_SPACE:
            if (symb == ' ') {
                //
            } else if (symb == '^') {
                current_state = DEGREE;
            } else if (symb == '+' || symb == '-'){
                current_state = SIGN;
            } else {
                current_state = INVALID;
                throw "Между переменными или многочленами не должно быть такого символа!";
            }
            break;
        case DEGREE:
            if (symb == ' ') {
                //
            } else if (isdigit(symb)) {
                current_state = DIGIT_DEG;
            } else {
                current_state = INVALID;
                throw "После знака степени может быть только степень в виде цисла!";
            }
            break;
        case DIGIT_DEG:
            if (isdigit(symb)) {
                //
            } else if(isalpha(symb)&& islower(symb)) {
                current_state = ALPHA;
            } else if(symb == '+' || symb == '-') {
                current_state = SIGN;
            } else if (symb == ' ') {
                current_state = DEGREE_SPACE;
            } else {
                current_state = INVALID;
                throw "После степени переменной может следовать только либо следующая переменная, либо следующий многочлен";
            }
            break;
        case DEGREE_SPACE:
            if (symb == ' ') {
                //
            } else if (isalpha(symb)&& islower(symb)) {
                current_state = ALPHA;
            } else if (symb == '+' || symb == '-') {
                current_state = SIGN;
            } else {
                current_state = INVALID;
                throw "После степени переменной может следовать только либо следующая переменная, либо следующий многочлен";
            }
            break;
    }
}

bool DFAPolynomial::CheckString(std::string& input) {
    current_state = START;
    for (int i = 0;i < input.size(); ++i) {
        char symb = input[i];
        try {
            CheckSymbol(symb);
        }catch(const char* error_string) {
            throw "Неправильный формат ввода полинома!Ошибка на " + std::to_string(i + 1)+ " символе:\n"+ error_string;
        }

    }
    return (current_state == DIGIT_FACT || current_state == DIGIT_SPACE ||
            current_state == ALPHA || current_state == ALPHA_SPACE ||
            current_state == DIGIT_DEG || current_state == DEGREE_SPACE || current_state == AFTER_DOT);
}