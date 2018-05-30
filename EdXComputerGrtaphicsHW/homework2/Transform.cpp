// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    float rad=(degrees*pi/180);
glm::mat3 I3,vec3M,A,F1,F2,F3,F;
int i,j;
float x,y,z,c,s,x2,xy,y2,yz,xz,z2;
for(i=0;i<3;i++)
    for (j=0;j<3;j++)
{ if(i==j)
    I3[i][j]=1.0;
    else
        I3[i][j]=0.0;
};
x=axis[0];
y=axis[1];
z=axis[2];
c=cos(rad);
s=sin(rad);
x2=x*x;
xy=x*y;
y2=y*y;
yz=y*z;
xz=x*z;
z2=z*z;
vec3M=mat3(x2,xy,xz,xy,y2,yz,xz,yz,z2);
A=mat3(0,z,-y,-z,0,x,y,-x,0);

 F1=I3*c;
 F2=vec3M*(1-c);
 F=F1+F2;
F3=A*s;
F=F+F3;
  // You will change this return call
  return mat3(F);
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
   // YOUR CODE FOR HW1 HERE
glm::mat3 R3;
glm::vec3 eyel,upl,axis;
axis=vec3(0.0,1.0,0.0);//y-axis;
R3=Transform::rotate(degrees,axis);
R3=glm::transpose(R3);
eyel=eye*R3;
eye=eyel;
upl=up*R3;
up=upl;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    // YOUR CODE FOR HW1 HERE
  glm::mat3 R3;
glm::vec3 eyel,upl,axis;
axis=vec3(-1.0,0.0,0.0);//x-axis;
R3=Transform::rotate(degrees,axis);
R3=glm::transpose(R3);
eyel=eye*R3;
eye=eyel;
upl=up*R3;
up=upl;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
   // YOUR CODE FOR HW1 HERE
//float xu,yu,zu,xv,yv,zv,xw,yw,zw;
vec3 v,u,w;
w=glm::normalize(eye);
u=glm::cross(up,w);
u=glm::normalize(u);
v=glm::cross(w,u);
v=glm::normalize(v);

float ud,vd,wd;
ud=glm::dot(u,eye);
vd=glm::dot(v,eye);
wd=glm::dot(w,eye);
vec4 v4,u4,w4,I;
v4=vec4(v,-vd);
u4=vec4(u,-ud);
w4=vec4(w,-wd);
I=vec4(0.0,0.0,0.0,1.0);
mat4 R;
R=mat4(u4,v4,w4,I);
R=glm::transpose(R);
 // You will change this return call
  return mat4(R);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
float theta=fovy/2;
float rad=(theta*pi/180);
float d=cos(rad)/sin(rad);
float x=d/aspect;
float A= -((zFar+zNear)/(zFar-zNear));
float B=-2*((zFar*zNear)/(zFar-zNear));
ret=mat4(x,0,0,0,0,d,0,0,0,0,A,-1,0,0,B,0);

               // New, to implement the perspective transform as well.
    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    ret=mat4(sx,0.0,0.0,0.0,0.0,sy,0.0,0.0,0.0,0.0,sz,0.0,0.0,0.0,0.0,1.0);
    // Implement scaling
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    ret=mat4(1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,tx,ty,tz,1.0);
    // Implement translation
    return ret;
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
    vec3 x = glm::cross(up,zvec);
    vec3 y = glm::cross(zvec,x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
