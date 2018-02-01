
#include "Waveform.h"
#include <algorithm>

void Waveform::Draw(IGraphics& graphics)
{
    float WR = mRECT.W() / mData.size();
    float MH = mRECT.MH();
    float HH = (mRECT.H() / 2.0) * 0.9;
    
    mVecLib->setIGraphics(&graphics);
    
    HISSTools_Color_Spec bgcolor(HISSTools_Color(0.8, 0.8, 0.8, 1.0));
    HISSTools_Color_Spec fgcolor(HISSTools_Color(0.1, 0.1, 0.1, 1.0));
    HISSTools_Color_Spec hlcolor(HISSTools_Color(0.8, 0.8, 0.8, 0.8));

    //HISSTools_Color_Spec bgcolor(HISSTools_Color(0.1, 0.1, 0.1, 1.0));
    //HISSTools_Color_Spec fgcolor(HISSTools_Color(0.9, 0.9, 0.9, 1.0));
    //HISSTools_Color_Spec hlcolor(HISSTools_Color(0.6, 0.6, 0.6, 0.8));
    
    // Background fill
    
    mVecLib->setColor(&bgcolor);
    mVecLib->fillRect(mRECT.L, mRECT.T, mRECT.W(),  mRECT.H());
    
    // Waveform
    
    if (mWaveformNeedsRedraw)
    {
        mVecLib->startGroup();
        mVecLib->setColor(&fgcolor);
        mVecLib->startMultiLine(mRECT.L, mRECT.MH(), 0.5);
    
        for (int i = 0; i < mData.size(); i++)
        {
            float span = mData[i] * HH;
            float x = i * WR + mRECT.L;
        
            mVecLib->continueMultiLine(x, MH + span);
            mVecLib->continueMultiLine(x, MH - span);
        }
        
        mVecLib->finishMultiLine();
        mCache = mVecLib->endGroup();
        mWaveformNeedsRedraw = false;
    }
    
    if (mCache)
    {
        float width = std::max(1.0, (mSelectR - mSelectL) * mRECT.W());
        
        mVecLib->renderPattern(mCache);
    
        // Selection fill
    
        mVecLib->setColor(&hlcolor);
        mVecLib->fillRect(mRECT.L + mSelectL * mRECT.W(), mRECT.T, width,  mRECT.H());
    }
}
    
void Waveform::Set(const float *data, int dataSize)
{
    double pos = 0.0;
    double incr = double(dataSize) / mData.size();
    int j = 0;
    
    // Store waveform overview
    
    for (int i = 0; i < mData.size(); i++)
    {
        float val = 0.0;
        int dataTo = std::min(dataSize, int(round(pos)));
        
        for (; j < dataTo; j++)
            val = std::max(val, fabsf(data[j]));
        
        mData[i] = val;
        pos += incr;
    }
    
    mWaveformNeedsRedraw = true;
    SetDirty();
}

void Waveform::SetSelect(double L, double R)
{
    mSelectL = std::min(std::max(0.0, L), 1.0);
    mSelectR = std::min(std::max(0.0, R), 1.0);
    
    SetDirty();
}

