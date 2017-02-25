#ifndef readObj_hpp
#define readObj_hpp

class ObjFile{
    public:
        ObjFile(const char* name);
        float* get_vertices(void);
        float* get_normals(void);
        int* get_faceN(void);
        int* get_faceV(void);
        int get_number_of_faces(void);
    private:
        const char* File_Name;
        int number_of_normals, number_of_vertices, number_of_faces;
};

#endif