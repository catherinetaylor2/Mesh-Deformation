#ifndef matrix_hpp
#define matrix_hpp
#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

void matrix_multiply_8(float a[][8], float b[][4], int r1, int c1, int r2, int c2, float M[][4]);
void matrix_multiply_4(float a[][4], float b[][8], int r1, int c1, int r2, int c2, float M[][8]);
void matrix_transpose(float a[][4], int r, int c, float at[][8]);
void matrix_to_array(glm::mat4 matrix, float a[][4]);
void array_to_matrix(glm::mat4 matrix, float a[][4]);
void get_top_rows_8(float a[][8], float b[][8]);
void get_top_rows_6(float a[][6], float b[][8]);

#endif