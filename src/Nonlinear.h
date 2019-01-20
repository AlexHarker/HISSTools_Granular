
#ifndef NONLINEAR_H
#define NONLINEAR_H

// Nonlinearities

template <typename T> T clip(T x, T lo, T hi) { return std::min(hi, std::max(lo, x)); }

template <typename T> inline T tanhLambert(const T& x)
{
    T x2 = x * x;
    T a = (((x2 + 378.0) * x2 + 17325.0) * x2 + 135135.0) * x;
    T b = ((28.0 * x2 + 3150.0) * x2 + 62370.0) * x2 + 135135.0;
    T div = a / b;
    return clip(div, -1.0, 1.0);
}

template <typename T> inline T simpSat(const T& x)
{
    T xc = clip(x, -1.0, 1.0);
    
    return xc * (3.0 - (xc * xc)) * 0.5;
}

template <typename T> inline T cnl(const T& x)
{
    T xc = clip(x, -1.0, 1.0);
    
    return xc * (1.0 - (0.33333333 * xc * xc));
}

template <typename T> inline T softStatic(const T& x)
{
    T xa = fabs(x);
    T nx = xa > 1.0 ? (1.0 - 4.0 / (xa + 3.0)) * 4.0 + 1.0 : xa;
    
    return copysign(nx, x);
}

template <typename T> inline T polySaturate(const T& x)
{
    T x2 = x * x;
    T x3 = x * x2;
    T y = x - (0.18963 * x3) + (0.016182 * x3 * x2);
    
    return clip(y, -1.0, 1.0);
}
#endif /* SallenAndKey_h */

