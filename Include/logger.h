//
// Created by youse on 2026/07/20.
//

#ifndef EUROPARENDER_LOGGER_H
#define EUROPARENDER_LOGGER_H

//BYE BYE LOGGER!
#ifdef NOT DEBUG_MODE
    #define LOG(expression)
#else
    #define LOG(expression) std::cout << expression << std::endl
#endif

#endif //EUROPARENDER_LOGGER_H
