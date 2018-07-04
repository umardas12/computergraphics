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
cout<<dibheader->noofbitsperpixel<<endl;
w=dibheader->bmpwidth;
h=dibheader->bmpheight;
pixelsize=w*h*3;
char* pixelbuff=new char[pixelsize];

myfile.seekp(0x36,ios::beg);
myfile.read(pixelbuff,pixelsize);
rgb=new RGBQUAD[w*h];
rgb=(RGBQUAD*)pixelbuff;

//Demosaic of image using average of neighbors

for(i=0;i<h;i++){
 for(j=0;j<w;j++)
{
    if (int(rgb[i*w+j].green)!=0){
        if(j-1<0){
             rgb[i*w+j].blue=rgb[(i*w)+j+1].blue;}
        else if(j+1>w-1){
             rgb[i*w+j].blue=rgb[(i*w)+j-1].blue;}
        else {
            
            rgb[i*w+j].blue=(char)(((int)rgb[i*w+j+1].red +(int)rgb[i*w+j-1].red)/2);}
         if(i+1>h-1){
                rgb[i*w+j].red=rgb[(i-1)*w+j].red;}
                else if(i-1<0){
                  rgb[i*w+j].red=rgb[(i+1)*w+j].red;}
                else
                 rgb[i*w+j].red=(char)(int)(((int)rgb[(i+1)*w+j].red +(int)rgb[(i-1)*w+j].red)/2);


        }


else if (int(rgb[i*w+j].red)!=0){
        if(j-1<0){
             if(i+1>h-1){

                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green)/2);
                rgb[i*w+j].blue=rgb[(i-1)*w+j+1].blue;
                }
                else if(i-1<0){

                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i+1)*w+j].green)/2);
                  rgb[i*w+j].blue=rgb[(i+1)*w+j+1].blue;
                }
                else{

                 rgb[i*w+j].blue=(char)(((int)rgb[(i+1)*w+j+1].blue +(int)rgb[(i-1)*w+j+1].blue)/2);
                 rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/3);}
        }

        else if(j+1>w-1){

            if(i+1>h-1){

                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i-1)*w+j].green)/2);
                rgb[i*w+j].blue=rgb[(i-1)*w+j-1].blue;
                }
                else if(i-1<0){

                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i+1)*w+j].green)/2);
                  rgb[i*w+j].blue=rgb[(i+1)*w+j-1].blue;
                }
                else{
                 rgb[i*w+j].blue=(char)(((int)rgb[(i-1)*w+j-1].blue +(int)rgb[(i+1)*w+j-1].blue)/2);
                 rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/3);}
        }

        else{

            if(i+1>h-1){
                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i*w)+j+1].green+(int)rgb[(i-1)*w+j].green)/3);
                rgb[i*w+j].blue=(char)(((int)rgb[(i-1)*w+j-1].blue+(int)rgb[(i-1)*w+j+1].blue)/2);
                }
                else if(i-1<0){
                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i*w)+j+1].green+(int)rgb[(i+1)*w+j].green)/3);
                  rgb[i*w+j].blue=(char)(((int)rgb[(i+1)*w+j-1].blue+(int)rgb[(i+1)*w+j+1].blue)/2);
                }
                else{
            rgb[i*w+j].blue=(char)(((int)rgb[(i-1)*w+j-1].blue+(int)rgb[(i-1)*w+j+1].blue +(int)rgb[(i+1)*w+j-1].blue+(int)rgb[(i+1)*w+j+1].blue)/4);
            rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green+(int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/4);}
            }
}


    else if (int(rgb[i*w+j].blue)!=0){
        if(j-1<0){
             if(i+1>h-1){
                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green)/2);
                rgb[i*w+j].red=rgb[(i-1)*w+j+1].red;
                }
                else if(i-1<0){
                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i+1)*w+j].green)/2);
                  rgb[i*w+j].red=rgb[(i+1)*w+j+1].red;
                }
                else{
                 rgb[i*w+j].red=(char)(((int)rgb[(i+1)*w+j+1].red +(int)rgb[(i-1)*w+j+1].red)/2);
                 rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/3);}
        }

        else if(j+1>w-1){
            if(i+1>h-1){
                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i-1)*w+j].green)/2);
                rgb[i*w+j].red=rgb[(i-1)*w+j-1].red;
                }
                else if(i-1<0){
                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i+1)*w+j].green)/2);
                  rgb[i*w+j].red=rgb[(i+1)*w+j-1].red;
                }
                else{
                 rgb[i*w+j].red=(char)(((int)rgb[(i-1)*w+j-1].red +(int)rgb[(i+1)*w+j-1].red)/2);
                 rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/3);}
        }

        else{
            if(i+1>h-1){
                rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i*w)+j+1].green+(int)rgb[(i-1)*w+j].green)/3);
                rgb[i*w+j].red=(char)(((int)rgb[(i-1)*w+j-1].red+(int)rgb[(i-1)*w+j+1].red)/2);
                }
                else if(i-1<0){
                  rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green +(int)rgb[(i*w)+j+1].green+(int)rgb[(i+1)*w+j].green)/3);
                  rgb[i*w+j].red=(char)(((int)rgb[(i+1)*w+j-1].red+(int)rgb[(i+1)*w+j+1].red)/2);
                }
                else{
            rgb[i*w+j].red=(char)(((int)rgb[(i-1)*w+j-1].red+(int)rgb[(i-1)*w+j+1].red +(int)rgb[(i+1)*w+j-1].red+(int)rgb[(i+1)*w+j+1].red)/4);
            rgb[i*w+j].green=(char)(((int)rgb[(i*w)+j-1].green+(int)rgb[(i*w)+j+1].green +(int)rgb[(i-1)*w+j].green+(int)rgb[(i+1)*w+j].green)/4);}
            }
    }
}};

pixelbuff=(char*)rgb;
myfile.close();

myfile.open("output_image.bmp",ios::binary|ios::out);
myfile.seekp(0x00,ios::beg);
myfile.write(chbuffer,sizeof(BITMAPFILEHEADER));
myfile.seekp(0x0E,ios::beg);
myfile.write(cdhbuffer,sizeof(BITMAPINFOHEADER));
myfile.seekp(0x36,ios::beg);
myfile.write(pixelbuff,pixelsize);

    myfile.close();

    free(pixelbuff);
    free(chbuffer);
    free(cdhbuffer);

    return 0;
}

