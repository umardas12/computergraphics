
//#include<stdlib>
#include<iostream>
#include<string>
#include<fstream>
#include<CImg.h>
#include<math.h>

#define MAINPROGRAM
#include "variables.h"
#include "readfile.h"

void createimage(char* color)
{ std::cout<<"inside create image"<<std::endl;

    std::ofstream filestream;
    unsigned int imagesize=width*height;
    filestream.open("outputimage.ppm",std::ios::binary|std::ios::out);
filestream<<"P6"<<"\n"<<width<<"\n"<<height<<"\n"<<maxcolorvalues<<"\n";
unsigned char * imagebuffer = new unsigned char[imagesize*3];
    for(int y=0;y<height;y++)
        for(int x=0;x<width;x++)
        for(int rgb=0;rgb<3;rgb++)
        {
          imagebuffer[y*width*3+x*3+rgb]=color[y*width*3+x*3+rgb];
        }


filestream.write((char*)imagebuffer,imagesize*3);


}

Ray computeray(const vec3f& eye,const float& fovy,int x, int y)
{
    float fradian =(3.14159*(fovy/2))/180;
float alpha,beta,ww=width/2,hh=height/2;
alpha=((x-ww)/ww)*(tan(fradian));
beta=((hh-y)/hh)*(tan(fradian))*(width/height);
vec3f u,v,w,temp,eyedirection,canu,canv,canw;
canu=canonicalu;
canv=canonicalv;
canw=canonicalw;
u=canu*alpha;
v=canv*beta;
w=canw*(-1);
temp=u+v;
eyedirection=temp+w;
eyedirection.normalize();
Ray ray;
ray={eyedirection,eye,'V'};
return ray;
}

bool isect(const Ray& ray,const object& obj,double& t0)
{
    vec3f eminusc,rayp,rayd,cntr;
    double t1;
    rayp=ray.rayposition;
    rayd=ray.raydirection;
    cntr=obj.center;
    eminusc=rayp-cntr;
    float deminusc=dot(rayd,eminusc);
    float dd=dot(rayd,rayd);
    float eminusc2=dot(eminusc,eminusc);
    float radius=obj.size;

        float determinant=(deminusc*deminusc)-(dd)*(eminusc2-(radius*radius));
        if(determinant<0)
            return false;
        else {
            t0=(-deminusc+sqrt(determinant))/dd;
        t1=(-deminusc-sqrt(determinant))/dd;
        }

    if(t0<t1 && t0>0) {
    return true;}
    else if(t1<t0 && t1>0)
    {
                t0=t1;
        return true;
    }
else if(t1==t0 && t0>0)
        return true;
   else return false;


}

vec3f ComputeLight ( const vec3f normal,const vec3f lightd, const vec3f halfv, const vec3f lightcolor,
                   const vec3f diffuse, const vec3f specular,const float shininess )
{
    vec3f tdiffuse,tspecular;
tdiffuse=diffuse;
tspecular=specular;
        float nDotL=dot(normal,lightd);
        vec3f tempcolor=tdiffuse*lightcolor;//normal product
    vec3f lambert =tempcolor*fmax(nDotL,0.0);
    float nDotH=dot(normal,halfv);
    tempcolor=tspecular * lightcolor;
    vec3f phong=tempcolor* pow(fmax(nDotH,0.0),shininess);
	vec3f retcolor=lambert + phong;
	/*if (retcolor[0]>1 || retcolor[1]>1 ||retcolor[2]>1)
    {std::cout<<" zero ";
    }*/
	return retcolor;
}

vec3f getcolor( const object& obj,const double t0,const Ray& visibilityray)
{
    vec3f rayp,rayd,hitvertex,temp,finalcolor,diffuse,specular,ambient;
    float shininess;

    rayp=visibilityray.rayposition;
    rayd=visibilityray.raydirection;
    temp=rayd*t0;
    hitvertex=rayp+rayd;
    diffuse = obj.diffuse;
    specular=obj.specular;
    ambient=obj.ambient;
    shininess=obj.shininess;

    vec3f eyepos,mypos,eyedirn,normal,lposition,lightd,_lightd,halfv,lcol,_lightcolor;

        eyepos=visibilityray.rayposition;

        mypos=hitvertex;
        eyedirn=eyepos - mypos;
        eyedirn.normalize(); //eye direction

        normal=mypos-obj.center;
        mypos=hitvertex;
        normal.normalize();//normal of vertex

finalcolor=ambient;
        for (int i=0;i<numlightused;i++){
                if(lightposn[i*4+3]==0){
          lposition={lightposn[i*4+0],lightposn[i*4+1],lightposn[i*4+2]};
		  }
        else{
            lposition={lightposn[i*4+0]/lightposn[i*4+3],lightposn[i*4+1]/lightposn[i*4+3],lightposn[i*4+2]/lightposn[i*4+3]};
			}
			_lightcolor={lightcolor[i*4+0],lightcolor[i*4+1],lightcolor[i*4+2]};
         temp=lposition;

         lightd=temp-mypos ;
         lightd.normalize();
        halfv=eyedirn + lightd;
        halfv.normalize();
        lcol=ComputeLight(normal,lightd,halfv,_lightcolor,diffuse,specular,shininess);
        finalcolor=finalcolor+lcol;
        }
       /* if (finalcolor[0]>1 || finalcolor[1]>1 ||finalcolor[2]>1)
    {std::cout<<" greater ";
    }*/
if (finalcolor[0]>1 ) finalcolor[0]=1;
if (finalcolor[1]>1 ) finalcolor[1]=1;
if (finalcolor[2]>1 ) finalcolor[2]=1;
return finalcolor;
}

void raytracer()
{
    std::cout<<"inside raytracer";
    double t0,thit;
    int o,hitobject;
    char color[3*width*height];
    for (int y=0;y<height;y++){
        for(int x=0;x<width;x++)
         {
           thit=0;t0=0;
           Ray visibilityray=computeray(eyeposition,fovy,x,y);
         for( o=0;o<numobjects;o++){
        bool hit=isect(visibilityray, objects[o],t0);
        if(hit)
        {    if(thit==0 && t0>0)
        {
           thit=t0;
            hitobject=o;
        }
            else if (t0<thit && t0>0)
                {thit=t0;
            hitobject=o;}
        }
         }


        if(thit>0)
        {
            vec3f calcolor=getcolor(objects[hitobject],thit,visibilityray);
            for (int rgb=0;rgb<3;rgb++)
   color[y*width*3+x*3+rgb] = (char)(255*calcolor[rgb]);
}
else if(thit==0) {color[y*width*3+x*3+0] =0;
color[y*width*3+x*3+1] =0;
color[y*width*3+x*3+2] =0;
}
        }
}

        createimage(&color[0]);

}
int main(int argc, char** argv)
{

    readfile(argv[1]);
    raytracer();
        return 0;
}





