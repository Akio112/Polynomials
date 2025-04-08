#include <iostream>
#include <vector>
#include <list>
#include<string>
#include <algorithm>
constexpr long double eps = 1e-11;

class Polynomial;

class Monomial{
    friend Polynomial;
public:
    Monomial() = default;
    Monomial(long double factor_init, const std::vector<int>& degrees_init):degrees(degrees_init),
    factor(factor_init){}

    Monomial(const Monomial& other):degrees(other.degrees), factor(other.factor) {}

    Monomial& operator=(const Monomial& other) {
        Monomial temp(other);
        std::swap(*this, temp);
        return *this;
    }

    ~Monomial() = default;

    Monomial operator*(const Monomial& other) {
        Monomial temp(*this);
        temp.factor *= other.factor;
        for (int i = 0;i < 26; ++i) {
            temp.degrees[i] += other.degrees[i];
        }
        return temp;
    }

    bool operator <(const Monomial& other) const{
        return (this->degrees<other.degrees); // сортируем по степеням всех переменных одночлена
    }
    bool operator >(const Monomial& other) const {
        return other<*this;
    }
    bool operator >=(const Monomial& other) const {
        return !(*this<other);
    }
    bool operator <=(const Monomial& other) const {
        return !(other<*this);
    }
    bool operator==(const Monomial& other) const {
        return (*this>=other && *this <= other);
    }
private:
    std::vector<int> degrees = std::vector(26, 0);
    long double factor = 0;
};

class Polynomial {

public:
    void AddMono(const Monomial& other) {
        if (monomials.empty()) {
            monomials.push_back(other);
            return;
        }
        if (monomials.back() ==(other)) {
            monomials.back().factor += other.factor;
        } else {
            monomials.push_back(other);
        }
    }

    void Stable() {
        monomials.sort();
        std::list<Monomial> temp = monomials;
        monomials.clear();
        for (auto mono_now:temp) {
            AddMono(mono_now);
        }
    }

    Polynomial& operator=(const Polynomial& other) {
        this->monomials = other.monomials;
        return *this;
    }

    Polynomial operator+(const Polynomial& other) {
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
        return temp;
    }

    Polynomial& operator+=(const Polynomial& other) {
        *this = (*this + other);
        return *this;
    }

    Polynomial operator*(const Monomial& other) {
        Polynomial temp;
        for (auto mono_now:monomials) {
            temp.AddMono(mono_now * other);
        }
        temp.Stable();
        return temp;
    }

    Polynomial operator*(const Polynomial& other) {
        Polynomial temp;
        for (Monomial mono_other:other.monomials) {
            temp += (*this) * mono_other;
        }
        return temp;
    }
private:
    std::list<Monomial> monomials;
};

int main() {
    return 0;
}
