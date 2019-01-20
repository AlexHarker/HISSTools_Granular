
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <vector>
#include "IControl.h"

// Forward declaration

class HISSToolsGranular;

class Waveform : public IControl
{
    
public:
    
    Waveform(HISSToolsGranular *plug, double x, double y, double w, double h) : IControl(IRECT(x, y, x + w, y + h)), mPlug(plug)
    {
        mData.resize(w * 2);
        SetSelect(0., 0.);
    }
    
    Waveform(const Waveform&) = delete;
    Waveform operator = (const Waveform&) = delete;
    
    void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override;
    
    void Draw(IGraphics& graphics) override;
    
    void Set(const float *data, int dataSize);
    void SetSelect(double L, double R);
    
private:
    
    double Normalise(float x) { return std::min(1.f, std::max(0.f, (x - mRECT.L) / mRECT.W())); }
    
    HISSToolsGranular *mPlug;
    std::vector<float> mData;
    double mSelectL;
    double mSelectR;
    double mClickedX;
    double mClickedY;
    double mEditWidth;
    bool mEditDrag;
    ILayerPtr mCache;
};

#endif
