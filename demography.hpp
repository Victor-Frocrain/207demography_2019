/*
** EPITECH PROJECT, 2020
** demography
** File description:
** demography.hpp
*/

#ifndef DEMOGRAPHY_HPP_
#define DEMOGRAPHY_HPP_

#include <vector>
#include <map>

#define MY_ERROR 84
#define MY_SUCCESS 0

int demography(int ac, char **args);

class Datas {
    public:
    std::map<std::pair<std::string, std::string>, std::vector<double>> numbers;
};

#endif /* DEMOGRAPHY_HPP_ */
