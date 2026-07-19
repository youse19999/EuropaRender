//
// Created by youse on 2026/07/20.
//

#ifndef EUROPARENDER_MAT42STR_H
#define EUROPARENDER_MAT42STR_H
#include <sstream>
#include <string>
#include <glm/glm.hpp>

std::stringstream Mat42Str(glm::mat4 mat) {
    std::stringstream stream;
    stream << mat[0][0] << ","<< mat[0][1] << "," << mat[0][3] << "\n";
    stream << mat[1][0] << ","<< mat[1][1] << "," << mat[1][3] << "\n";
    stream << mat[2][0] << ","<< mat[2][1] << "," << mat[2][3] << "\n";
    stream << mat[3][0] << ","<< mat[3][1] << "," << mat[3][3] << "\n";
    return stream;
}
#endif //EUROPARENDER_MAT42STR_H
