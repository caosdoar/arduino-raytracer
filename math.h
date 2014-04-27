//
// Basic math types and functions for the raytracer
//
// Oscar Sebio Cajaraville 2014
//

struct vec3 
{ 
  float x,y,z;
  
  vec3 operator +(const vec3 &v) const
  {
    vec3 r = {x + v.x, y + v.y, z + v.z};
    return r;
  }
  
  vec3 operator -(const vec3 &v) const
  {
    vec3 r = {x - v.x, y - v.y, z - v.z};
    return r;
  }
  
  vec3& operator +=(const vec3 &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  
  vec3& operator -=(const vec3 &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  
  vec3 operator -() const
  {
    vec3 r = {-x, -y, -z};
    return r;
  }
  
  vec3 operator *(float c) const
  {
    vec3 o = {x * c, y * c, z * c};
    return o;
  }
  
  vec3& normalize()
  {
    float nd_rcp = 1.0 / sqrt(x * x + y * y + z * z);
    x *= nd_rcp;
    y *= nd_rcp;
    z *= nd_rcp;
    return *this;
  }
};

struct ray 
{ 
  vec3 o,d; 
};

struct sphere 
{ 
  vec3 o; float r;
};

struct plane
{
  vec3 p, n;
};

struct color 
{ 
  uint8_t r,g,b; 
};

struct colorf 
{ 
  float r,g,b;
  
  colorf operator +(const colorf &c2)
  {
    colorf c = {r + c2.r, g + c2.g, b + c2.b};
    return c;
  }
  
  colorf& operator +=(const colorf &c2)
  {
    r += c2.r;
    g += c2.g;
    b += c2.b;
    return *this;
  }

  colorf operator *(float c)
  {
    colorf o;
    o.r = r * c;
    o.g = g * c;
    o.b = b * c;
    return o;
  }
  
  colorf& saturate()
  {
    r = max(0.0, min(1.0, r));
    g = max(0.0, min(1.0, g));
    b = max(0.0, min(1.0, b));
    return *this;
  }
};

// Float functions

float clamp(float x, float _min, float _max)
{
  return max(_min, min(_max, x));
}

// Vector functions

float dot(const vec3 &v1, const vec3 &v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


vec3 cross(const vec3 &v1, const vec3 &v2)
{
  vec3 o = {
    v1.x * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x};
 return o;
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
  float d = 2.0 * dot(v, n);
  vec3 o;
  o.x = v.x - d * n.x;
  o.y = v.y - d * n.y;
  o.z = v.z - d * n.z;
  return o;
}

// Colour functions

color cf2c8(const colorf &c)
{
  color c8 = {
    uint8_t(c.r * 255.0),
    uint8_t(c.g * 255.0),
    uint8_t(c.b * 255.0)};
  return c8;
}
