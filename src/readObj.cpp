#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include "readObj.hpp"

#define infinity FLT_MAX

ObjFile::ObjFile(std::string name){ //constructor
	fn = name;
  if ((fopen(fn.c_str(), "r")) == nullptr){ //check file exists
    exist = false;
  }
  else{
    exist = true;
  }
}

void ObjFile::getVertices(float** vertices){ //find the vertices and store in array
  char str[1024];
  float f;
  FILE *myObject  = fopen(fn.c_str(), "r"); //opens object file
  int EmptyLines =0;

  while (std::string(str) != "v"){ //scans file for specific arrangement of floats.
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
    EmptyLines++;
  }
  do{  
    fscanf(myObject, "%s%f%f%f" , str, &f, &f, &f);
    NumberOfVertices ++;
  }while (std::string(str) != "vt");

  *vertices= new float[3*NumberOfVertices];
  rewind(myObject); //go back to start

  for(int i=0; i<EmptyLines-1; i++)fscanf(myObject, "%s%f%f%f" , str, &f, &f, &f);
  for(int i=0; i<3*NumberOfVertices; i+=3){ //save values in array.
    fscanf(myObject, "%s%f%f%f", str, &(*vertices)[i], &(*vertices)[i+1], &(*vertices)[i+2]);
  }
  fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  fclose(myObject);
}

void ObjFile::getNormals(float** normals){ //as above with normals.
  char str[1000];
  float f;
  FILE * myObject = fopen(fn.c_str(), "r");
  int EmptyLines = 0;

  while (std::string(str) != "vn"){
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
    EmptyLines ++;
  }
  do{  
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
    NumberOfNormals ++;
  }while (std::string(str) == "vn");

  rewind(myObject);
  *normals = new float[3*NumberOfNormals];
  for(int i =0; i<EmptyLines-1; ++i)fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  for(int i=0; i<3*NumberOfNormals; i+=3){
       fscanf(myObject, "%s%f%f%f", str, &(*normals)[i], &(*normals)[i+1], &(*normals)[i+2]);
  }
  fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  fclose(myObject);
}

void ObjFile::getTextures(float ** textures){ //get texture values.
  char str[1000];
  float f;
  FILE * myObject;
  myObject = fopen(fn.c_str(), "r");

  *textures = new float[2*NumberOfVertices];
  while (std::string(str) != "v"){
     fscanf(myObject, "%s%f%f%f", str, &f, &f, &f); 
  }
  for(int i=0; i< NumberOfVertices-1; i++){
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  }
  for(int i=0; i<2*NumberOfVertices; i+=2){
    fscanf(myObject, "%s%f%f", str, &(*textures)[i], &(*textures)[i+1]);
  }
  fclose(myObject);
}

void ObjFile::getFaceData(int** faceVertices, int** faceNormals, int** faceTextures){ //find face vertices, normals and texture.
  char str[1000];
  float f;
  int i, t;
  FILE * myObject;
  myObject = fopen(fn.c_str(), "r");
  fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  while (std::string(str) != "vn"){
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  }
  for(int i=0; i< NumberOfNormals-1; i++){
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  }
  do{    
    NumberOfFaces++;
    t = fscanf(myObject, "%s%i/%i/%i%i/%i/%i%i/%i/%i", str, &i,  &i, &i, &i, &i, &i, &i, &i, &i);
  }while(t!=EOF);
  rewind(myObject);
  fscanf(myObject, "%s%f%f%f", str, &f, &f, &f);
  while (std::string(str) != "vn"){
    fscanf(myObject, "%s%f%f%f", str, &f, &f, &f); 
  }
  for(int i=0; i< NumberOfNormals-1; i++){
    fscanf(myObject, "%s%f%f%f" , str, &f, &f, &f);
  }
  *faceVertices = new int[3*NumberOfFaces], *faceNormals = new int[3*NumberOfFaces], *faceTextures = new int[3*NumberOfFaces];
  for(int i=0; i<3*NumberOfFaces; i+=3){//store in arrays of integers
    fscanf(myObject,"%s%i/%i/%i%i/%i/%i%i/%i/%i",str,&(*faceVertices)[i],&(*faceTextures)[i],&(*faceNormals)[i],&(*faceVertices)[i+1],
          &(*faceTextures)[i+1],&(*faceNormals)[i+1],&(*faceVertices)[i+2],&(*faceTextures)[i+2],&(*faceNormals)[i+2]);
  }
  for(int i = 0; i<3*NumberOfFaces; i++){
  // (*faceNormals)[i]+=-1, (*faceVertices)[i]+=-1, (*faceTextures)[i]+=-1;
  }
  fclose(myObject);
}

void ObjFile::getMeshData(ObjFile mesh,int** faceVertices, int** faceNormals, int** faceTextures, 
                          float** textures, float** normals, float** vertices, 
                          int* NumberOfFaces, int* NumberOfVertices){
		mesh.getVertices(vertices);
		mesh.getTextures(textures);
		mesh.getNormals(normals);
		mesh.getFaceData(faceVertices, faceNormals, faceTextures);
    *NumberOfVertices = mesh.getNumberOfVertices();
	  *NumberOfFaces = mesh.getNumberOfFaces();
}
void ObjFile::cleanUp(float*vertices, float* normals, float* textures,int* faceVertices, int* faceNormals, int* faceTextures){ //deletes pointers which are out of scope
  delete[] vertices;
  delete[] normals;
  delete[] textures;
  delete[] faceVertices;
  delete[] faceNormals;
  delete[] faceTextures;
}