#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN            6
#define NUMPIXELS      24
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Game settings
int SPEED_INCREMENT = 2;
int GREEN_FOOD = 50;
int BLUE_FOOD = 150;
int RED_FOOD = 200;
int SPAWN_RATE = 6; // Spawn every 1/SPAWN_RATE ticks.

// Power up active
boolean _powered = false;
// Current speed
int _speed = 10;
int _direction = 1;
int _location = 0;
// Color
int _r = 1;
int _g = 1;
int _b = 1;

// State values
int _state[24];
const int EMPTY = 0;
const int PELLET = 1;
const int GHOST = 2;
const int POWER = 3;

int _lastPress = 1;

bool playOn = false;

void setup() 
{
  pixels.begin();
  pinMode(3, INPUT);
  digitalWrite(3, HIGH);  
}

void loop()
{  
   initialize();
   while(playOn == true)
   {
      play();
   }
}

void initialize()
{
  for (int i = 0; i < 24; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    _state[i] = EMPTY;
  }
  pixels.show();
  playOn = true;
}

void play()
{  
    randomSeed(millis()); 
    int buttonPress = digitalRead(3);
    int lastPress = buttonPress;
    buttonPress = buttonPress ^ _lastPress;
    _lastPress = lastPress;
    if (buttonPress != 0 && lastPress == 0) _direction -= 2 * _direction; 
    
    //clean()
    spawn();
 
    int pac = _location;
    int state = _state[_location];
    
    // Eat a pellet
    if (state == PELLET)
    {
      _g = constrain (_g + GREEN_FOOD, 0, 255);    
    }
    // Hit a ghost
    else if (state == GHOST)
    {
      if (_powered)
      {
        _b = constrain(_b + BLUE_FOOD, 0, 255);
        //TODO
        _powered = false;
        _r = false;
      }
      else
      {
        _g = 1;
        _b = 1;
        die();
        return;
      }
    } 
    // Eat a power-up
    else if (state == POWER)
    {
      _powered = true;
      _r = constrain(_r + RED_FOOD, 0, 255);
      _speed += SPEED_INCREMENT;
    }

    // Set pacman colour
    if (_powered) 
    {
      pixels.setPixelColor(pac, pixels.Color(255, 255, 255));
    }
    else
    {
      pixels.setPixelColor(pac, pixels.Color(100, 100, 0));
    }
    pixels.show();
    delay(1000/_speed);
    
    pixels.setPixelColor(pac, pixels.Color(0, 0, 0));
    _state[_location] = EMPTY;
  
    move();
}

void move()
{
  if (_direction < 0 && _location == 0) _location = 23;
  else if (_direction > 0 && _location == 23) _location = 0;
  else _location += _direction;
}

void spawn()
{
  boolean doSpawn = random(SPAWN_RATE) == 1;
  if (!doSpawn) return;
  
  int spawnPoint = random(24);
  
  // Decide type by rolling a d20.
  int d20roll = random(20) + 1;
  int type;
  if (d20roll <= 2) type = GHOST;
  else if (d20roll == 20) type = POWER;
  else type = PELLET;
  
  // Add new spawned thing
  _state[spawnPoint] = type;
  
  // Draw that shit
  switch(type)
  {
    case PELLET:
      pixels.setPixelColor(spawnPoint, pixels.Color(0, 100, 0));
      break;
    case GHOST:
      pixels.setPixelColor(spawnPoint, pixels.Color(0, 0, 100));
      break;
    case POWER:
      pixels.setPixelColor(spawnPoint, pixels.Color(255, 255, 255));
      break;
    case EMPTY:
      pixels.setPixelColor(spawnPoint, pixels.Color(0, 0, 0));
      break;
    }
    
    pixels.show();
}

void die()
{
  playOn = false;
  _speed = 10;
    for (int i = 0; i < 24; i++)
    {
      if (i >= 0)pixels.setPixelColor(i, pixels.Color(100, 100, 0));
      else pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(50);
    }
  

  delay(1000);
  initialize();
  playOn = true;
}

void endState()
{
  for (int i = 0; i < 24; i++)
  {
    if (i == random(3)) pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    else pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    
    pixels.show();
    delay(666);
  }
}


