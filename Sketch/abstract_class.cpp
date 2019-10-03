#include <stdio.h>
#define PI 3.141592654


class Solid
{
  public:
    Solid();
    ~Solid();
    
    virtual void printVolume() = 0;
    virtual void printColour() = 0;
};

Solid::Solid(){}
Solid::~Solid(){}


class Sphere: public Solid
{
 public:
   float radius;
   
   int colour;
   
   Sphere();
   Sphere(float, int);
   ~Sphere();
   
   void printVolume();
   void printColour();
};

Sphere::Sphere()
{
    radius = 1.0;
    colour = 0;
}

Sphere::Sphere(float rad, int col)
{
    radius = rad;
    colour = col;
}

Sphere::~Sphere(){}

void Sphere::printVolume()
{
    float volume = 4 / 3 * radius * radius * radius * PI;
    printf("Volume: %.2f\n", volume);
}

void Sphere::printColour()
{
    printf("Colour: %d\n", colour);
}

class Prism: public Solid
{
  public:
    float a;
    float b;
    float c;
    
    int colour;

    Prism();
    Prism(float, float, float, int);
    ~Prism();
    
    void printVolume();
    void printColour();
};

Prism::Prism()
{
    a = 1.0;
    b = 1.0;
    c = 1.0;
    
    colour = 0;
}

Prism::Prism(float lg_a, float lg_b, float lg_c, int col)
{
    a = lg_a;
    b = lg_b;
    c = lg_c;
    
    colour = col;
}

Prism::~Prism(){}

void Prism::printVolume()
{
    float volume;
    volume = a * b * c;
    printf("Volume: %.2f\n", volume);
}

void Prism::printColour()
{
    printf("Colour: %d\n", colour);
}


int main()
{
    int nPrism = 5;
    Prism* boxes = new Prism[nPrism];
    
    for(int i = 0; i < nPrism; i++)
    {
        boxes[i].a = (i + 1) * 1.0;
        boxes[i].b = (i + 1) * (i + 1) * 1.0;
        boxes[i].c = 3 * (i + 1) * 1.0;
        
        boxes[i].colour = 8;
    }
    
    int nSphere = 3;
    Sphere* balls = new Sphere[nSphere];
    
    for(int i = 0; i < nSphere; i++)
    {
        balls[i].radius = (i + 1) * 1.0;
        balls[i].colour = 16;
    }

    Solid** bodypointer = new Solid*[nPrism + nSphere];
    
    for(int i = 0; i < nPrism; i++)
    {
        bodypointer[i] = &boxes[i];
    }
    int p = 0;
    for(int j = nPrism; j < nPrism + nSphere; j++)
    {
        bodypointer[j] = &balls[p];
        p++;
    }

    for(int i = 0; i < nPrism + nSphere; i++)
    {
        bodypointer[i]->printVolume();
        bodypointer[i]->printColour();
    }

    return 0;
}