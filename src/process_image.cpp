// COPY YOUR FILE FROM THE PREVIOUS ASSIGNMENT HERE
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  // TODO: calculate the pixels of 'gray'
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float r = im(x, y, 0);
        float g = im(x, y, 1);
        float b = im(x, y, 2);
        gray(x, y, 0) = 0.299f * r + 0.587f * g + 0.114f * b;
    }
}

  
  
  
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        im(x, y, c) += v;
    }
}

  
  
  
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        im(x, y, c) *= v;
    }
}

  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  
 int size = im.w * im.h * im.c;
for (int i = 0; i < size; ++i) {
    if (im.data[i] < 0.0f) im.data[i] = 0.0f;
    if (im.data[i] > 1.0f) im.data[i] = 1.0f;
}

  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to HSV format
  
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float r = im(x, y, 0);
        float g = im(x, y, 1);
        float b = im(x, y, 2);

        float max_val = max(r, g, b);
        float min_val = min(r, g, b);
        float delta = max_val - min_val;

        float h = 0.0f, s = 0.0f, v = max_val;

        if (delta != 0.0f) {
            s = delta / max_val;

            if (max_val == r)
                h = (g - b) / delta + (g < b ? 6.0f : 0.0f);
            else if (max_val == g)
                h = (b - r) / delta + 2.0f;
            else
                h = (r - g) / delta + 4.0f;

            h /= 6.0f;
        }

        im(x, y, 0) = h;
        im(x, y, 1) = s;
        im(x, y, 2) = v;
    }
}

  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from HSV format to RGB format
  
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float h = im(x, y, 0) * 6.0f;
        float s = im(x, y, 1);
        float v = im(x, y, 2);

        int i = static_cast<int>(floor(h));
        float f = h - i;
        float p = v * (1 - s);
        float q = v * (1 - s * f);
        float t = v * (1 - s * (1 - f));

        float r, g, b;

        switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }

        im(x, y, 0) = r;
        im(x, y, 1) = g;
        im(x, y, 2) = b;
    }
}

  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  
  
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float pi = std::acos(-1.0f);
        float r = im(x, y, 0);
        float g = im(x, y, 1);
        float b = im(x, y, 2);

        float L = (r + g + b) / 3.0f;
        float C = sqrt((r - g)*(r - g) + (r - b)*(g - b));
        float H = atan2(g - b, r - g);
        if (H < 0) H += 2 * pi;

        im(x, y, 0) = L;
        im(x, y, 1) = C;
        im(x, y, 2) = H / (2 * pi); // Normalizzato tra 0 e 1
    }
}

  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float pi = std::acos(-1.0f);
        float L = im(x, y, 0);
        float C = im(x, y, 1);
        float H = im(x, y, 2) * (2 * pi);

        float r = L + C * cos(H);
        float g = L + C * sin(H);
        float b = L - (C / 2.0f) * sin(H);

        im(x, y, 0) = r;
        im(x, y, 1) = g;
        im(x, y, 2) = b;
    }
}

  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
