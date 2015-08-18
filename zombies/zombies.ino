#include <Adafruit_NeoPixel.h>

#define PIN 6
#define PIXEL_COUNT 24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

class Thing
{
  public:
    int r = 0;
    int g = 0;
    int b = 0;
};

class Light
{
    float _r = 1;
    float _g = 1;
    float _b = 1;
    public: int _pos = 10;

    public:
    float r(int pos)
    {
      return brightness(pos, _r);
    }

    float g(int pos)
    {
      return brightness(pos, _g);
    }

    float b(int pos)
    {
      return brightness(pos, _b);
    }

    private:
    float brightness(int pos, float color)
    {
      int distance = _pos - pos;
      if (distance == 0) return color;
      return color / (distance * distance);
    }
    
};

class Track
{
  public:
    Thing things[24];

  Track()
  {
    for (int i = 0; i < PIXEL_COUNT; i++)
    {
      things[i].r = 1;
      things[i].g = 10;
      things[i].b = 10;
    }

    things[10].b = 15;

    things[2].r = 20;
  }
};

Track track = Track();
Light light = Light();

void setup()
{
  // put your setup code here, to run once:
  pixels.begin();
}

void loop()
{
  light._pos++;
  if (light._pos > 23) light._pos = 0;
  for(int i = 0; i < PIXEL_COUNT; i++)
  {
    pixels.setPixelColor(i, pixels.Color(track.things[i].r * light.r(i), track.things[i].g * light.g(i), track.things[i].b * light.b(i)));   
  }
  delay(160);
  
  
  pixels.show();
  // put your main code here, to run repeatedly:
}

void shineOn(int val)
{
  
}

