//
// Arduino minimal raytracer
//
// Oscar Sebio Cajaraville 2014
//

// SPI pins for the screen
#define cs  10
#define dc  9
#define rst 8

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "math.h"

// Screen lib creation
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void setup() 
{
  // Init screen library
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  
  Serial.begin(9600);
  Serial.print("hello!");
}

void loop() 
{
  // put your main code here, to run repeatedly:
  raytracer();
}

//
// The easiest raytracer in the world
//

float sphereCollision(const ray &r, const sphere &s, vec3 &o_p, vec3 &o_n)
{
  vec3 rs = r.o - s.o;
  float b = 2.0 * dot(r.d, rs);
  float c = dot(rs, rs) - s.r * s.r;
  float a = b * b - 4.0 * c;
  if (a < 0.0) return -1.0;
  
  float d = sqrt(a);
  float t0 = 0.5 * (-b + d);
  float t1 = 0.5 * (-b - d);
  float t = t1 > 0.0 ? t1 : t0;
  
  if (t > 0.0)
  {
    o_p.x = r.o.x + r.d.x * t;
    o_p.y = r.o.y + r.d.y * t;
    o_p.z = r.o.z + r.d.z * t;
    o_n.x = o_p.x - s.o.x;
    o_n.y = o_p.y - s.o.y;
    o_n.z = o_p.z - s.o.z;
    o_n.normalize();
  }
  
  return t;
}

float planeCollision(const ray &r, const plane &p, vec3 &o_p, vec3 &o_n)
{
  float d = dot(p.p - r.o, p.n) / dot(r.d, p.n);
  if (d > 0.0)
  {
    o_p = r.o + (r.d * d);
    o_n = p.n;
  }
  return d;
}

float TFT_W = float(tft.width());
float TFT_H = float(tft.height());
float TFT_RATIO = TFT_H / TFT_W;

void cameraRay(int16_t x, int16_t y, ray &o_r)
{
  o_r.o.x = 0.0;
  o_r.o.y = 0.0;
  o_r.o.z = 0.0;
  
  o_r.d.x = float(x) / TFT_W - 0.5;
  o_r.d.y = (float(y) / TFT_H - 0.5) * TFT_RATIO;
  o_r.d.z = 1.5;
  o_r.d.normalize();
}

bool scene(const ray &r, vec3 &o_p, vec3 &o_n, colorf &o_c)
{
  sphere scene_s0 = {0.0, 0.0, 8.0, 2.0};
  sphere scene_s1 = {1.0, 0.0, 4.0, 0.5};
  plane scene_p0 = {0.0, 0.0, 11.0, 0.0, 0.0, -1.0};
  plane scene_p1 = {-3.0, 0.0, 0.0, 1.0, 0.0, 0.0};
  plane scene_p2 = {3.0, 0.0, 0.0, -1.0, 0.0, 0.0};
  plane scene_p3 = {0.0, 3.0, 0.0, 0.0, -1.0, 0.0};
  plane scene_p4 = {0.0, -3.0, 0.0, 0.0, 1.0, 0.0};

  const float kDistError = 0.05;
  const float kDistLimit = 99999;
  
  float minDist = kDistLimit;
  float d;
  vec3 p, n;
  
  d = sphereCollision(r, scene_s0, p, n);
  if (d > kDistError && d < minDist)
  {
    colorf c = {0.0, 0.0, 0.6};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = sphereCollision(r, scene_s1, p, n);
  if (d > kDistError && d < minDist)
  {
    colorf c = {0.0, 1.0, 0.0};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = planeCollision(r, scene_p0, p, n);
  if (d > kDistError & d < minDist)
  {
    colorf c = {1.0, 1.0, 1.0};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = planeCollision(r, scene_p1, p, n);
  if (d > kDistError & d < minDist)
  {
    colorf c = {1.0, 0.0, 0.0};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = planeCollision(r, scene_p2, p, n);
  if (d > kDistError & d < minDist)
  {
    colorf c = {0.0, 1.0, 0.0};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = planeCollision(r, scene_p3, p, n);
  if (d > kDistError & d < minDist)
  {
    colorf c = {0.3, 0.3, 0.3};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  d = planeCollision(r, scene_p4, p, n);
  if (d > kDistError & d < minDist)
  {
    colorf c = {0.3, 0.3, 0.3};
    o_c = c; o_p = p; o_n = n;
    minDist = d;
  }
  
  return (minDist < kDistLimit);
}

void lighting(const vec3 &v, const vec3 &n, const vec3 &l, const colorf &mc, colorf &o_c)
{
  const float ambient = 0.5;
  float cosln = clamp(dot(l, n), 0.0, 1.0) + ambient;
  o_c.r = mc.r * cosln;
  o_c.g = mc.g * cosln;
  o_c.b = mc.b * cosln;
}

void raytracer()
{
  const uint16_t kNumRays = 2;
  
  vec3 light_pos = {3.0, 1.0, 0.0};
  
  int16_t x,y;
  ray r;
  
  int16_t padx = 0;
  int16_t pady = 0;
  for (int16_t pad = 0; pad < 4*4; pad++)
  {
    for (y = pady; y < tft.height(); y+=4)
    {
      for (x = padx; x < tft.width(); x+=4)
      {
        colorf final_color = {0, 0, 0};
        cameraRay(x, y, r);
        
        vec3 n, p;
        colorf mc, c;
        
        float e = 1.0;
        for (int16_t i = 0; i < kNumRays; ++i)
        {
          if (scene(r, p, n, mc))
          {
            vec3 l = light_pos - p;
            l.normalize();
            lighting(r.d, n, l, mc, c);
            final_color += c * e;
            e *= 0.5; // I know, this is wrong, implement materials!
            r.o = p;
            r.d = reflect(r.d, -n);
          }
          else
          {
            break; // No collision, no more rays
          }
        }
        
        final_color.saturate();
        color c8 = cf2c8(final_color);
        uint16_t c16 = tft.Color565(c8.r, c8.g, c8.b);
        tft.drawPixel(x, y, c16);
      }
    }
  
    // This helps to visualize the result quickier
    // Generating first one pixel for each 4x4 cell
    padx++;
    if ((pad % 2) == 1) { padx-=2; pady++; }
    if (pad == 7) { padx+=2; pady-=4; }
  }
  
  
  Serial.print("frame!");
}


