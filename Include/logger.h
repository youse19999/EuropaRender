//
// Created by youse on 2026/07/20.
//
#include <sstream>

#ifndef EUROPARENDER_LOGGER_H
#define EUROPARENDER_LOGGER_H

extern std::stringstream LOGGER_STREAM;
extern std::stringstream LOGGER_RENDER;

#define LOG(expression) LOGGER_STREAM << expression << '\n'
#define LOG_RENDER(expression) LOGGER_RENDER << expression << '\n'



#endif //EUROPARENDER_LOGGER_Hf
