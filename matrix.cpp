#include <iostream>
#include <cmath>
#include "matrix.hpp"

void matrix_multiply(float a[][3], float b[][3], int r1, int c1, int r2, int c2, float M[][3]){
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
