/*
** EPITECH PROJECT, 2020
** demography
** File description:
** demography.cpp
*/

#include "demography.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <time.h>

void dispHelp()
{
    std::ifstream fs("help");

    if (!fs.fail()) {
        std::cout << fs.rdbuf() << std::endl;
    }
}

bool parseNumber(const std::string &number, double &result)
{
    for (size_t i = 0; i < number.size(); i++) {
        if (number[i] < '0' || number[i] > '9') {
            return false;
        }
    }
    if (number.empty()) {
        return false;
    }
    result = std::stod(number);
    return true;
}

bool copyLine(size_t &i, const std::string &line, std::vector<double> &datas)
{
    std::string number = "";
    double nb = 0;

    for (; i < line.length(); i++) {
        if (line[i] != ';') {
            number += line[i];
        }
        else {
            if (parseNumber(number, nb)) {
                datas.push_back(nb);
                number = "";
            }
            else {
                return false;
            }
        }
    }
    if (parseNumber(number, nb)) {
        datas.push_back(nb);
    }
    else {
        return false;
    }
    return true;
}

bool parseLine(const std::string &line, const std::vector<std::string> &codes, Datas &datas)
{
    std::string country = "";
    std::string code = "";
    size_t i = 0;

    for (size_t id = 0; i < line.length() && id < 2; i++) {
        if (line[i] != ';') {
            if (id == 0) {
                country += line[i];
            }
            else if (id == 1) {
                code += line[i];
            }
        }
        else {
            id++;
        }
    }
    for (size_t j = 0; j < codes.size(); j++) {
        if (codes[j] == code) {
            std::pair<std::string, std::string> pair = {code, country};
            std::vector<double> numbers;
            if (copyLine(i, line, numbers)) {
                datas.numbers[pair] = numbers;
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool parseCountries(const int &ac, char **&av, Datas &datas)
{
    std::ifstream fs("207demography_data.csv");
    std::string line = "";
    std::vector<std::string> codes;
    int nbEntries = 0;

    for (int i = 1; i < ac; i++) {
        codes.push_back(av[i]);
    }
    if (!fs.fail()) {
        for (std::getline(fs, line); line != ""; std::getline(fs, line)) {
            if (parseLine(line, codes, datas)) {
                nbEntries++;
            }
            if (nbEntries + 1 == ac) {
                return true;
            }
        }
    }
    else {
        return false;
    }
    return false;
}

void firstRootMean(Datas &datas, double &result, const double &a, const double &b)
{
    double diff = 0;
    double n = 2017 - 1960 + 1;
    int year = 1960;

    result = 0;
    std::vector<double> combine;
    for (size_t i = 0; i < n; i++) {
        combine.push_back(0);
    }
    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it = datas.numbers.begin();
    for (; it != datas.numbers.end(); it++) {
        std::vector<double> values = it->second;
        for (size_t i = 0; i < values.size() && i < combine.size(); i++) {
            combine[i] += values[i];
        }
    }
    for (size_t i = 0; i < combine.size(); i++, year++) {
        diff += std::pow(combine[i] - (a * year + b), 2);
    }
    result = std::sqrt(diff / n);
}

void secondRootMean(Datas &datas, double &result, const double &a, const double &b)
{
    double diff = 0;
    double n = 2017 - 1960 + 1;
    int year = 1960;

    result = 0;
    std::vector<double> combine;
    for (size_t i = 0; i < n; i++) {
        combine.push_back(0);
    }
    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it = datas.numbers.begin();
    for (; it != datas.numbers.end(); it++) {
        std::vector<double> values = it->second;
        for (size_t i = 0; i < values.size() && i < combine.size(); i++) {
            combine[i] += values[i];
        }
    }
    for (size_t i = 0; i < combine.size(); i++, year++) {
        diff += std::pow(combine[i] - ((-(b) + year) / a), 2);
    }
    result = std::sqrt(diff / n);
}

bool computeFirstXY(Datas &datas, double &a, double &b)
{
    double year = 1960;
    double sumX = 0;
    double sumY = 0;
    double sumXY = 0;
    double sumXX = 0;
    double n = 2017 - 1960 + 1;

    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it = datas.numbers.begin();
    for (; it != datas.numbers.end(); it++) {
        std::vector<double> values = it->second;
        if (n != values.size()) {
            return false;
        }
        year = 1960;
        sumX = 0;
        sumY = 0;
        sumXY = 0;
        sumXX = 0;
        for (size_t i = 0; i < values.size(); i++, year++) {
            sumX += year;
            sumY += values[i];
            sumXX += std::pow(year, 2);
            sumXY += year * values[i];
        }
        double denom = n * sumXX - std::pow(sumX, 2);
        if (denom == 0) {
            return false;
        }
        else {
            a += (n * sumXY - sumX * sumY) / denom;
            b += (sumY * sumXX - sumX * sumXY) / denom;
        }
    }
    return true;
}

bool computeSecondXY(Datas &datas, double &a, double &b)
{
    double year = 1960;
    double sumX = 0;
    double sumY = 0;
    double sumXY = 0;
    double sumXX = 0;
    double n = 2017 - 1960 + 1;

    a = 0,
    b = 0;
    std::vector<double> combine;
    for (size_t i = 0; i < n; i++) {
        combine.push_back(0);
    }
    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it = datas.numbers.begin();
    for (; it != datas.numbers.end(); it++) {
        std::vector<double> values = it->second;
        for (size_t i = 0; i < values.size() && i < combine.size(); i++) {
            combine[i] += values[i];
        }
    }
    if (n != combine.size()) {
            return false;
    }
    for (size_t i = 0; i < n; i++, year++) {
        sumX += combine[i];
        sumY += year;
        sumXX += std::pow(combine[i], 2);
        sumXY += year * combine[i];
    }
    double denom = n * sumXX - std::pow(sumX, 2);
    if (denom == 0) {
        return false;
    }
    else {
        a = (n * sumXY - sumX * sumY) / denom;
        b = (sumY * sumXX - sumX * sumXY) / denom;
    }
    return true;
}

void computeCorrelation(Datas &datas, double &result, const double &a1, const double &b1, const double &a2, const double &b2, const double &d1, const double &d2)
{
    double diff = 0;
    double n = 2017 - 1960 + 1;
    int year = 1960;

    result = 0;
    std::vector<double> combine;
    for (size_t i = 0; i < n; i++) {
        combine.push_back(0);
    }
    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it = datas.numbers.begin();
    for (; it != datas.numbers.end(); it++) {
        std::vector<double> values = it->second;
        for (size_t i = 0; i < values.size() && i < combine.size(); i++) {
            combine[i] += values[i];
        }
    }
    for (size_t i = 0; i < combine.size(); i++, year++) {
        diff += (combine[i] - (a1 * year + b1)) * (combine[i] - ((-(b2) + year) / a2));
    }
    diff /= n;
    result = diff / (d1 * d2);
}

bool treatmentDatas(Datas &datas)
{
    std::map<std::pair<std::string, std::string>, std::vector<double>>::iterator it;
    size_t nb = 0;
    double x1 = 0;
    double y1 = 0;
    double x2 = 0;
    double y2 = 0;
    double firstRoot = 0;
    double secondRoot = 0;
    double correlation = 0;
    std::string sign = "";

    std::cout << std::setprecision(2) << std::fixed;
    if (!computeFirstXY(datas, y1, x1)) {
        return false;
    }
    firstRootMean(datas, firstRoot, y1, x1);
    if (y1 < 0) {
        y1 *= -1.0;
    }
    std::cout << "Country: ";
    for (it = datas.numbers.begin(); it != datas.numbers.end(); it++, nb++) {
        std::cout << it->first.second;
        if (nb + 1 < datas.numbers.size()) {
            std::cout << ", ";
        }
        else {
            std::cout << std::endl;
        }
    }
    std::cout << "Fit1" << std::endl;
    if (x1 >= 0) {
        sign = "+";
    }
    else {
        sign = "-";
    }
    std::cout << "    Y = " << y1 / 1000000 << " X " + sign + " " << std::abs(x1) / 1000000 << std::endl;
    std::cout << "    Root-mean-square deviation: " << firstRoot / 1000000 << std::endl;
    std::cout << "    Population in 2050: " << (y1 / 1000000) * 2050 + (x1 / 1000000) << std::endl;


    std::cout << "Fit2" << std::endl;
    if (!computeSecondXY(datas, y2, x2)) {
        return false;
    }
    secondRootMean(datas, secondRoot, y2, x2);
    if (y2 < 0) {
        y2 *= -1.0;
    }
    if (x2 >= 0) {
        sign = "+";
    }
    else {
        sign = "-";
    }
    std::cout << "    X = " << y2 * 1000000 << " Y " + sign + " " << x2 << std::endl;
    std::cout << "    Root-mean-square deviation: " << secondRoot / 1000000 << std::endl;
    std::cout << "    Population in 2050: " << (((-(x2) + 2050) / y2) / 1000000) << std::endl;

    computeCorrelation(datas, correlation, y1, x1, y2, x2, firstRoot, secondRoot);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Correlation: " << correlation << std::endl;
    return true;
}

int demography(int ac, char **args)
{
    int result = MY_SUCCESS;
    Datas datas;

    if (ac >= 2) {
        std::string arg(args[1]);
        if (ac == 2 && arg == "-h") {
            dispHelp();
        }
        else {
            if (!parseCountries(ac, args, datas) || !treatmentDatas(datas)) {
                return MY_ERROR;
            }
        }
    }
    else
        result = MY_ERROR;
    return result;
}