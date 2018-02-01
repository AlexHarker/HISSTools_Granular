
#ifndef GRAIN_H
#define GRAIN_H

#include <cmath>
#include <vector>
#include <algorithm>

#include "Buffer.h"
#include "SallenAndKey.h"
#include "FrameLib_RandGen.h"
#include "IPlugStructs.h"
#include "Waveform.h"

// Filter Class

class Filter
{
    
public:
    
    enum Type { kNone, kLPF, kHPF, kBPF };
    
    Filter() : mType(kNone) {}
    
    void init(Type type, double freq, double res, double samplingRate);
    void process(double* io, int numSamps);
    
private:
    
    Type mType;
    SallenAndKeyMultimode<double> mFilter;
};

// Nonlinear Class

class Nonlinear
{
    
public:
    
    enum Type { kNone, kTanh, kSimple, kCubic, kSoft, kPoly };
    
    Nonlinear() : mType(kTanh), mGain(1.0) {}
    
    void init(Type type, double gain);
    void process(double* io, int numSamps);
    
private:
    
    Type mType;
    double mGain;
};

// Panner Class

class Panner
{
    
public:
    
    Panner() : mLL(1.0), mRL(0.0), mRR(1.0), mLR(0.0) {}
    
    void init(double pan);
    void process(double *ioL, double* ioR, double numSamps);
    
    double mLL;
    double mRL;
    double mLR;
    double mRR;
};

class Window : private Buffer<double>
{
    const static int sLength = 4096;
    
public:
 
    enum Type { kHann, kTriangle, kCosine, kKaiser, kTukey };
    
    Window() { set(kHann); }
    
    void set(Type type);
    void read(double* io, int numSamps, double amp)     { Buffer::read(io, io, numSamps, amp); }
    int length()                                        { return sLength; }
};

// Stereo Buffer

class StereoBuffer
{
    
public:
    
    StereoBuffer() : mSampleRate(44100.0) {}
    
    void load(const char *path);
    void read(double* output, double *positions, int numSamps, int chan);
    void clear();

    double getDuration()        { return mBuffers[0].getSize() / mSampleRate; }
    double getSampleRate()      { return mSampleRate; }
    
    void save(IByteChunk &storage);
    int recall(IByteChunk &storage, int pos);
    
    void setWaveformL(Waveform *waveform)    { waveform->Set(mBuffers[0].data(), mBuffers[0].getSize()); }
    void setWaveformR(Waveform *waveform)    { waveform->Set(mBuffers[1].data(), mBuffers[1].getSize()); }

private:
    
    double mSampleRate;
    Buffer<float> mBuffers[2];
};

// Grain Class

class Grain
{
    
public:
    
    Grain();
    
    void init(StereoBuffer *buffer,
              Window *window,
              double windowPow,
              double offset,
              double phaseIncr,
              double offsetIncr,
              double offsetIncrMul,
              double amp,
              Nonlinear::Type distType,
              double gain,
              Filter::Type filterType,
              double freq,
              double res,
              double pan,
              double samplingRate);
    
    int process(double* outputL, double* outputR, double* tempVector, int maxSamps);
    
private:
    
    StereoBuffer *mBuffer;
    Window *mWindow;
    Panner mPanner;
    Nonlinear mNonlinear;
    Filter mFilters[2];
    
    double mWindowPow;
    bool mWindowBiasDirection;
    
    double mPhase;
    double mPhaseIncr;
    
    double mOffsetIncr;
    double mOffsetIncrMul;
    double mOffset;
        
    double mAmp;
};

// Granular Class

class Granular
{
    struct Bounds
    {
        Bounds() {}
        Bounds(double lo, double hi) : mLo(lo), mHi(hi) {}
        
        double mLo;
        double mHi;
    };
    
public:
    
    Granular();
    
    void setOffset(double relative, double addition)
    {
        double lo = relative * mBuffer.getDuration();
        mOffset = Bounds(lo, lo + addition);
    }
    
    void setDuration(double lo, double hi) { mDuration = Bounds(log(lo), log(hi)); }
    void setPitch(double lo, double hi) { mPitch = Bounds(lo, hi); }
    void setGliss(double lo, double hi) { mGliss = Bounds(lo, hi); }
    void setVol(double lo, double hi) { mVol = Bounds(lo, hi); }
    void setPan(double lo, double hi) { mPan = Bounds(lo, hi); }
    void setWindowBias(double lo, double hi) { mWindowBias = Bounds(lo, hi); }
    void setDrive(double lo, double hi) { mDrive = Bounds(lo, hi); }
    void setFilterFreq(double center, double variation) { mFilterFreq = Bounds(center, variation); }
    void setFilterResonance(double lo, double hi) { mFilterResonance = Bounds(lo, hi); }
    
    void setDensity(double density)                  { mDensity = density; }
    void setWindowType(Window::Type type)            { mWindowType = type; }
    void setFilterType(Filter::Type type)            { mFilterType = type; }
    void setDistortionType(Nonlinear::Type type)     { mNonlinearType = type; }
    
    void load(const char* path)
    {
        mBuffer.load(path);
    }

    void setWaveformL(Waveform *waveform)    { mBuffer.setWaveformL(waveform); }
    void setWaveformR(Waveform *waveform)    { mBuffer.setWaveformR(waveform); }
 
    double getBufferDuration() { return mBuffer.getDuration(); }
    
    bool save(IByteChunk& chunk);
    int recall(IByteChunk& chunk, int pos);

    
    double dbToA(double dB)
    {
        return exp(0.11512925464970 * dB);
    }
    
    double pitchToSpeed(double pitch)
    {
        return exp2(pitch / 12.0);
    }
    
    double calcOffset(double time)
    {
        return time * mBuffer.getSampleRate();
    }
    
    double calcOffsetIncrMul(double glissDepth, double sampleRate)
    {
        double speedMul = pitchToSpeed(glissDepth);
        
        return pow(speedMul, 1.0 / sampleRate);
    }

    double calcPhaseIncr(double duration, double sampleRate)
    {
        return 1.0 / (mBuffer.getSampleRate() * duration);
    }
    
    double calcOffsetIncr(double speed, double sampleRate)
    {
        return speed * (mBuffer.getSampleRate() / sampleRate);
    }
    
    double randomBounds(Bounds bounds)
    {
        return mGen.randDouble(bounds.mLo, bounds.mHi);
    }
    
    double getDuration();
    void silentGrain(std::vector<Grain>::iterator grain, double sampleRate);
    void initGrain(std::vector<Grain>::iterator grain, double sampleRate);
    void reset(double sampleRate);
    void processBlock(double* outputL, double* outputR, int numSamps, double sampleRate);
    
private:
    
    double mDensity;
    Window::Type mWindowType;
    Filter::Type mFilterType;
    Nonlinear::Type mNonlinearType;
    
    FrameLib_RandGen mGen;
    
    StereoBuffer mBuffer;
    Window mHannWindow;
    Window mTriWindow;
    Window mCosineWindow;
    Window mKaiserWindow;
    Window mTukeyWindow;
    
    Bounds mWindowBias;
    Bounds mDuration;
    Bounds mPan;
    Bounds mFilterFreq;
    Bounds mFilterResonance;
    Bounds mVol;
    Bounds mDrive;
    Bounds mPitch;
    Bounds mGliss;
    Bounds mOffset;
    
    std::vector<double> tempL;
    std::vector<double> tempR;
    std::vector<double> tempCalc;
    
    std::vector<Grain> mGrains;
};

#endif /* GRAIN_H */
