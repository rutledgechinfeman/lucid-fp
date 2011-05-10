/**
  Vector constructs and utilities.

  author - psastras

**/

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <math.h>

#define SAFE_DELETE(x) if((x)) { delete (x); (x) = NULL; }
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define PI 3.14159265358979323846


struct double3 {


    double3(double v0 = 0, double v1 = 0, double v2 = 0) : x(v0), y(v1), z(v2) { }
    double3(double *data) { x = data[0]; y = data[1]; z = data[2]; }

    static inline double3 zero() { return double3(0,0,0); }

    #define VECOP_PCW(op) { x op rhs.x; y op rhs.y; z op rhs.z; return *this; }
    #define VECOP_SCA(op) { x op rhs;   y op rhs  ; z op rhs; return *this; }

    inline double3& operator  = (const double3& rhs) VECOP_PCW( =) /// equality assignment
    inline double3& operator += (const double3& rhs) VECOP_PCW(+=) /// piecewise addition operator
    inline double3& operator -= (const double3& rhs) VECOP_PCW(-=) /// piecewise subtraction operator


    inline double3  operator  + (const double3& rhs) const { return double3(*this) += rhs; } /// piecewise addition
    inline double3  operator  - (const double3& rhs) const { return double3(*this) -= rhs; } /// piecewise subtraction

    inline double3& operator += (const double  rhs)  VECOP_SCA(+=) /// scalar addition operator
    inline double3& operator -= (const double  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
    inline double3& operator *= (const double  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
    inline double3& operator /= (const double  rhs)  VECOP_SCA(/=) /// scalar division operator

    inline double3  operator  + (const double  rhs) const { return double3(*this) += rhs; } /// piecewise addition
    inline double3  operator  - (const double  rhs) const { return double3(*this) -= rhs; } /// piecewise subtraction
    inline double3  operator  * (const double  rhs) const { return double3(*this) *= rhs; } /// piecewise multiplication
    inline double3  operator  / (const double  rhs) const { return double3(*this) /= rhs; } /// piecewise multiplication

    #undef VECOP_PCW
    #undef VECOP_SCA


    inline double magnitude2() { return x*x + y*y + z*z; }
    inline double magnitude() { return sqrtf(magnitude2()); }
    inline double3& normalize() { return *this /= magnitude(); }
    inline double3 getNormalized() { return double3(*this).normalize(); }

    inline bool operator==(const double3 &rhs) {
            return (x == rhs.x && y == rhs.y && z == rhs.z );
    }

    inline bool operator!=(const double3 &rhs) {
            return (x != rhs.x || y != rhs.y || z != rhs.z);
    }

    union {
        struct {
            double x, y, z;
        };
        struct {
            double s, t, r;
        };
        double data[3];
    };
};


inline double3 operator*(const double scale, const double3 &rhs) {
    return double3(rhs.x * scale, rhs.y * scale, rhs.z * scale);
}

inline double3 operator/(const double scale, const double3 &rhs) {
    return double3(scale / rhs.x, scale / rhs.y , scale / rhs.z);
}

inline double3 operator-(const double3 &rhs) {
    return double3(-rhs.x, -rhs.y, -rhs.z);
}

inline std::ostream& operator<<(std::ostream& os, const double3& f) {
        os <<"[";
        for (unsigned i = 0; i < 3; ++i) {
            os << f.data[i] << ",";
        }
        os << "]";
        return os;
}

struct int2 {
    int2(double v0, double v1) { x = (int)(v0+0.5); y = (int)(v1+0.5); }
    int2(int v0 = 0, int v1 = 0) : x(v0), y(v1) { }
    int2(int *data) { x = data[0]; y = data[1]; }

    static inline int2 zero() { return int2(0,0); }

    #define VECOP_PCW(op) { x op rhs.x; y op rhs.y; return *this; }
    #define VECOP_SCA(op) { x op rhs;   y op rhs  ; return *this; }

    inline int2& operator  = (const int2& rhs) VECOP_PCW( =) /// equality assignment
    inline int2& operator += (const int2& rhs) VECOP_PCW(+=) /// piecewise addition operator
    inline int2& operator -= (const int2& rhs) VECOP_PCW(-=) /// piecewise subtraction operator


    inline int2  operator  + (const int2& rhs) const { return int2(*this) += rhs; } /// piecewise addition
    inline int2  operator  - (const int2& rhs) const { return int2(*this) -= rhs; } /// piecewise subtraction

    inline int2& operator += (const int  rhs)  VECOP_SCA(+=) /// scalar addition operator
    inline int2& operator -= (const int  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
    inline int2& operator *= (const int  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
    inline int2& operator /= (const int  rhs)  VECOP_SCA(/=) /// scalar division operator

    inline int2  operator  + (const int  rhs) const { return int2(*this) += rhs; } /// piecewise addition
    inline int2  operator  - (const int  rhs) const { return int2(*this) -= rhs; } /// piecewise subtraction
    inline int2  operator  * (const int  rhs) const { return int2(*this) *= rhs; } /// piecewise multiplication
    inline int2  operator  / (const int  rhs) const { return int2(*this) /= rhs; } /// piecewise multiplication

    inline bool operator==(const int2 &rhs) {
            return (x == rhs.x && y == rhs.y);
    }

    inline bool operator!=(const int2 &rhs) {
            return (x != rhs.x || y != rhs.y);
    }

    union {
        struct {
            int x, y;
        };
        struct {
            int s, t;
        };
        int data[2];
    };
};

struct double2 {
    double2(int2 in){x = (double)in.x; y = (double)in.y; }

    double2(double v0 = 0, double v1 = 0) : x(v0), y(v1) { }
    double2(double *data) { x = data[0]; y = data[1]; }

    static inline double2 zero() { return double2(0,0); }

    #define VECOP_PCW(op) { x op rhs.x; y op rhs.y; return *this; }
    #define VECOP_SCA(op) { x op rhs;   y op rhs  ; return *this; }

    inline double2& operator  = (const double2& rhs) VECOP_PCW( =) /// equality assignment
    inline double2& operator += (const double2& rhs) VECOP_PCW(+=) /// piecewise addition operator
    inline double2& operator -= (const double2& rhs) VECOP_PCW(-=) /// piecewise subtraction operator


    inline double2  operator  + (const double2& rhs) const { return double2(*this) += rhs; } /// piecewise addition
    inline double2  operator  - (const double2& rhs) const { return double2(*this) -= rhs; } /// piecewise subtraction

    inline double2& operator += (const double  rhs)  VECOP_SCA(+=) /// scalar addition operator
    inline double2& operator -= (const double  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
    inline double2& operator *= (const double  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
    inline double2& operator /= (const double  rhs)  VECOP_SCA(/=) /// scalar division operator

    inline double2  operator  + (const double  rhs) const { return double2(*this) += rhs; } /// piecewise addition
    inline double2  operator  - (const double  rhs) const { return double2(*this) -= rhs; } /// piecewise subtraction
    inline double2  operator  * (const double  rhs) const { return double2(*this) *= rhs; } /// piecewise multiplication
    inline double2  operator  / (const double  rhs) const { return double2(*this) /= rhs; } /// piecewise multiplication

    #undef VECOP_PCW
    #undef VECOP_SCA


    inline double magnitude2() { return x*x + y*y; }
    inline double magnitude() { return sqrtf(magnitude2()); }

    inline bool operator==(const double2 &rhs) {
            return (x == rhs.x && y == rhs.y);
    }

    inline bool operator!=(const double2 &rhs) {
            return (x != rhs.x || y != rhs.y);
    }

    union {
        struct {
            double x, y;
        };
        struct {
            double s, t;
        };
        double data[2];
    };
};

inline double2 operator*(const double scale, const double2 &rhs) {
    return double2(rhs.x * scale, rhs.y * scale);
}
inline double2 operator-(const double2 &rhs) {
    return double2(-rhs.x, -rhs.y);
}

inline std::ostream& operator<<(std::ostream& os, const double2& f) {
        os <<"[";
        for (unsigned i = 0; i < 2; ++i) {
            os << f.data[i] << ",";
        }
        os << "]";
        return os;
}

#endif //COMMON_H
