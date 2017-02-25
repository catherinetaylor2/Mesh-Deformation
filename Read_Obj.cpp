#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include<string>
#include"readObj.hpp"


ObjFile::ObjFile(const char* name){
  File_Name = name;
}

 float* ObjFile::get_vertices(void){
  char str[2];
  float f1, f2, f3;
  std::string s = "a";
  FILE * myObject;
  int k_v = 0;
  myObject = fopen(File_Name, "r");

  while (s != "v"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }
  do{  
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
    k_v=k_v+1;
  }while (s != "vt");

  fclose(myObject);
  myObject = fopen(File_Name, "r");
  number_of_vertices = k_v;
  float* V = new float[3*number_of_vertices];

  while (s != "v"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }
  for(int i=0; i<3*number_of_vertices; i+=3){
    V[i] = f1;
    V[i+1] = f2;
    V[i+2]=f3; 
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  }
  return V;
 }

float* ObjFile::get_normals(void){
  char str[2];
  float f1, f2, f3;
  std::string s = "a";
  FILE * myObject;
  int k_vn=0;
  myObject = fopen(File_Name, "r");

  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }

  do{  
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
    k_vn=k_vn+1;
  }while (s == "vn");

  fclose(myObject);
  myObject = fopen(File_Name, "r");
  number_of_normals = k_vn;
  float* N = new float[3*number_of_normals];

  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }

  for(int i=0; i<3*number_of_normals; i+=3){
    N[i] = f1;
    N[i+1] = f2;
    N[i+2]=f3;   
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  }

  return N;
}

int* ObjFile::get_faceV(void){
  char str[2], c1, c2, c3, c4, c5, c6;
  float f1, f2, f3, f4, f5, f6, f7, f8, f9;
  std::string s = "a";
  FILE * myObject;
  int k_vf=0, t;

  myObject = fopen(File_Name, "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }
  for(int i=0; i< number_of_normals-1; i++){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
  }
  do{    
    s = str;
    k_vf=k_vf+1;
    t=fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
  }while(t!=EOF);

 
  fclose(myObject);
  myObject = fopen(File_Name, "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }

  for(int i=0; i< number_of_normals-1; i++){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
  }

  int number_of_faces = k_vf-1;
  int* FV = new int[3*number_of_faces];
  for(int i=0; i<3*number_of_faces; i+=3){
    fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
    s=str;
    FV[i] = f1;
    FV[i+1]=f4;
    FV[i+2]=f7;
  }
  return FV;
}

int* ObjFile::get_faceN(void){
  char str[2], c1, c2, c3, c4, c5, c6;
  float f1, f2, f3, f4, f5, f6, f7, f8, f9;
  std::string s = "a";
  FILE * myObject;
  int k_vf=0, t;

  myObject = fopen(File_Name, "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }
  for(int i=0; i< number_of_normals-1; i++){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
  }
  do{    
    s = str;
    k_vf=k_vf+1;
    t=fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
  }while(t!=EOF);
 
  fclose(myObject);
  myObject = fopen(File_Name, "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;  
  }
  for(int i=0; i< number_of_normals-1; i++){
    fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
  }
  number_of_faces = k_vf-1;
  int* FN = new int[3*number_of_faces];

  for(int i=0; i<3*number_of_faces; i+=3){
    fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
    s=str;
    FN[i] = f3;
    FN[i+1]=f6;
    FN[i+2]=f9;
  }
  return FN;
}
