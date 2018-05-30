# version 120

/* This is the fragment shader for reading in a scene description, including
   lighting.  Uniform lights are specified from the main program, and used in
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do
 //in vec4 color;
 //in vec3 mynormal;
 //in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal;
varying vec4 myvertex;

const int numLights = 10;
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to
// a buffer.  But for now, I'll just make them uniform.
// I use ambient, diffuse, specular, shininess as in OpenGL.
// But, the ambient is just additive and doesn't multiply the lights.

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

vec4 ComputeLight ( const in vec3 normal,const in vec3 lightd, const in vec3 halfv, const in vec4 lightcolor,
                   const in vec4 diffuse, const in vec4 specular,const in float shininess)
{
        float nDotL=dot(normal,lightd);
    vec4 lambert =diffuse*lightcolor*max(nDotL,0.0);
    float nDotH=dot(normal,halfv);
    vec4 phong=specular * lightcolor * pow(max(nDotH,0.0),shininess);
	vec4 retcolor=lambert + phong;
	return retcolor;
}

void main (void)
{
    if (enablelighting) {
        vec4 finalcolor;
        finalcolor=vec4(0.0,0.0,0.0,0.0);

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader
        vec3 eyepos,mypos,eyedirn,_normal,normal,lposition[numLights],lightd[numLights],halfv[numLights];
        vec4 nlightp[numLights],_mypos,lcol[numLights];
eyepos=vec3(0.0,0.0,0.0);
        _mypos=gl_ModelViewMatrix*myvertex;
        mypos=_mypos.xyz/_mypos.w;
        eyedirn=normalize(eyepos - mypos); //eye direction
        _normal=(gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0)).xyz;
        normal=normalize(_normal);//normal of vertex
        for (int i=0;i<numused;i++){
                if(lightposn[i].w==0){
          lposition[i]=lightposn[i].xyz;
		  }
        else{
            lposition[i]=lightposn[i].xyz/lightposn[i].w;
			}

         lightd[i]=normalize(lposition[i]-mypos) ;
        halfv[i]=normalize(eyedirn + lightd[i]);
        lcol[i]=ComputeLight(normal,lightd[i],halfv[i],lightcolor[i],diffuse,specular,shininess);
        }
        // Color all pixels black for now, remove this in your implementation!
        //finalcolor = vec4(0,0,0,1);
        finalcolor=ambient;
        for (int i=0;i<numused;i++)
        finalcolor=finalcolor+lcol[i];

        gl_FragColor = finalcolor;
    } else {
        gl_FragColor = color;
    }
}
