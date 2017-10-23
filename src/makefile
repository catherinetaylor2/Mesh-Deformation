IDIR = /usr/include
CC = clang++ -std=c++11
CFLAGS = -I$(IDIR)

ODIR = obj

LIBS = -lGLEW -lglfw -lGL



_OBJ = Mesh_Deformation.o readObj.o shader.o 
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.cpp ;
	$(CC) -c -o $@ $< $(CFLAGS)


MeshDeformation: $(OBJ) ;
	clang++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 