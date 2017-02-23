#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include<string>


using namespace std;

int main(){
char str[2], c1, c2, c3, c4, c5, c6, c7, c8;
  float f1, f2, f3, f4, f5, f6, f7, f8;
  string s = "a";
  FILE * myObject;
  int k_v = 0, k_vn=0, k_vf=0;
  //  ifstream myObject ("sphere.obj");

  myObject = fopen("cube.obj", "r");

for (int i=0; i<4; i++){
fscanf(myObject, "%*[^\n]\n", NULL);
}
while (s == "v"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;
  k_v=k_v+1;
 }

fclose(myObject);
 myObject = fopen("cube.obj", "r");
int number_of_vertices = 8;
 float* V = new float[3*number_of_vertices];
for (int i=0; i<4; i++){
fscanf(myObject, "%*[^\n]\n", NULL);
}
for(int i=0; i<3*number_of_vertices; i+=3){
     fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
   cout <<"i "<<i<<"\n";
     V[i] = f1;
     V[i+1] = f2;
     V[i+2]=f3;    
}

fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
s=str;
while (s == "vn"){
  fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
  s = str;
  k_vn=k_vn+1;
 }

fclose(myObject);
 myObject = fopen("cube.obj", "r");

int number_of_normals = k_vn;
 float* N = new float[3*number_of_normals];
for (int i=0; i<12; i++){
    fscanf(myObject, "%*[^\n]\n", NULL);
    }
for(int i=0; i<3*number_of_normals; i+=3){
     fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
     N[i] = f1;
     N[i+1] = f2;
     N[i+2]=f3;    
}

for (int i=0; i<4;i++){
 fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
}
s=str;
// while(s!="f"){
//     fscanf(myObject, "%s %f %f %f" , str, &f1, &f2, &f3);
//     s=str;
 cout<<"s "<<s<<"\n";
// }

fscanf(myObject, "%s %f %c %c %f %f %c %c %f %f %c %c %f %f %c %c %f" , str, &f1, &c1, &c2, &f2, &f3, &c3,&c4, &f4,&f5, &c5, &c6, &f6, &f7, &c7, &c8, &f8);
  s = str;
  cout <<"str "<<str<<"f1 "<<f1<<"f3 "<<f3<<"f5 "<<f5<<"\n";
  cout<<"s "<<s<<"\n";
while (s.c_str()!=EOF){
fscanf(myObject, "%s %f %c %c %f %f %c %c %f %f %c %c %f %f %c %c %f" , str, &f1, &c1, &c2, &f2, &f3, &c3,&c4, &f4,&f5, &c5, &c6, &f6, &f7, &c7, &c8, &f8);
  s = str;
  k_vf=k_vf+1;
cout <<"str "<<str<<"f1 "<<f1<<"f3 "<<f3<<"f5 "<<f5<<"\n";
}
cout<<"k "<<k_vf<<"\n";
    return 0;
}