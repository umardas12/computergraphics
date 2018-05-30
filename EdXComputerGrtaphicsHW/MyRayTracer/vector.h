#include <math.h>
#include <assert.h>

template <class T>
class vec3
{
    public:
    //x,y,z
    T n[3];

    //Constructor
    vec3(){n[0]=0.0;n[1]=0.0;n[2]=0.0;}
    vec3(const T x,const T y,const T z){n[0]=x;n[1]=y;n[2]=z;}
    vec3(const vec3<T>& v){n[0]=v[0];n[1]=v[1];n[2]=v[2];}

    //access operator
    T& operator [](int i)
    {return n[i];}
    T operator [](int i) const
    {return n[i];}

    // operator
    vec3<T>& operator = (const vec3<T>& v)
    {
        n[0]=v[0];n[1]=v[1];n[2]=v[2];return *this;
    }
    vec3<T>& operator * (const T value)
    {
        n[0]*= value;
        n[1]*=value;
        n[2]*=value;
    }
    vec3<T>& operator * (const vec3<T>& v)
    {
        n[0]*= v[0];
        n[1]*=v[1];
        n[2]*=v[2];
    }
    vec3<T>& operator + (const vec3<T>& v)
    {
        n[0]=n[0]+v[0];
        n[1]=n[1]+v[1];
        n[2]=n[2]+v[2];
    }
vec3<T>& operator - (const vec3<T>& v)
    {
        n[0]=n[0]-v[0];
        n[1]=n[1]-v[1];
        n[2]=n[2]-v[2];
    }
    //length
    double length()
    {
        double l;
        l= n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
        l=sqrt(l);
        return l;
    }

    //normalize
    void normalize()
    {
        double len=length();
        assert(len != 0);
        n[0]/=len;n[1]/=len;n[2]/=len;

    }



};

typedef vec3<float> vec3f;
typedef vec3<double>vec3d;

 //products
    template <typename T>
    vec3<T> crossprod(const vec3<T>& u,const vec3<T>& v)
{
    T x,y,z;
    x=u[1]*v[2]-u[2]*v[1];
    y=u[2]*v[0]-u[0]*v[2];
    z=u[0]*v[1]-u[1]*v[0];

    vec3<T> resultv(x,y,z);
    return resultv;
}

template <typename T>
    T dot(const vec3<T>& u,const vec3<T>& v)
{
    T x,y,z,result;
    x=u[0]*v[0];
    y=u[1]*v[1];
    z=u[2]*v[2];

    result=x+y+z;
    return result;
}

