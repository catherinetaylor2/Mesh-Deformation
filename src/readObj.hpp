
#ifndef readObj_hpp
#define readObj_hpp

#include <string>

class ObjFile{
    public:
        ObjFile(std::string name);
        void getVertices(float** vertices);
        void getNormals(float** normals);
        void getTextures(float** textures);
        void getFaceData(int** faceVertices, int** faceNormals, int** faceTextures);
        static void cleanUp(float* vertices, float* normals, float* textures,int* faceVertices, int* faceNormals, int* faceTextures);
        int getNumberOfFaces(void){
            return NumberOfFaces;
        }
        int getNumberOfVertices(void){
            return NumberOfVertices;
        }
        std::string getFileName(void){
            return fn;
        }
        bool doesExist(void){
            return exist;
        }
        void getMeshData(ObjFile mesh, int** faceVertices, int** faceNormals, int** faceTextures, //face data
                        float** textures, float** normals, float** vertices, //vertex data
                        int* NumberOfFaces, int* NumberOfVertices); //number data
    private:
		std::string fn;
        int NumberOfNormals = 0;
        int NumberOfVertices = 0;
        int NumberOfFaces = -1;
        bool exist;
};
#endif