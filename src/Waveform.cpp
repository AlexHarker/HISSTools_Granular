
#include "Waveform.h"
#include <algorithm>
#include "HISSToolsGranular.h"

void Waveform::Draw(IGraphics& graphics)
{
    HISSTools_VecLib vecDraw(graphics);
    
    float WR = mRECT.W() / mData.size();
    float MH = mRECT.MH();
    float HH = (mRECT.H() / 2.f) * 0.9f;
    
    HISSTools_Color_Spec bgcolor(HISSTools_Color(0.8, 0.8, 0.8, 1.0));
    HISSTools_Color_Spec fgcolor(HISSTools_Color(0.1, 0.1, 0.1, 1.0));
    HISSTools_Color_Spec hlcolor(HISSTools_Color(0.8, 0.8, 0.8, 0.8));
    
    //HISSTools_Color_Spec bgcolor(HISSTools_Color(0.1, 0.1, 0.1, 1.0));
    //HISSTools_Color_Spec fgcolor(HISSTools_Color(0.9, 0.9, 0.9, 1.0));
    //HISSTools_Color_Spec hlcolor(HISSTools_Color(0.6, 0.6, 0.6, 0.8));
    
    // Background fill
    
    vecDraw.setColor(&bgcolor);
    vecDraw.fillRect(mRECT.L, mRECT.T, mRECT.W(),  mRECT.H());
    
    // Waveform
    
    if (!graphics.CheckLayer(mCache))
    {
        vecDraw.startGroup(mRECT);
        vecDraw.setColor(&fgcolor);
        vecDraw.startMultiLine(mRECT.L, mRECT.MH(), 0.5);
        
        for (int i = 0; i < mData.size(); i++)
        {
            float span = mData[i] * HH;
            float x = i * WR + mRECT.L;
            
            vecDraw.continueMultiLine(x, MH + span);
            vecDraw.continueMultiLine(x, MH - span);
        }
        
        vecDraw.finishMultiLine();
        mCache = vecDraw.endGroup();
    }
    
    if (mCache)
    {
        float width = std::max(1.f, static_cast<float>(mSelectR - mSelectL) * mRECT.W());
        
        vecDraw.renderGroup(mCache);
        
        // Selection fill
        
        vecDraw.setColor(&hlcolor);
        vecDraw.fillRect(mRECT.L + mSelectL * mRECT.W(), mRECT.T, width,  mRECT.H());
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
    
    if (mCache)
        mCache->Invalidate();
    SetDirty();
}

void Waveform::SetSelect(double L, double R)
{
    mSelectL = std::min(std::max(0.0, L), 1.0);
    mSelectR = std::min(std::max(0.0, R), 1.0);
    
    SetDirty();
}

void Waveform::OnMouseDown(float x, float y, const IMouseMod& mod)
{
    mEditDrag = mod.S;
    mEditWidth = mSelectR - mSelectL;
    
    mClickedX = x;
    mClickedY = y;
    
    OnMouseDrag(x, y, 0, 0, mod);
}

void Waveform::OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod)
{
    if (mPlug)
    {
        double ref1 = Normalise(mClickedX);
        double ref2 = Normalise(x);
        
        if (mEditDrag)
        {
            const double refHeight = mRECT.H() * 0.9;
            const double halfWidth = (mEditWidth * 0.5) * (std::max(0.0, mClickedY - y + refHeight) / refHeight);
            
            ref1 = std::max(0.0, ref2 - halfWidth);
            ref2 = std::min(1.0, ref2 + halfWidth);
        }
        
        mPlug->SelectFromGUI(ref1, ref2);
    }
}

