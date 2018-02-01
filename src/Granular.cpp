
#include "Granular.h"
#include "Nonlinear.h"
#include "IAudioFile.h"
#include "WindowFunctions.hpp"

// Filter Class

void Filter::init(Type type, double freq, double res, double samplingRate)
{
    mType = type;
    mFilter.reset();
    mFilter.set(freq, res, samplingRate);
}

void Filter::process(double* io, int numSamps)
{
    switch(mType)
    {
        case kNone:
            break;
        case kLPF:
            for (int i = 0; i < numSamps; i++)
                io[i] = mFilter.lpf(io[i]);
            break;
        case kHPF:
            for (int i = 0; i < numSamps; i++)
                io[i] = mFilter.hpf(io[i]);
            break;
        case kBPF:
            for (int i = 0; i < numSamps; i++)
                io[i] = mFilter.bpf(io[i]);
            break;
    }
}

// Nonlinear Class

void Nonlinear::init(Type type, double gain)
{
    mType = type;
    mGain = gain;
}

void Nonlinear::process(double* io, int numSamps)
{
    switch(mType)
    {
        case kNone:
            break;
        case kTanh:
            for (int i = 0; i < numSamps; i++)
                io[i] = tanhLambert(io[i] * mGain);
            break;
        case kSimple:
            for (int i = 0; i < numSamps; i++)
                io[i] = simpSat(io[i] * mGain);
            break;
        case kCubic:
            for (int i = 0; i < numSamps; i++)
                io[i] = cnl(io[i] * mGain);
            break;
        case kSoft:
            for (int i = 0; i < numSamps; i++)
                io[i] = softStatic(io[i] * mGain);
            break;
        case kPoly:
            for (int i = 0; i < numSamps; i++)
                io[i] = polySaturate(io[i] * mGain);
            break;
    }
}

// Panner Class

void scale(double x, double& a, double& b)
{
    double scaleFactor = ( a || b) ? x / (a + b) : 0.0;
    a *= scaleFactor;
    b *= scaleFactor;
}
     
void Panner::init(double pan)
{
    double c = cos(pan * M_PI * 0.5) * 2.0 * M_SQRT1_2;
    double s = sin(pan * M_PI * 0.5) * 2.0 * M_SQRT1_2;
    
    pan *= 2.0;
    
    mLL = std::min(2.0 - pan, 1.0);
    mRL = std::max(1.0 - pan, 0.0);
    scale(c, mLL, mRL);
   
    mLR = std::max(pan - 1.0, 0.0);
    mRR = std::min(pan, 1.0);
    scale(s, mLR, mRR);
}

void Panner::process(double *ioL, double* ioR, double numSamps)
{
    for (int i = 0; i < numSamps; i++)
    {
        double L = ioL[i];
        double R = ioR[i];
        
        ioL[i] = L * mLL + R * mRL;
        ioR[i] = R * mRR + L * mLR;
    }
}

// Window Class

IndexedWindowFunctions<double* > mWindowFunc;

void Window::set(Type type)
{
    IndexedWindowFunctions<double*>::WindowTypes indexType = IndexedWindowFunctions<double*>::kWindowHann;
    
    switch(type)
    {
        case kTriangle:     indexType = IndexedWindowFunctions<double*>::kWindowTriangle;   break;
        case kHann:         indexType = IndexedWindowFunctions<double*>::kWindowHann;       break;
        case kCosine:       indexType = IndexedWindowFunctions<double*>::kWindowCosine;     break;
        case kKaiser:       indexType = IndexedWindowFunctions<double*>::kWindowKaiser;     break;
        case kTukey:        indexType = IndexedWindowFunctions<double*>::kWindowTukey;      break;
    }
    
    setInterpType(kInterpLinear);
    resize(sLength + 2);
    mWindowFunc.calculate(indexType, data(), 4096, 4097);
    
    for (int i = 0; i < 4097; i++)
        data()[i] = sqrt(data()[i]);
}

// Stereo Buffer

void StereoBuffer::load(const char *path)
{
    HISSTools::IAudioFile file(path);
    
    if (!file.isOpen())
    {
        clear();
        mSampleRate = 44100.0;
        return;
    }
    
    int length = file.getFrames();
    
    mBuffers[0].resize(length);
    mBuffers[1].resize(length);
    
    file.readChannel(mBuffers[0].data(), length, 0);
    file.seek();
    
    if (file.getChannels() > 1)
        file.readChannel(mBuffers[1].data(), length, 1);
    else
        mBuffers[1] = mBuffers[0];
    
    mSampleRate = file.getSamplingRate();
}

void StereoBuffer::read(double* output, double *positions, int numSamps, int chan)
{
    chan = std::max(0, std::min(chan, 1));
    mBuffers[chan].read(output, positions, numSamps, 1.0);
}

void StereoBuffer::clear()
{
    mBuffers[0].clear();
    mBuffers[1].clear();
}

void StereoBuffer::save(IByteChunk &storage)
{
    storage.Put(&mSampleRate);
    mBuffers[0].save(storage);
    mBuffers[1].save(storage);
}

int StereoBuffer::recall(IByteChunk &storage, int pos)
{
    pos = storage.Get(&mSampleRate, pos);
    
    pos = mBuffers[0].recall(storage, pos);
    pos = mBuffers[1].recall(storage, pos);

    return pos;
}

// Grain Class

Grain::Grain()
{
    mBuffer = NULL;
    mWindow = NULL;
    mPhase = 1.0;
    mPhaseIncr = 1.0;
    mOffset = 0.0;
    mOffsetIncr = 0.0;
    mAmp = 0.0;
}

void Grain::init(StereoBuffer *buffer,
                 Window *window,
                 double windowFactor,
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
                 double samplingRate)
{
    mBuffer = buffer;
    mWindow = window;
    mWindowPow = fabs(windowFactor);
    mWindowBiasDirection = windowFactor > 0;
    mPhase = 0.0;
    mPhaseIncr = phaseIncr;
    mOffset = offset;
    mOffsetIncr = offsetIncr;
    mOffsetIncrMul = offsetIncrMul;
    mAmp = amp;
    mNonlinear.init(distType, gain);
    mFilters[0].init(filterType, freq, res, samplingRate);
    mFilters[1] = mFilters[0];
    mPanner.init(pan);
}

int Grain::process(double* outputL, double* outputR, double* tempVector, int maxSamps)
{
    // Calculate drive position and end
    
    int numSamps = std::min(maxSamps, int(ceil((1.0 - mPhase) / mPhaseIncr)));
    
    if (!numSamps || mOffset < 0.0 || !mBuffer || !mWindow)
    {
        mPhase += mPhaseIncr * numSamps;
        
        std::fill_n(outputL, numSamps, 0.0);
        std::fill_n(outputR, numSamps, 0.0);
        
        return numSamps;
    }

    // Construct offset vector
    
    for (int i = 0; i < numSamps; i++)
    {
        tempVector[i] = mOffset;
        mOffset += mOffsetIncr;
        mOffsetIncr *= mOffsetIncrMul;
    }
    
    // TODO: Wobble/Varispeed
    
    // Playback
    
    mBuffer->read(outputL, tempVector, numSamps, 0);
    mBuffer->read(outputR, tempVector, numSamps, 1);
    
    mNonlinear.process(outputL, numSamps);
    mNonlinear.process(outputR, numSamps);
    
    // Construct Phase Vector
    
    double windowLength = mWindow->length();
    
    for (int i = 0; i < numSamps; i++)
    {
        const double windowPhase = mWindowBiasDirection ? pow(mPhase, mWindowPow) : 1.0 - pow(1.0 - mPhase, mWindowPow);
        tempVector[i] = windowPhase * windowLength;
        mPhase += mPhaseIncr;
    }
    
    if (mOffset < 0.0)
        return numSamps;
    
    // Window (once pre-filter and once post)
    
    mWindow->read(tempVector, numSamps, mAmp);
    
    for (int i = 0; i < numSamps; i++)
    {
        double scale = tempVector[i];
        outputL[i] *= scale;
        outputR[i] *= scale;
    }
    
    mFilters[0].process(outputL, numSamps);
    mFilters[1].process(outputR, numSamps);
    
    for (int i = 0; i < numSamps; i++)
    {
        double scale = tempVector[i];
        outputL[i] *= scale;
        outputR[i] *= scale;
    }
    
    mPanner.process(outputL, outputR, numSamps);
    
    return numSamps;
}

// Granular Class

Granular::Granular()
{
    mGrains.resize(100);
    
    mHannWindow.set(Window::kHann);
    mTriWindow.set(Window::kTriangle);
    mCosineWindow.set(Window::kCosine);
    mKaiserWindow.set(Window::kKaiser);
    mTukeyWindow.set(Window::kTukey);
    
    setDensity(0.7);
    setWindowType(Window::kHann);
    setFilterType(Filter::kNone);
    setDistortionType(Nonlinear::kNone);
    
    setRate(0.01, 0.01);
    setDuration(0.3, 0.7);
    setWindowBias(-2, -1.5);
    setOffset(0.0, 1.0);
    setPitch(0.0, 0.0);
    setGliss(0.1, 0.1);
    setVol(-10.0, 0.0);
    setDrive(0.0, 10.0);
    setPan(0.3, 0.7);
    setFilterFreq(-48, 48);
    setFilterResonance(0.1, 0.9);
    mMaxVoices = 70.0;
}

double Granular::chooseDuration()
{
    return exp(randomBounds(mDuration));
}

void Granular::setActive(Grain& grain)
{
    mFreeList.remove(&grain);
    mActiveList.add(&grain);
}

void Granular::setFree(Grain& grain)
{
    mActiveList.remove(&grain);
    mFreeList.add(&grain);
}

void Granular::silentGrain(Grain& grain, double lengthMul, double sampleRate)
{
    double phaseIncr = calcPhaseIncr(chooseDuration() * lengthMul, sampleRate);
    
    setFree(grain);
    grain.init(&mBuffer, &mHannWindow, 1.0, -1.0, phaseIncr, 0.0, 0.0, 0.0, Nonlinear::kNone, 0.0, Filter::kNone, 100.0, 0.0, 0.5, sampleRate);
}

void Granular::initGrain(Grain& grain, bool forceSilent, double sampleRate)
{
    // Variables
    
    if (forceSilent || mGen.randDouble() > mDensity)
    {
        silentGrain(grain, 1.0, sampleRate);
        return;
    }
    
    setActive(grain);

    const double offsetLo = mOffset.mLo * mBuffer.getDuration();
    const double offsetHi = offsetLo + mOffset.mHi;
    
    const double duration = chooseDuration();
    const double windowBias = randomBounds(mWindowBias);
    const double time = randomBounds(Bounds(offsetLo, offsetHi));
    const double pitch = randomBounds(mPitch);
    const double glissSpeed = randomBounds(mGliss);
    const double volDB = randomBounds(mVol);
    const double distDB = randomBounds(mDrive);
    const double pan = randomBounds(mPan);
    const double freq = std::min(mFilterFreq.mLo * pitchToSpeed(mGen.randDouble(-1.0, 1.0) * mFilterFreq.mHi), 20000.0);
    const double resParam = randomBounds(mFilterResonance);
    
    // Engine Values
    
    const double windowFactor = copysign(exp2(fabs(windowBias)), windowBias);
    const double speed = pitchToSpeed(pitch);
    const double offset = calcOffset(time);
    const double phaseIncr = calcPhaseIncr(duration, sampleRate);
    const double offsetIncr = calcOffsetIncr(speed, sampleRate);
    const double offsetIncrMul = calcOffsetIncrMul(glissSpeed, sampleRate);
    const double amp = dbToA(volDB);
    const double gain = dbToA(distDB);
    const double res = sqrt(resParam);
    
    Window* window = &mHannWindow;

    switch (mWindowType)
    {
        case Window::kHann:         window = &mHannWindow;      break;
        case Window::kTriangle:     window = &mTriWindow;       break;
        case Window::kCosine:       window = &mCosineWindow;    break;
        case Window::kKaiser:       window = &mKaiserWindow;    break;
        case Window::kTukey:        window = &mTukeyWindow;     break;
    }
    
    grain.init(&mBuffer, window, windowFactor, offset, phaseIncr, offsetIncr, offsetIncrMul, amp, mNonlinearType, gain, mFilterType, freq, res, pan, sampleRate);
}

void Granular::reset(double sampleRate)
{
    mActiveList.clear();
    mFreeList.clear();
    
    for (auto it = mGrains.begin(); it != mGrains.end(); it++)
        silentGrain(*it, mGen.randDouble(), sampleRate);
    
    mCloudTillNext = 0.0;
}

int Granular::processGrain(Grain& grain, double* outputL, double* outputR, int samps)
{
    int done = grain.process(mTempL.data(), mTempR.data(), mTempCalc.data(), samps);
    
    for (int i = 0; i < done; i++)
    {
        outputL[i] += mTempL[i];
        outputR[i] += mTempR[i];
    }
    
    return done;
}

void Granular::processBlock(double* outputL, double* outputR, int numSamps, double sampleRate)
{
    if (!numSamps)
        return;
    
    if (numSamps > mTempL.size())
    {
        mTempL.resize(numSamps);
        mTempR.resize(numSamps);
        mTempCalc.resize(numSamps);
    }
    
    std::fill_n(outputL, numSamps, 0.0);
    std::fill_n(outputR, numSamps, 0.0);
    
    switch (mMode)
    {
        case kStream:
        {
            for (auto it = mGrains.begin(); it != mGrains.end(); it++)
            {
                int samps = numSamps;
                
                while (samps)
                {
                    samps -= processGrain(*it, outputL + (numSamps - samps), outputR + (numSamps - samps), samps);
                    
                    if (samps)
                        initGrain(*it,  (it - mGrains.begin()) >= mMaxVoices, sampleRate);
                }
            }
        }
        break;
        
        case kCloud:
        {
            int samps = numSamps;

            while (samps)
            {
                int loopSize = std::min(samps, int(ceil(mCloudTillNext)));

                for (auto it = mActiveList.begin(); it != mActiveList.end(); )
                {
                    Grain& grain= **it++;
                    if (processGrain(grain, outputL + (numSamps - samps), outputR + (numSamps - samps), loopSize) != loopSize)
                        silentGrain(grain, 1.0, sampleRate);
                }
                
                mCloudTillNext -= loopSize;
                samps -= loopSize;
                
                // Check for new grain
                
                if (mCloudTillNext <= 0.0)
                {
                    mCloudTillNext += exp(randomBounds(mRate)) * sampleRate;
                    
                    if (!mFreeList.empty() && mActiveList.size() < mMaxVoices)
                        initGrain(**mFreeList.begin(), false, sampleRate);
                }
            }
        
        }
        break;
    }
}

bool Granular::save(IByteChunk& chunk)
{
    mBuffer.save(chunk);
    return true;
}

int Granular::recall(IByteChunk& chunk, int pos)
{
    return mBuffer.recall(chunk, pos);
}
