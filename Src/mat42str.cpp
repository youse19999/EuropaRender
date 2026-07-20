//
// Created by youse on 2026/07/20.
//
#include <Mat42Str.h>
std::stringstream Mat42Str(glm::mat4 mat)
{
    std::stringstream stream;
    stream << mat[0][0] << ","<< mat[0][1] << "," << mat[0][2] << ","  << mat[0][3] << "\n";
    stream << mat[1][0] << ","<< mat[1][1] << "," << mat[1][2] << ","  << mat[1][3] << "\n";
    stream << mat[2][0] << ","<< mat[2][1] << "," << mat[2][2] << ","  << mat[2][3] << "\n";
    stream << mat[3][0] << ","<< mat[3][1] << "," << mat[3][2] << ","  << mat[3][3] << "\n";
    return stream;
}