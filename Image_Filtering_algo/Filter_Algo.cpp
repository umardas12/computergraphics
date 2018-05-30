
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define Pi 3.14159
#define exp 2.71828


using namespace std;

int radius=6;


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

void gaussian_filter(int r,double** gxy)
{
    int i,j,w,h;
    double t,sum=0;

  for (i=-r;i<=r;i++){
        h=(i+r);
    for(j=-r;j<=r;j++)
  {
      w=(j+r);
  t=sqrt(h)+sqrt(w);
  gxy[h][w]= (1/(2*Pi))*pow(exp,-(t/2));
  sum=sum+gxy[h][w];
  }
  }

  for (i=-r;i<=r;i++)
    for(j=-r;j<=r;j++)
    {
      gxy[i+r][j+r] /= sum;
    }
}

int main()
{
     std::fstream myfile;

    int imagewidth,imageheight,pixelsize,i,j;
    double** gxy;
    int range=radius*2+1;
    gxy =new double* [range];
    for(i=0;i<range;i++)
        gxy[i]=new double [range];


     gaussian_filter(radius,gxy);

    BITMAPFILEHEADER* fileheader;
    BITMAPINFOHEADER* infoheader;
    RGBQUAD* rgb;
    char* chbuffer=new char[sizeof(BITMAPFILEHEADER)];
    char* chinfobuffer=new char[sizeof(BITMAPINFOHEADER)];

    myfile.open("image1.bmp",ios::binary|ios::in); //where is the error check???

    myfile.seekp(0,ios::beg);
    myfile.read(chbuffer,sizeof(BITMAPFILEHEADER));
    myfile.seekp(0x0E,ios::beg);
    myfile.read(chinfobuffer,sizeof(BITMAPINFOHEADER));
    fileheader=(BITMAPFILEHEADER*)chbuffer;
    infoheader=(BITMAPINFOHEADER*)chinfobuffer;
imagewidth=infoheader->bmpwidth;
imageheight=infoheader->bmpheight;
pixelsize=imagewidth*imageheight*3;
char* pixelbuff=new char[pixelsize];

cout<<imagewidth << " "<<imageheight<<endl;
myfile.seekp(0x36,ios::beg);
myfile.read(pixelbuff,pixelsize);
rgb=new RGBQUAD[imagewidth*imageheight];
rgb=(RGBQUAD*)pixelbuff;
RGBQUAD* rgbcopy;
rgbcopy=rgb;
float sumr,sumg,sumb;


for(i=radius;i<imageheight-radius;i++){
        //std::cout<<i<<endl;
    for(j=radius;j<imagewidth-radius;j++){
           sumr=0;sumg=0;sumb=0;
           //std::cout<<j<<endl;
        for(int k=-radius;k<=radius;k++)
        for(int l=-radius;l<=radius;l++)
{
    //std::cout<<k<<" "<<l<<endl;
    sumr=sumr + ((int)rgbcopy[(i+k)*imagewidth+j+l].red)*gxy[k+radius][l+radius];
    sumg=sumg + ((int)rgbcopy[(i+k)*imagewidth+j+l].green)*gxy[k+radius][l+radius];
    sumb=sumb + ((int)rgbcopy[(i+k)*imagewidth+j+l].blue)*gxy[k+radius][l+radius];
    //cout<<"color"<<endl;
}
rgb[i*imagewidth+j].red=(int)sumr;
rgb[i*imagewidth+j].green=(int)sumg;
rgb[i*imagewidth+j].blue=(int)sumb;
//cout<<"final assign"<<endl;
    }}
pixelbuff=(char*)rgb;
myfile.close();

myfile.open("image4.bmp",ios::binary|ios::out);
myfile.seekp(0x00,ios::beg);
myfile.write(chbuffer,sizeof(BITMAPFILEHEADER));
myfile.seekp(0x0E,ios::beg);
myfile.write(chinfobuffer,sizeof(BITMAPINFOHEADER));
myfile.seekp(0x36,ios::beg);
myfile.write(pixelbuff,pixelsize);

//cout<<"pixel assign"<<endl;

    myfile.close();

    free(pixelbuff);
    free(chbuffer);
    free(chinfobuffer);
    range=radius*2+1;
    for (i=0;i<range;i++)
        delete[] gxy[i];
    delete[] gxy;

    //free(rgb);


    return 0;
}
