
#ifndef GRAIN_H
#define GRAIN_H

#include <cassert>

#include <cmath>
#include <vector>
#include <algorithm>

#include "List.h"
#include "Buffer.h"
#include "SallenAndKey.h"
#include "HISSTools_Library/RandomGenerator.hpp"

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
    
    int getLength() const           { return mBuffers[0].getSize(); }
    double getDuration() const      { return getLength() / mSampleRate; }
    double getSampleRate() const    { return mSampleRate; }
    
    const float *getL() const       { return mBuffers[0].data(); }
    const float *getR() const       { return mBuffers[1].data(); }
    
    void save(iplug::IByteChunk &storage) const;
    int recall(const iplug::IByteChunk &storage, int pos);
    
private:
    
    double mSampleRate;
    Buffer<float> mBuffers[2];
};

// Grain Class

class Grain : public List<Grain>::Listable
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
    
    enum GranMode { kStream, kCloud };
    
    Granular();
    
    // N.B. offset and filter freq do further internal conversion
    
    void setRate(double lo, double hi) { mRate = Bounds(log(lo), log(hi)); }
    void setOffset(double relative, double addition) { mOffset = Bounds(relative, addition); }
    void setDuration(double lo, double hi) { mDuration = Bounds(log(lo), log(hi)); }
    void setPitch(double lo, double hi) { mPitch = Bounds(lo, hi); }
    void setGliss(double lo, double hi) { mGliss = Bounds(lo, hi); }
    void setVol(double lo, double hi) { mVol = Bounds(lo, hi); }
    void setPan(double lo, double hi) { mPan = Bounds(lo, hi); }
    void setWindowBias(double lo, double hi) { mWindowBias = Bounds(lo, hi); }
    void setDrive(double lo, double hi) { mDrive = Bounds(lo, hi); }
    void setFilterFreq(double center, double variation) { mFilterFreq = Bounds(center, variation); }
    void setFilterResonance(double lo, double hi) { mFilterResonance = Bounds(lo, hi); }
    
    void setMode(GranMode mode)                      { mMode = mode; }
    void setMaxVoices(int maxVoices)                 { mMaxVoices = maxVoices; }
    void setDensity(double density)                  { mDensity = density; }
    void setWindowType(Window::Type type)            { mWindowType = type; }
    void setFilterType(Filter::Type type)            { mFilterType = type; }
    void setDistortionType(Nonlinear::Type type)     { mNonlinearType = type; }
    
    void load(const char* path)
    {
        mBuffer.load(path);
    }
    
    const float* getBufferL()   { return mBuffer.getL(); }
    const float* getBufferR()   { return mBuffer.getR(); }
    
    int getBufferLength() { return mBuffer.getLength(); }
    double getBufferDuration() { return mBuffer.getDuration(); }
    
    bool save(iplug::IByteChunk& chunk) const;
    int recall(const iplug::IByteChunk& chunk, int pos);
    
    void reset(double sampleRate);
    void processBlock(double* outputL, double* outputR, int numSamps, double sampleRate);
    
private:
    
    double chooseDuration();
    void silentGrain(Grain& grain, double lengthMul, double sampleRate);
    void initGrain(Grain& grain, bool forceSilent, double sampleRate);
    
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
        return 1.0 / (sampleRate * duration);
    }
    
    double calcOffsetIncr(double speed, double sampleRate)
    {
        return speed * (mBuffer.getSampleRate() / sampleRate);
    }
    
    double randomBounds(Bounds bounds)
    {
        return mGen.rand_double(bounds.mLo, bounds.mHi);
    }
    
    void setActive(Grain& grain);
    void setFree(Grain& grain);
    int processGrain(Grain& grain, double* outputL, double* outputR, int samps);
    
    GranMode mMode;
    
    double mCloudTillNext;
    
    double mDensity;
    Window::Type mWindowType;
    Filter::Type mFilterType;
    Nonlinear::Type mNonlinearType;
    
    random_generator<> mGen;
    
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
    Bounds mRate;
    
    int mMaxVoices;
    
    std::vector<double> mTempL;
    std::vector<double> mTempR;
    std::vector<double> mTempCalc;
    
    std::vector<Grain> mGrains;
    List<Grain> mActiveList;
    List<Grain> mFreeList;
};

#endif /* GRAIN_H */

