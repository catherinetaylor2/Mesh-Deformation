#ifndef readObj_hpp
#define readObj_hpp


class ObjFile{
    public:
        ObjFile(const char* name);
        float* get_vertices(void);
    private:
        const char* File_Name;

};

#endif