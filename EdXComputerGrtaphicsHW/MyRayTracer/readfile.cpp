#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "variables.h"




void canonicalcoordinate(const vec3f& eye,const vec3f& aim,const vec3f& up)
{
vec3f u,v,w;
w=eye;
w.normalize();
u=crossprod(up,w);
u.normalize();
v=crossprod(w,u);
v.normalize();
canonicalu=u;
canonicalv=v;
canonicalw=w;
}

bool readval(std::stringstream &s, const int numvals, float* values)
{
for(int i=0;i<numvals;i++)
{
    s>>values[i];
    if(s.fail()) {
            std::cout<<"Failed reading values "<< i << " ,skipping the value"<<std::endl;
    return false;
    }
}
return true;
}
void readfile(const char* filename)
{
std::string str,cmd;
std::ifstream in;
in.open(filename);
numobjects=0;
numlightused=0;
if(in.is_open())
{

std::getline(in,str);
while(in){
        if((str.find_first_not_of(" \t\n\r")!=std::string::npos) && (str[0]!='#')){
          std::stringstream s(str);

            s>>cmd;
            float values[10];
            bool validinput;
            if(cmd=="camera"){
                validinput=readval(s,10,values);
                if(validinput){

                    eyeposition={values[0],values[1],values[2]};
                    cameraaim={values[3],values[4],values[5]};
                    updirection={values[6],values[7],values[8]};
                     fovy=values[9];
                     canonicalcoordinate(eyeposition,cameraaim,updirection);

                }
            }
            if(cmd=="diffuse"){
                    validinput=readval(s,3,values);
            if (validinput){
                diffusem={values[0],values[1],values[2]};
            }

            }
             if(cmd=="specular"){
               validinput=readval(s,3,values);
            if (validinput){
                specularm={values[0],values[1],values[2]};
            }
            }
             if(cmd=="ambient"){
                validinput=readval(s,3,values);
            if (validinput){
                ambientm={values[0],values[1],values[2]};
            }
            }
             if(cmd=="shininess"){
              validinput=readval(s,1,values);
            if (validinput){
                shininess=values[0];
            }
            }
            if(cmd=="Sphere"){
                    validinput=readval(s,4,values);
                    if (validinput){
                object *obj=&objects[numobjects];
                obj->size=values[0];
                obj->center={values[1],values[2],values[3]};
            obj->objecttype=sphere;

                obj->diffuse=diffusem;
                obj->specular=specularm;
                obj->ambient=ambientm;
                obj->shininess=shininess;
            ++numobjects;
            }


            }
            if(cmd=="Light"){
                validinput=readval(s,7,values);
                if (validinput) {
                           int i,j;
                            for (i=0;i<4;i++,j++) lightposn[numlightused*4+i]=values[i];
                            for (i=4,j=0;i<7,j<3;i++,j++) lightcolor[numlightused*4+j]=values[i];

                ++numlightused;
            }
        }
        }
        std::getline(in,str);
}
}
else{
    std::cerr<<"Unable to open file : "<<filename<<std::endl;
}

}





