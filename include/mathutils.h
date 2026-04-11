#ifndef UTILS_H
#define UTILS_H

#include <math.h>

/* Absolute Value */
static inline int i_abs(int v) { return v < 0 ? -v : v; }
static inline float f_abs(float v) { return fabsf(v); }
static inline double d_abs(double v) { return fabs(v); }

#define abs(v) _Generic((v), \
    int: i_abs,                \
    float: f_abs,              \
    double: d_abs              \
)(v)

/* Min / Max */
static inline int i_min(int a, int b) { return a < b ? a : b; }
static inline int i_max(int a, int b) { return a > b ? a : b; }

static inline float f_min(float a, float b) { return fminf(a, b); }
static inline float f_max(float a, float b) { return fmaxf(a, b); }

static inline double d_min(double a, double b) { return fmin(a, b); }
static inline double d_max(double a, double b) { return fmax(a, b); }

#define min(a, b) _Generic((a), \
    int: i_min,                   \
    float: f_min,                 \
    double: d_min                  \
)(a, b)

#define max(a, b) _Generic((a), \
    int: i_max,                   \
    float: f_max,                 \
    double: d_max                  \
)(a, b)

/* Clamp */
static inline int i_clamp(int v, int lo, int hi) {
    return i_min(i_max(v, lo), hi);
}

static inline float f_clamp(float v, float lo, float hi) {
    return fminf(fmaxf(v, lo), hi);
}

static inline double d_clamp(double v, double lo, double hi) {
    return fmin(fmax(v, lo), hi);
}

#define clamp(v, lo, hi) _Generic((v), \
    int: i_clamp,                        \
    float: f_clamp,                      \
    double: d_clamp                      \
)(v, lo, hi)

#endif // UTILS_H