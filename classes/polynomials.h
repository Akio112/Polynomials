#pragma once
#include <vector>
#include <list>
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

    void AddMono(const Monomial& other);

    void Stable();

    void UpdateMono() {}

    Polynomial& operator=(const Polynomial& other);

    Polynomial operator+(const Polynomial& other);

    Polynomial& operator+=(const Polynomial& other);

    Polynomial operator*(const Monomial& other);

    Polynomial operator*(const Polynomial& other);

    bool operator==(const Polynomial& other) const;

    long double Get();
private:
    std::list<Monomial> monomials;
    std::bitset<26> variables;
};