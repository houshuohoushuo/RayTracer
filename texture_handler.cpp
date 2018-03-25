#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>
#include <memory.h>

int texture_width, texture_height;
GLubyte *wood;

int env_width, env_height;
GLubyte *env_texture;
using namespace std;

// Inspired by https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
// -A simple libpng example
// This c++ code snippet is an example of loading a png image file into an opengl 
// texture object. It requires libpng and opengl to work. To compile with gcc, 
// link png glu32 and opengl32 . Most of this is taken right out of the libpng manual.
// There is no checking or conversion of the png format to the opengl texture format. 
// This just gives the basic idea
int loadTexture(char *name, int &outWidth, int &outHeight, GLubyte **outData) {
    int interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return -1;
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return -1;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, (png_infopp) NULL);
        return -1;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
      fclose(fp);
      return -1;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 0);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
    // png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth,color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    outWidth = width;
    outHeight = height;

    // png_read_update_info(png_ptr, info_ptr);
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) {
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return 1;
}

void ini_texture_handler()
{	
    char texture[]="starnight.png";
    loadTexture(texture, texture_width, texture_height,&wood);
    char env_image[]="torontodowntown.png";
    loadTexture(env_image, env_width, env_height,&env_texture);
}
