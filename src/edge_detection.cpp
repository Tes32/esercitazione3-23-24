#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846


/*
Smooths a grayscale image by convolving it with a Gaussian kernel of standard deviation sigma.
Input:
    Image im: the input image
    float sigma: the standard deviation of the Gaussian kernel
Output:
    Image: the smoothed image (im.w, im.h, 1)
*/
Image smooth_image(const Image& im, float sigma)
{
    // TODO: Your code here
     Image filter = make_gaussian_filter(1.4f);
     Image smoothed = convolve_image(im, filter, true);
     return smoothed;

}


/*
Computes the magnitude and direction of the gradient of an image.
Input:
    Image im: the input image
Output:
    pair<Image,Image>: the magnitude and direction of the gradient of the image
                       with magnitude in [0,1] and direction in [-pi,pi]
*/
pair<Image,Image> compute_gradient(const Image& im)
{
    // TODO: Your code here
    Image sobel_x = make_gx_filter();
    Image sobel_y = make_gy_filter();

    Image gx = convolve_image(im, sobel_x, true);
    Image gy = convolve_image(im, sobel_y, true);

    Image mag(im.w, im.h, 1);
    Image theta(im.w, im.h, 1);

    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            float dx = gx.clamped_pixel(x, y, 0);
            float dy = gy.clamped_pixel(x, y, 0);
            mag.set_pixel(x, y, 0, std::sqrt(dx * dx + dy * dy));
            theta.set_pixel(x, y, 0, std::atan2(dy, dx));
        }
    }

    return {mag, theta};
}


/*
Performs non-maximum suppression on an image.
Input:
    Image mag: the magnitude of the gradient of the image [0,1]
    Image dir: the direction of the gradient of the image [-pi,pi]
Output:
    Image: the image after non-maximum suppression
*/
Image non_maximum_suppression(const Image& mag, const Image& dir)
{
    Image nms(mag.w, mag.h, 1);
    float neighbor1, neighbor2;

    // Iterate through the image and perform non-maximum suppression
    for (int y = 0; y < mag.h; y++) {
        for (int x = 0; x < mag.w; x++) {
            
            // TODO: Your code here
            float angle = dir.clamped_pixel(x, y, 0);
float magnitude = mag.clamped_pixel(x, y, 0);

float deg = angle * 180.0f / M_PI;
if (deg < 0) deg += 180;

float neighbor1 = 0, neighbor2 = 0;

if ((deg >= 0 && deg < 22.5) || (deg >= 157.5 && deg <= 180)) {
    neighbor1 = mag.clamped_pixel(x + 1, y, 0);
    neighbor2 = mag.clamped_pixel(x - 1, y, 0);
} else if (deg >= 22.5 && deg < 67.5) {
    neighbor1 = mag.clamped_pixel(x + 1, y - 1, 0);
    neighbor2 = mag.clamped_pixel(x - 1, y + 1, 0);
} else if (deg >= 67.5 && deg < 112.5) {
    neighbor1 = mag.clamped_pixel(x, y + 1, 0);
    neighbor2 = mag.clamped_pixel(x, y - 1, 0);
} else if (deg >= 112.5 && deg < 157.5) {
    neighbor1 = mag.clamped_pixel(x - 1, y - 1, 0);
    neighbor2 = mag.clamped_pixel(x + 1, y + 1, 0);
}

if (magnitude >= neighbor1 && magnitude >= neighbor2)
    nms.set_pixel(x, y, 0, magnitude);
else
    nms.set_pixel(x, y, 0, 0);

            // Get the direction of the gradient at the current pixel

            // Round the direction to the nearest multiple of PI/4

            // Get the magnitude of the gradient of the two neighbors along that direction
            // (Hint: use clamped_pixel to avoid going out of bounds)            

            // If the magnitude of the gradient of the current pixel is greater than that of both neighbors,
            // then it is a local maximum
        }
    }

    return nms;
}



/*
    Applies double thresholding to an image.
    Input:
        Image im: the input image
        float lowThreshold: the low threshold value
        float highThreshold: the high threshold value
        float strongVal: the value to use for strong edges
        float weakVal: the value to use for weak edges
    Output:
        Image: the thresholded image
*/
Image double_thresholding(const Image& im, float lowThreshold, float highThreshold, float strongVal, float weakVal)
{
    Image res(im.w, im.h, im.c);

    // TODO: Your code here
        for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float val = im.clamped_pixel(x, y, 0);
        if (val >= highThreshold)
            res.set_pixel(x, y, 0, strongVal);
        else if (val >= lowThreshold)
            res.set_pixel(x, y, 0, weakVal);
        else
            res.set_pixel(x, y, 0, 0);
    }
}


    return res;
}


/*
    Applies hysteresis thresholding to an image.
    Input:
        Image im: the input image
        float weak: the value of the weak edges
        float strong: the value of the strong edges
    Output:
        Image: the image after hysteresis thresholding, with only strong edges
*/
Image edge_tracking(const Image& im, float weak, float strong)
{
    Image res(im.w, im.h, im.c);

    for (int y=0; y < im.h; ++y) {
        for (int x=0; x < im.w; ++x) {
            // TODO: Your code here
                        float val = im.clamped_pixel(x, y, 0);
if (val == weak) {
    bool connected = false;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (im.clamped_pixel(x + dx, y + dy, 0) == strong) {
                connected = true;
            }
        }
    }
    res.set_pixel(x, y, 0, connected ? strong : 0);
} else {
    res.set_pixel(x, y, 0, val);
}


            // Hint: use clamped_pixel when checking the neighbors to avoid going out of bounds
        }
    }
    return res;

}
