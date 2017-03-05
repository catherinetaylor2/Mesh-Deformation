#include <iostream>
#include <cmath>
#include "matrix.hpp" 
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


void matrix_multiply_8(float a[][8], float b[][4], int r1, int c1, int r2, int c2, float M[][4]){
    if(c1!=r2){
        std::cout<<"These matrices cannot be multiplied \n";
        return;
    }
    for (int i=0; i<r1; i++){
        for (int j=0; j<c2;j++){
            M[i][j]=0;
        }
    }
    for(int i = 0; i < r1; ++i){
        for(int j = 0; j < c2; ++j){
            for(int k = 0; k < c1; ++k){
                M[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_multiply_4(float a[][4], float b[][8], int r1, int c1, int r2, int c2, float M[][8]){
    if(c1!=r2){
        std::cout<<"These matrices cannot be multiplied \n";
        return;
    }
    for (int i=0; i<r1; i++){
        for (int j=0; j<c2;j++){
            M[i][j]=0;
        }
    }
    for(int i = 0; i < r1; ++i){
        for(int j = 0; j < c2; ++j){
            for(int k = 0; k < c1; ++k){
                M[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void matrix_transpose(float a[][4], int r, int c, float at[][8]){
    for (int i=0; i<c; i++){
        for (int j=0; j<r;j++){
            at[i][j]=a[j][i];
        }
    }
}

void array_to_matrix(glm::mat4 matrix, float a[][4]){
    for (int x = 0; x< 4;x++){
        for (int y=0; y<4;y++){
            matrix[y][x] = a[x][y];
        }
    }
}

void matrix_to_array(glm::mat4 matrix, float a[][4]){
    for (int x = 0; x< 4;x++){
        for (int y=0; y<4;y++){
            a[x][y] = matrix[y][x];
        }
    }
}
void get_top_rows_8(float a[][8], float b[][8]){
    for (int j=0; j<2; j++){
        for(int i=0; i<8; i++){
            b[j][i]=a[j][i];
        }
    }
}
void get_top_rows_6(float a[][6], float b[][6]){
    for (int j=0; j<2; j++){
        for(int i=0; i<6; i++){
            b[j][i]=a[j][i];
        }
    }
}