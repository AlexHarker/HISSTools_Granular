
#ifndef SALLENANDKEY_H
#define SALLENANDKEY_H

template <typename T> struct SallenAndKeyLPF
{
    struct Coefficients
    {
        Coefficients(const T& K = 0.0, const T& A0 = 0.0, const T& A1 = 0.0, const T& A2 = 0.0, const T& A3 = 0.0,  const T& A4 = 0.0, const T& A5 = 0.0)
        : k(K), a0(A0), a1(A1), a2(A2), a3(A3), a4(A4), a5(A5){}
        
        T k, a0, a1, a2, a3, a4, a5;
    };
    
    struct CachedCoeff
    {
        CachedCoeff() : mCutoff(0.0), mRes(0.0), mSamplingRate(0.0) {}
        
        Coefficients& operator()(const double& cutoff, const double& res, const double& samplingRate)
        {
            if (cutoff != mCutoff || res != mRes || samplingRate != mSamplingRate)
            {
                mCutoff = cutoff;
                mRes = res;
                mSamplingRate = samplingRate;
                
                mCoeff = calc(cutoff, res, samplingRate);
            }
            
            return mCoeff;
        }
        
    private:
        
        double mCutoff;
        double mRes;
        double mSamplingRate;
        
        Coefficients mCoeff;
    };
    
    SallenAndKeyLPF()
    {
        Coefficients coeff;
        
        reset();
        set(coeff);
    }
    
    inline void reset()
    {
        ic1eq = ic2eq = 0.0;
    }
    
    inline T operator()(const T& x)
    {
        T v1, v2;
        
        process(x, v1, v2);
        
        return v2;
    }
    
    inline T operator()(const T& x, Coefficients& coeff)
    {
        set(coeff);
        return operator()(x);
    }
    
    inline T nonLinear(const T& x)
    {
        T v1, v2;
        
        processNonLinear(x, v1, v2);
        
        return v2;
    }
    
    inline T nonLinear(const T& x, Coefficients& coeff)
    {
        set(coeff);
        return nonLinear(x);
    }
    
    inline void set(const Coefficients& coeff)
    {
        mCoeff = coeff;
    }
    
    static inline Coefficients calc(const double& cutoff, const double& res, const double& samplingRate)
    {
        Coefficients coeff;
        
        // N.B. - tan approximation could be different, but this might change sound design
        
        double g = tan(M_PI * (cutoff / samplingRate));
        double gp1 = 1.0 + g;
        
        coeff.k = 2.0 * res;
        coeff.a0 = 1.0 / ((gp1 * gp1) - (g * coeff.k));
        coeff.a1 = coeff.k * coeff.a0;
        coeff.a2 = gp1 * coeff.a0;
        coeff.a3 = g * coeff.a2;
        coeff.a4 = 1.0 / gp1;
        coeff.a5 = g * coeff.a4;
        
        return coeff;
    }
    
protected:
    
    inline void process(const T& x, T& v1, T& v2)
    {
        v1 = (mCoeff.a1 * ic2eq) + (mCoeff.a2 * ic1eq) + (mCoeff.a3 * x);
        v2 = (mCoeff.a4 * ic2eq) + (mCoeff.a5 * v1);
        
        update(v1, v2);
    }
    
    inline void processNonLinear(const T& x, T& v1, T& v2)
    {
        v1 = (mCoeff.a1 * ic2eq) + (mCoeff.a2 * ic1eq) + tanhLambert(mCoeff.a3 * x);
        v2 = (mCoeff.a4 * ic2eq) + tanhLambert(mCoeff.a5 * v1);
        
        update(v1, v2);
    }
    
private:
    
    inline void update(const T& v1, const T& v2)
    {
        ic1eq = (2.0 * (v1 - (mCoeff.k * v2))) - ic1eq;
        ic2eq = (2.0 * v2) - ic2eq;
    };
    
    
    Coefficients mCoeff;
    T ic1eq, ic2eq;
};


template <typename T> struct SallenAndKeyMultimode : SallenAndKeyLPF<T>
{
    void set(const double& cutoff, const double& res, const double& samplingRate)
    {
        SallenAndKeyLPF<T>::set(SallenAndKeyLPF<T>::calc(cutoff, res, samplingRate));
    }
    
    inline T lpf(const T& x)
    {
        T v1, v2;
        
        SallenAndKeyLPF<T>::process(x, v1, v2);
        
        return v2;
    }
    
    inline T bpf(const T& x)
    {
        T v1, v2;
        
        SallenAndKeyLPF<T>::process(x, v1, v2);
        
        return v1 - v2;
    }
    
    inline T hpf(const T& x)
    {
        T v1, v2;
        
        SallenAndKeyLPF<T>::process(x, v1, v2);
        
        return x - v2;
    }
};

#endif /* SallenAndKey_h */

