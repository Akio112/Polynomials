//#define _GLIBCXX_DEBUG 1
#include "classes/polynomials.h"
#include "classes/list.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>

void save(const List<Polynomial>& poly_list) {
    std::ofstream save_out("save.txt");
    for (Polynomial poly_now:poly_list) {
        save_out << poly_now << '\n';
    }
    std::cout << "Полиномы успешно экспортированы" << std::endl;
}

void load(List<Polynomial>& poly_list, const std::string& path_load) {
    std::ifstream load_in(path_load);
    if (!load_in.is_open()) {
        throw "Не удалось открыть файл";
    }
    DFAPolynomial checker;
    List<Polynomial> new_list;
    std::string inp_str;
    while (std::getline(load_in, inp_str)) {
        if (checker.CheckString(inp_str)) {
            new_list.push_back(Polynomial(inp_str));
        } else {
            throw "Входные данные из файла некорректны";
        }
    }
    poly_list.clear();
    poly_list = new_list;
    std::cout << "Полиномы успешно импортировались" << std::endl;
}

void Help() {
    std::cout << "check - Посмотреть, какие полиномы лежат в БД\n";
    std::cout << "load - эспортировать полиномы из файлика(они должны быть каждый на своей строке в формате ввода)\n";
    std::cout << "save - сохранить полиномы в текстовый файлик\n";
    std::cout << "get - получить значение от полинома в точке\n";
    std::cout << "div - поделить один полином на другой\n";
    std::cout << "addlist - Добавить полином:\n";
    std::cout << "multipl - Перемножить полиномы:\n";
    std::cout << "add - сложить 2 полинома\n";
    std::cout << "derative - взять производную от полинома\n";
    std::cout << "end - завершить программу\n";
    std::cout << "help - снова написать все команды в консоль\n";
    std::cout << "del - удалить полином" << std::endl;
}

void PrintPolynoms(List<Polynomial>& polynoms_print) {
    int cnt = 0;
    for (Polynomial now_poly:polynoms_print) {
        std::cout << cnt << ". " << now_poly << '\n';
        cnt++;
    }
    std::flush(std::cout);
}

std::pair<int, int> GetTwoPolynoms() {
    std::cout << "Введите номера полиномов, с которыми вы хотите произвести функцию" << std::endl;
    std::string first_ind_str, second_ind_str;
    std::cin >> first_ind_str >> second_ind_str;
    return {std::stoll(first_ind_str), std::stoll(second_ind_str)};
}

int GetOnePolynom() {
    std::cout << "Введите номер полинома, с которым хотите произвести функцию" << std::endl;
    std::string ind_poly_str;
    std::cin >> ind_poly_str;
    return std::stoll(ind_poly_str);
}

void SaveQuery(List<Polynomial>& DB, const Polynomial& res) {
    if (res.empty()) return;
    std::cout << "Хотите сохранить полином " << res << "? y/n"<<  std::endl;
    std::string ans;
    std::cin >> ans;
    if (ans == "y") {
        DB.push_back(res);
    }
}


void CommandHandler(DFAPolynomial& checker_poly,List<Polynomial>& DB,std::string& inp_command) {
    try {
        if (inp_command == "check") {
            PrintPolynoms(DB);
        } else if (inp_command == "help"){
            Help();
        } else if (inp_command == "addlist") {
            std::cout << "Введите полином, который хотите добавит: ";
            std::string new_polynom;
            std::cin.ignore();
            std::getline(std::cin, new_polynom);
            if (checker_poly.CheckString(new_polynom)) {
                DB.push_back(Polynomial(new_polynom));
            } else {
                throw "Неправильный формат ввода полинома!";
            }
        } else if (inp_command == "load") {
            std::cout << "Введите файл, откуда хотите загрузить полином" << std::endl;
            std::string path;
            std::cin >> path;
            load(DB, path);
        } else if(inp_command == "save") {
            save(DB);
            std::cout << "Полиномы сохранены в save.txt" << std::endl;
        } else if (inp_command == "get") {
            int ind_poly = GetOnePolynom();
            long double res =  DB[ind_poly].Get();
            std::cout << "Вот ваше значение:" << res << std::endl;
        } else if (inp_command == "add") {
            auto [first_ind,second_ind] = GetTwoPolynoms();
            Polynomial res = DB[first_ind] + DB[second_ind];
            std::cout << "Вот ваше значение:" << res << std::endl;
            SaveQuery(DB, res);
        } else if (inp_command == "multipl") {
            auto [first_ind,second_ind] = GetTwoPolynoms();
            Polynomial res = DB[first_ind] * DB[second_ind];
            SaveQuery(DB, res);
        } else if (inp_command == "div") {
            auto [first_ind,second_ind] = GetTwoPolynoms();
            auto [res, rem] = DB[first_ind] / DB[second_ind];
            SaveQuery(DB, res);
            SaveQuery(DB, rem);

        } else if (inp_command == "derative") {
            int ind_poly = GetOnePolynom();
            std::string k_str, var_str;
            std::cin >> k_str >> var_str;
            if (var_str.size() != 1) {
                throw "Нельзя производную не от одной переменной!";
            }
            int k = std::stoll(k_str), var = var_str[0];
            Polynomial res = DB[ind_poly].Derivative(k, var);
            SaveQuery(DB,res);
        } else if (inp_command == "end") {
            std::cout << "Хорошего тебе дня пользователь!:)";
            exit(0);
        } else if (inp_command == "save") {
            save(DB);
            std::cout << "База успешно сохранена в save.txt!" << std::endl;
        } else if (inp_command == "load") {
            std::string path;
            std::cin >> path;
            load(DB, path);
        } else if (inp_command == "del") {
            std::cout << "Введите номер полинома, который вы хотите удалить" << std::endl;
            std::string ind_str;
            std::cin >> ind_str;
            int ind = std::stoll(ind_str);
            DB.erase(ind);
            std::cout << "Полином успешно удален!" << std::endl;
        }else {
            throw "Кажется, вы ввели неверную команду";
        }
    }
    catch(const char* error_string) {
        std::cout << error_string << std::endl;
    }
    catch(const std::invalid_argument& e) {
        std::cout << "Кажется, что-то пошло не так, возможно вы ошиблись с вводом" << std::endl;
    }
}


int main() {
    List<Polynomial> DB;
    DFAPolynomial checker_poly;
    Help();
    while (true) {
        std::cout << "Напишите команду, которую вы хотите совершить:";
        std::string inp_command;
        std::cin >> inp_command;
        CommandHandler(checker_poly, DB, inp_command);
    }
}