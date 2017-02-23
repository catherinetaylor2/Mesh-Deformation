#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include<string>


using namespace std;

int main(){
char str[2], c1, c2, c3, c4, c5, c6;
  float f1, f2, f3, f4, f5, f6, f7, f8, f9;
  string s = "a";
  FILE * myObject;
  int k_v = 0, k_vn=0, k_vf=0, t;

  myObject = fopen("pyramid.obj", "r");

//-----------------------------------------------------------------
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
 myObject = fopen("pyramid.obj", "r");
int number_of_vertices = k_v;
 float* V = new float[3*number_of_vertices];

 while (s != "v"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;  
 }

 k_v = 0;
for(int i=0; i<3*number_of_vertices; i+=3){
  V[i] = f1;
  V[i+1] = f2;
  V[i+2]=f3; 
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
}
 
//---------------------------------------------------------------------

while (s != "vn"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;  
 }
 cout<<"s "<<s<<"\n";

 do{  
   fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;
  k_vn=k_vn+1;
 }while (s == "vn");
cout<<"n "<<k_vn<<"\n";
fclose(myObject);
 myObject = fopen("pyramid.obj", "r");

int number_of_normals = k_vn;
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
 cout<<"nx "<<N[3*number_of_normals-3]<<" ";
  cout<<"ny "<<N[3*number_of_normals-2]<<" ";
  cout<<"nz "<<N[3*number_of_normals-1]<<" ";


//--------------------------------------------------------------------
fclose(myObject);
 myObject = fopen("pyramid.obj", "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;  
 }
 for(int i=0; i< number_of_normals-1; i++){
   fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
    cout<<"s "<<s<<"\n";
 }

do{
    s = str;
  k_vf=k_vf+1;
  cout<<"s "<<s<<"\n";
  t=fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
cout<<"t "<<t<<"\n";
cout<<"f9 "<<f9<<"\n";
}while(t!=EOF);
cout <<"k"<<k_vf<<"\n";

fclose(myObject);
 myObject = fopen("pyramid.obj", "r");
 fclose(myObject);
 myObject = fopen("pyramid.obj", "r");
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str; 
  while (s != "vn"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;  
 }
 for(int i=0; i< number_of_normals-1; i++){
   fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
    s = str;
    cout<<"s "<<s<<"\n";
 }

int number_of_faces = k_vf-1;
 float* FV = new float[3*number_of_faces];
  float* FN= new float[3*number_of_faces];

  for(int i=0; i<3*number_of_faces; i+=3){
fscanf(myObject, "%s %f %c %f %c %f %f %c %f %c %f %f %c %f %c %f", str, &f1, &c1, &f2, &c2, &f3, &f4, &c3, &f5, &c4, &f6, &f7, &c5, &f8, &c6, &f9);
FV[i] = f1;
FV[i+1]=f4;
FV[i+2]=f7;

FN[i]=f3;
FV[i+1] = f6;
FV[i+2]=f9;

  }
  cout<<"FV "<<FV[3*number_of_faces -3]<<" ";
   cout<<"FV "<<FV[3*number_of_faces -2]<<" ";
    cout<<"FV "<<FV[3*number_of_faces -1]<<"\n";

      cout<<"FN "<<FN[3*number_of_faces -3]<<" ";
   cout<<"FN "<<FN[3*number_of_faces -2]<<" ";
    cout<<"FN "<<FN[3*number_of_faces -1]<<"\n";
    return 0;
}