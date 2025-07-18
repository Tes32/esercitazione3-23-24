// COPY YOUR FILE FROM THE PREVIOUS ASSIGNMENT HERE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image &im) {

    // TODO: Normalize each channel
    int size = im.w * im.h;
    for (int c = 0; c < im.c; ++c) {
        float sum = 0.0f;
        for (int i = 0; i < size; ++i) {
            sum += im.data[c * size + i];
        }
        if (sum != 0.0f) {
            for (int i = 0; i < size; ++i) {
                im.data[c * size + i] /= sum;
            }
        }
    }
}



// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w) {
    assert(w % 2); // w needs to be odd

    // TODO: Implement the filter

    Image filter(w, w, 1);

    float value = 1.0f;
    for (int y = 0; y < w; ++y) {
        for (int x = 0; x < w; ++x) {
            filter(x, y, 0) = value;
        }
    }

    l1_normalize(filter);
    return filter;
}


// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    //Image ret;
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds

    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the convolution operator
    

    // Make sure to return ret and not im. This is just a placeholder

    int fw = filter.w;
    int fh = filter.h;
    int pad_x = fw / 2;
    int pad_y = fh / 2;

    int out_c = preserve ? im.c : 1;
    Image ret(im.w, im.h, out_c);

    for (int c = 0; c < im.c; ++c) {
        for (int y = 0; y < im.h; ++y) {
            for (int x = 0; x < im.w; ++x) {
                float sum = 0.0f;

                for (int fy = 0; fy < fh; ++fy) {
                    for (int fx = 0; fx < fw; ++fx) {
                        int ix = x + fx - pad_x;
                        int iy = y + fy - pad_y;

                        float im_val = im.clamped_pixel(ix, iy, c);
                        float filter_val = filter(fx, fy, 0);

                        sum += im_val * filter_val;
                    }
                }

                if (preserve) {
                    ret(x, y, c) = sum;
                } else {
                    ret(x, y, 0) += sum;
                }
            }
        }
    }
    clamp_image(ret);

    return ret;
}


// HW1 #2.2+ Fast convolution
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image_fast(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    Image ret;
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds

    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the fast convolution operator. Remember to use Eigen for matrix operations
      return convolve_image(im, filter, preserve);


    // Make sure to return ret and not im. This is just a placeholder
    return im;
}


// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter() {
    // TODO: Implement the filter
    Image filter(3, 3, 1);

    filter(0, 0, 0) = 0;  filter(1, 0, 0) = -1; filter(2, 0, 0) = 0;
    filter(0, 1, 0) = -1; filter(1, 1, 0) =  4; filter(2, 1, 0) = -1;
    filter(0, 2, 0) = 0;  filter(1, 2, 0) = -1; filter(2, 2, 0) = 0;

    return filter;
}




// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter() {
    // TODO: Implement the filter
    Image filter(3, 3, 1);

    filter(0, 0, 0) = 0;  filter(1, 0, 0) = -1; filter(2, 0, 0) = 0;
    filter(0, 1, 0) = -1; filter(1, 1, 0) =  5; filter(2, 1, 0) = -1;
    filter(0, 2, 0) = 0;  filter(1, 2, 0) = -1; filter(2, 2, 0) = 0;

    return filter;
}


// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter() {
    // TODO: Implement the filter
    
    Image filter(3, 3, 1);

    filter(0, 0, 0) = -2; filter(1, 0, 0) = -1; filter(2, 0, 0) = 0;
    filter(0, 1, 0) = -1; filter(1, 1, 0) =  1; filter(2, 1, 0) = 1;
    filter(0, 2, 0) = 0;  filter(1, 2, 0) =  1; filter(2, 2, 0) = 2;

    return filter;
}

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma) {
    // TODO: Implement the filter
    int size = static_cast<int>(ceil(6 * sigma));
    if (size % 2 == 0) size += 1;

    Image filter(size, size, 1);
    int center = size / 2;
    float sum = 0.0f;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int dx = x - center;
            int dy = y - center;
            float value = exp(-(dx * dx + dy * dy) / (2 * sigma * sigma));
            filter(x, y, 0) = value;
            sum += value;
        }
    }

    for (int i = 0; i < size * size; ++i) {
        filter.data[i] /= sum;
    }

    return filter;
}



// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    // TODO: Implement addition
    Image result(a.w, a.h, a.c);

    int size = a.w * a.h * a.c;
    for (int i = 0; i < size; ++i) {
        result.data[i] = a.data[i] + b.data[i];
    }
    clamp_image(result);
    return result;
}

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size

    // TODO: Implement subtraction
    Image result(a.w, a.h, a.c);

    int size = a.w * a.h * a.c;
    for (int i = 0; i < size; ++i) {
        result.data[i] = a.data[i] - b.data[i];
    }
    clamp_image(result);
    return result;
}

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter() {
    // TODO: Implement the filter
    Image filter(3, 3, 1);

    filter(0, 0, 0) = -1; filter(1, 0, 0) = 0; filter(2, 0, 0) = 1;
    filter(0, 1, 0) = -2; filter(1, 1, 0) = 0; filter(2, 1, 0) = 2;
    filter(0, 2, 0) = -1; filter(1, 2, 0) = 0; filter(2, 2, 0) = 1;

    return filter;
}
// HW1 #4.1
// returns basic GY filter
Image make_gy_filter() {
    // TODO: Implement the filter
    Image filter(3, 3, 1);

    filter(0, 0, 0) = -1; filter(1, 0, 0) = -2; filter(2, 0, 0) = -1;
    filter(0, 1, 0) =  0; filter(1, 1, 0) =  0; filter(2, 1, 0) =  0;
    filter(0, 2, 0) =  1; filter(1, 2, 0) =  2; filter(2, 2, 0) =  1;

    return filter;
}

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image &im) {
    assert(im.w * im.h); // assure we have non-empty image

    // TODO: Normalize the features for each channel
    NOT_IMPLEMENTED();

}


// Normalizes features across all channels
void feature_normalize_total(Image &im) {
    assert(im.w * im.h * im.c); // assure we have non-empty image

    int nc = im.c;
    im.c = 1;
    im.w *= nc;

    feature_normalize(im);

    im.w /= nc;
    im.c = nc;

}


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image, Image> sobel_image(const Image &im) {
    // TODO: Your code here
    Image gx = convolve_image(im, make_gx_filter(), true);
    Image gy = convolve_image(im, make_gy_filter(), true);

    Image magnitude(im.w, im.h, 1);
    Image direction(im.w, im.h, 1);

    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            float gx_val = gx(x, y, 0);
            float gy_val = gy(x, y, 0);

            magnitude(x, y, 0) = sqrt(gx_val * gx_val + gy_val * gy_val);
            direction(x, y, 0) = atan2(gy_val, gx_val);
        }
    }

    return {magnitude, direction};
}

// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image &im) {

    // TODO: Your code here
    pair<Image, Image> sobel = sobel_image(im);
Image mag = sobel.first;
Image dir = sobel.second;

    feature_normalize(mag);
    feature_normalize(dir);


    for (int y = 0; y < dir.h; ++y) {
        for (int x = 0; x < dir.w; ++x) {
            dir(x, y, 0) = dir(x, y, 0) / (2 * M_PI) + 0.5f;
        }
    }

    Image hsv(im.w, im.h, 3);
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            hsv(x, y, 0) = dir(x, y, 0); // hue → visualizzato come rosso
            hsv(x, y, 1) = mag(x, y, 0); // saturation → verde
            hsv(x, y, 2) = mag(x, y, 0); // value → blu
        }
    }

    // Applichiamo un blur per renderla più gradevole
    Image blurred = convolve_image(hsv, make_gaussian_filter(4), true);
    return blurred;
}



// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image &im, float sigma1, float sigma2) {
    Image bf = im;

    // TODO: Your bilateral code
    int size = static_cast<int>(ceil(6 * sigma1));
if (size % 2 == 0) size += 1;
int half = size / 2;

bf = Image(im.w, im.h, im.c);

for (int c = 0; c < im.c; ++c) {
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            float norm = 0.0f;
            float val = 0.0f;
            float center = im(x, y, c);

            for (int fy = -half; fy <= half; ++fy) {
                for (int fx = -half; fx <= half; ++fx) {
                    int ix = x + fx;
                    int iy = y + fy;

                    float spatial = exp(-(fx * fx + fy * fy) / (2 * sigma1 * sigma1));
                    float range = exp(-pow(im.clamped_pixel(ix, iy, c) - center, 2) / (2 * sigma2 * sigma2));
                    float weight = spatial * range;

                    val += weight * im.clamped_pixel(ix, iy, c);
                    norm += weight;
                }
            }

            bf(x, y, c) = val / norm;
        }
    }
}


    return bf;
}

// HW1 #4.5+ Fast bilateral filter
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter_fast(const Image &im, float sigma1, float sigma2) {
    Image bf = im;

    // TODO: Your fast bilateral code
    bf = bilateral_filter(im, sigma1, sigma2);


    return bf;
}

// HM #5
//
float *compute_histogram(const Image &im, int ch, int num_bins) {
    float *hist = (float *) malloc(sizeof(float) * num_bins);
    for (int i = 0; i < num_bins; ++i) {
        hist[i] = 0;
    }

    // TODO: Your histogram code
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            float val = im(x, y, ch);
            int bin = static_cast<int>(val * num_bins);
            if (bin >= num_bins) bin = num_bins - 1;
            hist[bin]++;
        }
    }

    return hist;
}

float *compute_CDF(float *hist, int num_bins) {
    float *cdf = (float *) malloc(sizeof(float) * num_bins);

    cdf[0] = hist[0];

    // TODO: Your cdf code
    for (int i = 1; i < num_bins; ++i) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    float total = cdf[num_bins - 1];
    for (int i = 0; i < num_bins; ++i) {
        cdf[i] /= total;
    }

    return cdf;
}

Image histogram_equalization_hsv(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // TODO: Your histogram equalization code
    Image luminance(im.w, im.h, 1);
for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float r = im(x, y, 0);
        float g = im(x, y, 1);
        float b = im(x, y, 2);
        luminance(x, y, 0) = (r + g + b) / 3.0f;
    }
}

float *hist = compute_histogram(luminance, 0, num_bins);
float *cdf = compute_CDF(hist, num_bins);

for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float val = luminance(x, y, 0);
        int bin = static_cast<int>(val * num_bins);
        if (bin >= num_bins) bin = num_bins - 1;
        float new_val = cdf[bin];
        for (int c = 0; c < im.c; ++c) {
            new_im(x, y, c) = new_val;
        }
    }
}

free(hist);
free(cdf);
    // convert to hsv
    // compute histograms for the luminance channel
    // compute cdf
    // equalization
    // convert back to rgb

    // delete the allocated memory!
//    delete hist;
//    delete cdf;

    return new_im;
}

Image histogram_equalization_rgb(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // compute histograms for each color channel
    for (int c = 0; c < im.c; ++c) {

        // TODO: Your equalization code
        float *hist = compute_histogram(im, c, num_bins);
float *cdf = compute_CDF(hist, num_bins);

for (int y = 0; y < im.h; ++y) {
    for (int x = 0; x < im.w; ++x) {
        float val = im(x, y, c);
        int bin = static_cast<int>(val * num_bins);
        if (bin >= num_bins) bin = num_bins - 1;
        new_im(x, y, c) = cdf[bin];
    }
}

free(hist);
free(cdf);

        NOT_IMPLEMENTED();

        // delete the allocated memory!
//        delete hist;
//        delete cdf;
    }

    return new_im;
}


// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }

void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }

void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image &a, const Image &b) { return sub_image(a, b); }

Image operator+(const Image &a, const Image &b) { return add_image(a, b); }
