//#define _GLIBCXX_DEBUG 1
#pragma once

#include "list.h"
#include <list>
#include <vector>
#include <bitset>
#include <string>

class Polynomial;

class Monomial{
    friend Polynomial;
    friend std::ostream& operator << (std::ostream& os, const Monomial& this_mono);
public:
    Monomial();

    Monomial(long double factor_init, const std::vector<int>& degrees_init);

    Monomial(const Monomial& other);

    long double GetFactor();

    Monomial& operator=(const Monomial& other);


    Monomial operator*(const Monomial& other);

    bool operator <(const Monomial& other) const;
    bool operator >(const Monomial& other) const;
    bool operator >=(const Monomial& other) const;
    bool operator <=(const Monomial& other) const;
    bool operator==(const Monomial& other) const;
    bool operator!=(const Monomial& other) const;

    void SetFactor(const std::string& new_factor);
    void SetDegree(const std::string& new_degree, int var);

    void AddFactor(const std::string& add_factor);
    void AddDegree(const std::string& add_degree, int var);

    void Clear();

private:
    std::vector<int> degrees = std::vector(26, 0);
    long double factor = 0;
};

class Polynomial {
    friend std::ostream& operator << (std::ostream& os, const Polynomial& this_mono);
public:
    Polynomial();
    Polynomial(std::string);
    Polynomial(const Polynomial& other);
    void AddMono(const Monomial& other);

    void Stable();

    void UpdateMono() {}

    Polynomial& operator=(const Polynomial& other);

    Polynomial operator+(const Polynomial& other)const;

    Polynomial operator-() const;
    Polynomial operator-(const Polynomial& other)const;

    Polynomial& operator+=(const Polynomial& other);

    Polynomial operator*(const Monomial& other)const;

    Polynomial operator*(const Polynomial& other)const;

    std::pair<Polynomial, Polynomial> operator/(const Polynomial& other);

    Polynomial Derivative(int k, int var);//считаем к-ю производную


    bool operator==(const Polynomial& other) const;

    bool Availible(int x) const;
    int size() const;
    bool empty()const;

    long double Get(const std::vector<int>& params);
    std::vector<int> Roots();

private:
    List<Monomial> monomials;
    std::bitset<26> variables;
};


class DFAPolynomial {
    public:

    void CheckSymbol(char symb);

    bool CheckString(std::string& input);

    private:
    enum States{
        START,
        SIGN,
        DIGIT_FACT,
        AFTER_DOT,
        DOT,
        DIGIT_DEG,
        DEGREE_SPACE,
        ALPHA,
        DEGREE,
        ALPHA_SPACE,
        DIGIT_SPACE,
        INVALID
    };
    States current_state{States::START};
};