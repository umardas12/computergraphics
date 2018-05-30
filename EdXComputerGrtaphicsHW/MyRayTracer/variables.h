#include "vector.h"

#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

const int height=800,width=800,maxcolorvalues=255;
EXTERN vec3f eyeposition;
EXTERN vec3f cameraaim;
EXTERN vec3f updirection;
EXTERN float fovy;
EXTERN vec3f canonicalu;
EXTERN vec3f canonicalv;
EXTERN vec3f canonicalw;

enum shape {sphere,trinagle,cube};
EXTERN int numobjects;
const int maxobjects=10;

const int maxlights=4;
EXTERN float lightposn[4*maxlights];
EXTERN float lightcolor[3*maxlights];
EXTERN int numlightused;

EXTERN vec3f diffusem;
EXTERN vec3f specularm;
EXTERN vec3f ambientm;
EXTERN float shininess;

EXTERN struct object
{
   shape objecttype;
   float size;
   vec3f center;
   vec3f diffuse;
   vec3f ambient;
   vec3f specular;
   float shininess;

}objects[maxobjects];

class Ray
{
public:
    enum raytype{VISIBILITY,SHADOWRAY};
    vec3f rayposition;
    vec3f raydirection;
    raytype type;

    Ray(vec3f eyedirection,vec3f position,char t)
    {
        rayposition=position;
        raydirection=eyedirection;
        if (t=='V')
        type=VISIBILITY;
        else if(t=='S')
            type=SHADOWRAY;
    }
    Ray(){};

    };



