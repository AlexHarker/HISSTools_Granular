
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <vector>
#include "IControl.h"
#include <HISSTools_VecLib.hpp>

// Forward declaration

class HISSToolsGranular;

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE

class Waveform : public IControl
{
    
public:
    
    Waveform(HISSToolsGranular *plug, double x, double y, double w, double h) : IControl(HISSTools_Bounds(x, y, w, h)), mPlug(plug)
    {
        mData.resize(static_cast<size_t>(w * 2));
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
    
    double Normalise(double x) { return std::min(1.0, std::max(0.0, (x - mRECT.L) / mRECT.W())); }
    
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

END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE

#endif

