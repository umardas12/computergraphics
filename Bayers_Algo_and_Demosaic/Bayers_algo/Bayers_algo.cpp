#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef struct{
unsigned short signature;
unsigned int sizeofbmpfile;
unsigned short reserved1;
unsigned short reserver2;
unsigned int bmpstrtaddress ;
}BITMAPFILEHEADER;

typedef struct{
unsigned int sizeofdibheader;
int bmpwidth;
int bmpheight;
unsigned short noofcolorplanes;
unsigned short noofbitsperpixel;
unsigned int compressionmethod;
unsigned int bmpsize;
unsigned int pixelpmhorizontal;
unsigned int pixelpmvertical;
unsigned int noofcolorplattes;
unsigned int noofcolorused;
}BITMAPINFOHEADER;

typedef struct{
unsigned char blue;
unsigned char green;
unsigned char red;
//unsigned char padding;
}RGBQUAD;



int main()
{
     std::fstream myfile;

    int w,h,pixelsize,i,j;


    BITMAPFILEHEADER* bmpheader;
    BITMAPINFOHEADER* dibheader;
    RGBQUAD* rgb;
    char* chbuffer=new char[sizeof(BITMAPFILEHEADER)];
    char* cdhbuffer=new char[sizeof(BITMAPINFOHEADER)];

    myfile.open("input_image.bmp",ios::binary|ios::in);

    myfile.seekp(0,ios::beg);
    myfile.read(chbuffer,sizeof(BITMAPFILEHEADER));
    myfile.seekp(0x0E,ios::beg);
    myfile.read(cdhbuffer,sizeof(BITMAPINFOHEADER));
    bmpheader=(BITMAPFILEHEADER*)chbuffer;
    dibheader=(BITMAPINFOHEADER*)cdhbuffer;

w=dibheader->bmpwidth;
h=dibheader->bmpheight;
pixelsize=w*h*3;
char* pixelbuff=new char[pixelsize];

myfile.seekp(0x36,ios::beg); //skip the header and read the file
myfile.read(pixelbuff,pixelsize);
rgb=new RGBQUAD[w*h];
rgb=(RGBQUAD*)pixelbuff;

//Bayers Filter

for(i=0;i<h;i++){
for(j=0;j<w;j++)
{
    if(i%2==0&&j%2==0)
    {
        rgb[i*w+j].red=0;
        rgb[i*w+j].blue=0;
    }
    else if(i%2==0&&j%2==1)
    {       rgb[i*w+j].green=0;
        rgb[i*w+j].red=0;
    }
    else if(i%2==1&&j%2==0)
    {       rgb[i*w+j].blue=0;
        rgb[i*w+j].green=0;
    }
    else if(i%2==1&&j%2==1)
    {       rgb[i*w+j].blue=0;
        rgb[i*w+j].red=0;
    }
}};

pixelbuff=(char*)rgb;
myfile.close();

//write to output image
myfile.open("output_image.bmp",ios::binary|ios::out);
myfile.seekp(0x00,ios::beg);
myfile.write(chbuffer,sizeof(BITMAPFILEHEADER));
myfile.seekp(0x0E,ios::beg);
myfile.write(cdhbuffer,sizeof(BITMAPINFOHEADER));
myfile.seekp(0x36,ios::beg);
myfile.write(pixelbuff,pixelsize);

    myfile.close();
//release memory
    free(pixelbuff);
    free(chbuffer);
    free(cdhbuffer);

    return 0;
}
