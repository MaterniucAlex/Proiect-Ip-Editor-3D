#ifndef CUSTOM_OBJ_READER
#define CUSTOM_OBJ_READER

typedef struct Material
{
  char *materialName;
  char *textureName;

} Material;

typedef struct ObjectFile 
{
  int verticesArraySize;
  float *verticesArray;

  int textureArraySize;
  float *textureArray;

  int normalsArraySize;
  float *normalsArray;

  int faceArraySize;
  unsigned int *faceArray;
  
  int indecesArraySize;
  unsigned int *indecesArray;

  Material material;

} ObjectFile;


int loadObject(char *address, ObjectFile *objToBeLoaded);
void freeObject(ObjectFile *obj);

#endif
