
#include "HISSToolsGranular.h"
#include "IPlug_include_in_plug_src.h"
#include "HISSTools_Controls.hpp"

// Visual Design

class Design : public HISSTools_Design_Scheme
{
  
public:
  
  Design() : HISSTools_Design_Scheme(true)
  {
    HISSTools_Color_Spec *col1 = new HISSTools_Color_Spec(0.7, 0.0, 0.0, 0.9);
    HISSTools_Color_Spec *col2 = new HISSTools_Color_Spec(0.2, 0.7, 0.3, 0.9);
    HISSTools_Color_Spec *col3 = new HISSTools_Color_Spec(0.35, 0.0, 0.35, 0.9);
    HISSTools_Color_Spec *col4 = new HISSTools_Color_Spec(0.0, 0.5, 0.5, 0.9);
    HISSTools_Color_Spec *col5 = new HISSTools_Color_Spec(0.5, 0.5, 0.0, 0.9);
    HISSTools_Color_Spec *col6 = new HISSTools_Color_Spec(0.1, 0.4, 0.7, 0.9);
    HISSTools_Color_Spec *col7 = new HISSTools_Color_Spec(0.8, 0.4, 0.1, 0.9);
    
    HISSTools_Color_Spec *blackCS = new HISSTools_Color_Spec(0., 0., 0., 1.0);
    HISSTools_Color_Spec *greyCS = new HISSTools_Color_Spec(0.9, 0.9, 0.9, 0.5);
    
    HISSTools_LICE_VGradient* activeFillCS = new HISSTools_LICE_VGradient;
    HISSTools_LICE_VGradient* activeOffFillCS = new HISSTools_LICE_VGradient;
    
    activeOffFillCS->addStop(HISSTools_Color(0.415, 0.415, 0.415, 1.0), 0.0);
    activeOffFillCS->addStop(HISSTools_Color(0.169, 0.169, 0.169, 1.0), 1.0);
    
    activeFillCS->addStop(HISSTools_Color(0.6, 0.6, 0.6, 1.0), 0);
    activeFillCS->addStop(HISSTools_Color(0.3, 0.3, 0.3, 1.0), 1.);
    
    HISSTools_LICE_VGradient* panelFillCS = new HISSTools_LICE_VGradient;
    panelFillCS->addStop(HISSTools_Color(0.4, 0.4, 0.4, 0.4), 0.0);
    panelFillCS->addStop(HISSTools_Color(0.2, 0.2, 0.2, 0.5), 0.94);
    panelFillCS->addStop(HISSTools_Color(0.075, 0.075, 0.075, 0.6), 1.0);
    
    HISSTools_Color_Spec *shadowCS = new HISSTools_Color_Spec(HISSTools_Color(0.00, 0.00, 0.00, 0.90));
    HISSTools_Shadow *shadowSpec = new HISSTools_Shadow(shadowCS, 4, 4, 6);
    
    addColorSpec("PanelFill", "upper", panelFillCS);
    addColorSpec("PanelFill", "main", panelFillCS);
    addColorSpec("PanelFill", "thick", blackCS);
    
    addColorSpec("ButtonHandleLabelOff", "alt", greyCS);
    addColorSpec("ButtonHandleOff", "alt", activeOffFillCS);
    addColorSpec("ButtonHandleOn", "alt", activeFillCS);
    
    addShadow("TextBlock", "name", shadowSpec);
    
    HISSTools_Color_Spec *textColor = new HISSTools_Color_Spec(0.9, 0.9, 0.9, 0.80);
    HISSTools_Text *nameTxt = new HISSTools_Text(42, "Arial Bold");
    
    addColorSpec("TextBlock", "name", textColor);
    addTextStyle("TextBlock", "name", nameTxt);
    
    addColorSpec("DialIndicator", "1", col1);
    addColorSpec("DialIndicator", "2", col2);
    addColorSpec("DialIndicator", "3", col3);
    addColorSpec("DialIndicator", "4", col4);
    addColorSpec("DialIndicator", "5", col5);
    addColorSpec("DialIndicator", "6", col6);
    addColorSpec("DialIndicator", "7", col7);
    
    addDimension("PanelRoundnessTL","tighter", 5);
    addDimension("PanelRoundnessTR","tighter", 5);
    addDimension("PanelRoundnessBL","tighter", 5);
    addDimension("PanelRoundnessBR","tighter", 5);
    
    addDimension("DialRefValue", "gain", 2.0/7.0);
    addDimension("DialRefValue", "vol", 6.0/7.0);
    
    addDimension("ValueTextArea", "spacious", 25);
    
    addFlag("ValueDrawTriangle", "small", false);
    addFlag("ValueDrawLabel", "nolabel", false);
    
    addFlag("ValueLabelBelow", true);
    addFlag("ValueLabelBelow", "above", false);
    addFlag("DialDrawValOnlyOnMO", true);
  }
};

Design designScheme;

class HISSTools_GFileSelector : public HISSTools_FileSelector
{
  
public:
  
  HISSTools_GFileSelector(HISSToolsGranular *plug, double x, double y, double w, double h, EFileAction action, char* dir = "", char* extensions = "", const char *type = 0, HISSTools_Design_Scheme *designScheme = &DefaultDesignScheme)
  :HISSTools_FileSelector(kNoParameter, x, y, w, h, action, dir, extensions, type, designScheme, "Select"), mPlug(plug) {}
  
private:
  
  virtual void reportToPlug() override
  {
    mPlug->SelectFile(GetLastSelectedFileForPlug().Get());
  }
  
  HISSToolsGranular *mPlug;
};

void HISSToolsGranular::AddDualControl(IGraphics* graphics, double x, double y, int idx, int idxRand, const char *options)
{
  WDL_String val(options);
  WDL_String var("small");
  
  var.Append(" ");
  var.Append(options);
  
  graphics->AttachControl(new HISSTools_Dial(idx, x, y, val.Get(), &designScheme));
  graphics->AttachControl(new HISSTools_Dial(idxRand, x + kSmallDialXOffset, y + kSmallDialYOffset, var.Get(), &designScheme));
}

void HISSToolsGranular::AddBiPolarDualControl(IGraphics* graphics, double x, double y, int idx, int idxRand, const char *options, const char *mainOptions)
{
  WDL_String val(mainOptions);
  WDL_String var("small");
  
  val.Append(" ");
  val.Append(options);
  val.Append(" bipolar");
  
  var.Append(" ");
  var.Append(options);
  
  graphics->AttachControl(new HISSTools_Dial(idx, x, y, val.Get(), &designScheme));
  graphics->AttachControl(new HISSTools_Dial(idxRand, x + kSmallDialXOffset, y + kSmallDialYOffset, var.Get(), &designScheme));
}

HISSToolsGranular::HISSToolsGranular(const InstanceInfo &info)
: Plugin(info, MakeConfig(kNumParams, kNumPrograms)), mWaveformL(nullptr), mWaveformR(nullptr)
{
  TRACE;
  
  // Parameter Setup
  
  GetParam(kActive)->InitBool("Active", true);
  
  GetParam(kMode)->InitEnum("Mode", 0, 2);
  GetParam(kMode)->SetDisplayText(0, "Streams");
  GetParam(kMode)->SetDisplayText(1, "Clouds");
  
  GetParam(kDensity)->InitDouble("Density", 100.0, 0.0, 100.0, 0.1, "%", 0, "", IParam::ShapePowCurve(2.0));
  
  GetParam(kMaxVoices)->InitInt("Max Voices", 50, 1, 100);
  
  GetParam(kRate)->InitDouble("Rate", 10.0, 0.1, 2000.0, 0.1, "ms", 0, "", IParam::ShapePowCurve(2.0));
  GetParam(kRateRand)->InitDouble("Rand Rate", 10.0, 0.0, 4000.0, 0.1, "ms", 0, "", IParam::ShapePowCurve(2.0));
  
  GetParam(kOffset)->InitDouble("Offset", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kOffsetRand)->InitDouble("Rand Offset", 200.0, 0.0, 10000.0, 0.1, "ms", 0, "", IParam::ShapePowCurve(2.0));
  
  GetParam(kDuration)->InitDouble("Duration", 100.0, 0.1, 5000.0, 0.1, "ms", 0, "", IParam::ShapePowCurve(2.0));
  GetParam(kDurationRand)->InitDouble("Rand Dur", 20.0, 0.0, 4000.0, 0.1, "ms", 0, "", IParam::ShapePowCurve(2.0));
  
  GetParam(kPitch)->InitDouble("Pitch", 0.0, -36.0, 36.0, 0.01, "st");
  GetParam(kPitchRand)->InitDouble("Rand Pitch", 0.0, 0.0, 48.0, 0.01, "st", 0, "", IParam::ShapePowCurve(2.0));
  
  GetParam(kGliss)->InitDouble("Gliss Speed", 0.0, -36.0, 36, 0.01, "st/s");
  GetParam(kGlissRand)->InitDouble("Rand Gliss", 0.0, 0.0, 36.0, 0.01, "st/s");
  
  GetParam(kVol)->InitDouble("Volume", -15.0, -60.0, 10.0, 0.1, "dB");
  GetParam(kVolRand)->InitDouble("Rand Vol", 5.0, 0.0, 20.0, 0.1, "dB");
  
  GetParam(kPan)->InitDouble("Pan", 0.0, -100.0, 100.0, 0.1, "");
  GetParam(kPanRand)->InitDouble("Rand Pan", 20.0, 0.0, 100.0, 0.1, "");
  
  GetParam(kWindowType)->InitEnum("Window Type", 0, 5);
  GetParam(kWindowType)->SetDisplayText(0, "Hann");
  GetParam(kWindowType)->SetDisplayText(1, "Triangle");
  GetParam(kWindowType)->SetDisplayText(2, "Cosine");
  GetParam(kWindowType)->SetDisplayText(3, "Kaiser");
  GetParam(kWindowType)->SetDisplayText(4, "Tukey");
  
  GetParam(kWindowBias)->InitDouble("Window Bias", 0.0, -100.0, 100.0, 0.1, "%");
  GetParam(kWindowBiasRand)->InitDouble("Rand Bias", 0.0, 0.0, 100.0, 0.1, "%");
  
  GetParam(kDistortionType)->InitEnum("Distortion Type", 0, 6);
  GetParam(kDistortionType)->SetDisplayText(0, "Off");
  GetParam(kDistortionType)->SetDisplayText(1, "Tanh");
  GetParam(kDistortionType)->SetDisplayText(2, "Simple");
  GetParam(kDistortionType)->SetDisplayText(3, "Cubic");
  GetParam(kDistortionType)->SetDisplayText(4, "Soft");
  GetParam(kDistortionType)->SetDisplayText(5, "Poly");
  
  GetParam(kGain)->InitDouble("Drive", 0.0, -20.0, 50.0, 0.1, "dB");
  GetParam(kGainRand)->InitDouble("Rand Drive", 5.0, 0.0, 20.0, 0.1, "dB");
  
  GetParam(kFilterType)->InitEnum("Filter Type", 0, 4);
  GetParam(kFilterType)->SetDisplayText(0, "Off");
  GetParam(kFilterType)->SetDisplayText(1, "LPF");
  GetParam(kFilterType)->SetDisplayText(2, "HPF");
  GetParam(kFilterType)->SetDisplayText(3, "BPF");
  
  GetParam(kFilterFreq)->InitDouble("Filter Freq", 500.0, 20.0, 16000.0, 0.1, "Hz", 0, "", IParam::ShapeExp());
  GetParam(kFilterFreqRand)->InitDouble("Rand Freq", 48.0, 0.0, 48.0, 0.1, "st");
  
  GetParam(kFilterResonance)->InitDouble("Filter Reson", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kFilterResonanceRand)->InitDouble("Rand Reson", 10.0, 0.0, 100.0, 0.1, "%");
  
  MakeDefaultPreset("-", kNumPrograms);
}

HISSToolsGranular::~HISSToolsGranular() {}

void HISSToolsGranular::ProcessBlock(double** inputs, double** outputs, int nFrames)
{
  WDL_MutexLock lock(&mMutex);
  
  mGranular.processBlock(outputs[0], outputs[1], nFrames, GetSampleRate());
}

IGraphics* HISSToolsGranular::CreateGraphics()
{
  return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, 60, 1.);
}

void HISSToolsGranular::LayoutUI(IGraphics* pGraphics)
{
  if (!pGraphics->NControls())
  {
    pGraphics->LoadFont("Arial Bold", "Arial", ETextStyle::Bold);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    
    // Controls
    
    // Panels
    
    pGraphics->AttachControl(new HISSTools_Panel(kPanel1X, kPanel1Y, kPanel2W, kPanel1H, "upper tight", &designScheme));
    pGraphics->AttachControl(new HISSTools_Panel(kPanel2X, kPanel2Y, kPanel2W, kPanel2H, "main tight", &designScheme));
    pGraphics->AttachControl(new HISSTools_Panel(kPanel3X, kPanel3Y, kPanel3W, kPanel3H, "tighter thick", &designScheme));
    
    // Main Controls
    
    const double wX = kWaveformX + kWaveformOutline;
    const double wW = kWaveformW - (2 * kWaveformOutline);
    const double wH = kWaveformH - kWaveformOutline;
    const double w1Y = kWaveformY + kWaveformOutline;
    const double w2Y = w1Y + wH;
    
    mWaveformL = new Waveform(this, wX, w1Y, wW, wH);
    mWaveformR = new Waveform(this, wX, w2Y, wW, wH);
    
    // Voice Control
    
    pGraphics->AttachControl(new HISSTools_Value(kMode, kVoiceColX, kVoiceRow1Y, kValueWL, kValueHL, "above spacious", &designScheme));
    pGraphics->AttachControl(new HISSTools_Value(kMaxVoices, kVoiceColX, kVoiceRow2Y, kValueWL, kValueHL, "above spacious", &designScheme));
    pGraphics->AttachControl(new HISSTools_Value(kDensity, kVoiceColX, kVoiceRow3Y, kValueWL, kValueHL, "above spacious", &designScheme));
    
    // Rate
    
    AddDualControl(pGraphics, kRateActiveX, kRateY, kRate, kRateRand, "7");
    pGraphics->AttachControl(new HISSTools_Button(kActive, kRateActiveX, kBeneathWavefromY, kActiveW, kValueH, "alt tight spacious", &designScheme));
    
    pGraphics->AttachControl(new HISSTools_GFileSelector(this, kWaveformX, kBeneathWavefromY, kWaveformW, kValueH, EFileAction::Open, "", "aif aiff wav", "tight", &designScheme));
    pGraphics->AttachControl(mWaveformL);
    pGraphics->AttachControl(mWaveformR);
    
    // Dials
    
    // Top Row
    
    AddDualControl(pGraphics, kCol1X, kRow1Y, kOffset, kOffsetRand, "1");
    AddDualControl(pGraphics, kCol2X, kRow1Y, kDuration, kDurationRand, "1");
    AddBiPolarDualControl(pGraphics, kCol3X, kRow1Y, kPitch, kPitchRand, "2");
    AddBiPolarDualControl(pGraphics, kCol4X, kRow1Y, kGliss, kGlissRand, "2");
    AddBiPolarDualControl(pGraphics, kCol5X, kRow1Y, kPan, kPanRand, "6");
    AddBiPolarDualControl(pGraphics, kCol5X, kRow2Y, kVol, kVolRand, "6", "vol");
    
    // Bottom Row
    
    AddBiPolarDualControl(pGraphics, kCol1X, kRow2Y, kWindowBias, kWindowBiasRand, "3");
    pGraphics->AttachControl(new HISSTools_Value(kWindowType, kCol1X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
    
    AddBiPolarDualControl(pGraphics, kCol2X, kRow2Y, kGain, kGainRand, "4", "gain");
    pGraphics->AttachControl(new HISSTools_Value(kDistortionType, kCol2X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
    
    AddDualControl(pGraphics, kCol3X, kRow2Y, kFilterFreq, kFilterFreqRand, "5");
    pGraphics->AttachControl(new HISSTools_Value(kFilterType, kCol3X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
    AddDualControl(pGraphics, kCol4X, kRow2Y, kFilterResonance, kFilterResonanceRand, "5");
    
    // Name
    
    pGraphics->AttachControl(new HISSTools_TextBlock(kNameX, kNameY, kNameW, kNameH, "HISSTools Granular", kHAlignCenter, kVAlignCenter, "name", &designScheme));
    
    mWaveformL->Set(mGranular.getBufferL(), mGranular.getBufferLength());
    mWaveformR->Set(mGranular.getBufferR(), mGranular.getBufferLength());
    
    // Finalise Graphics
    
    pGraphics->HandleMouseOver(true);
    //pGraphics->SetStrictDrawing(false);
  }
}

void HISSToolsGranular::SelectFromGUI(double click, double drag)
{
  const double dragLimit = std::min(fabs(click - drag) * 1000.0 * mGranular.getBufferDuration(), GetParam(kOffsetRand)->GetMax());
  const double lo = click > drag ? click - (dragLimit / (mGranular.getBufferDuration() * 1000.0)) : click;
  const double offsetRand = GetParam(kOffsetRand)->ToNormalized(dragLimit);
  SendParameterValueFromUI(kOffset, lo);
  SendParameterValueFromUI(kOffsetRand, offsetRand);
  SendParameterValueFromAPI(kOffset, lo, true);
  SendParameterValueFromAPI(kOffsetRand, offsetRand, true);
}

void HISSToolsGranular::GUIUpdateSelection()
{
  if (!GetUI())
    return;
  
  double L = GetParam(kOffset)->Value() / 100.0;
  double R = L + (GetParam(kOffsetRand)->Value() / (1000.0 * mGranular.getBufferDuration()));
  
  mWaveformL->SetSelect(L, R);
  mWaveformR->SetSelect(L, R);
}

void HISSToolsGranular::GUIGrayOutControl(int paramIdx, bool gray)
{
  if (GetUI())
    GetUI()->DisableControl(paramIdx, gray);
}

void HISSToolsGranular::OnParamChange(int paramIdx, EParamSource source, int sampleOffset)
{
  WDL_MutexLock lock(&mMutex);
  
  switch (paramIdx)
  {
    case kDensity:
    case kActive:
      mGranular.setDensity(GetParam(kActive)->Value() * (GetParam(kDensity)->Value() / 100.0));
      break;
      
    case kMode:
      mGranular.setMode((Granular::GranMode) GetParam(kMode)->Int());
      break;
      
    case kMaxVoices:
      mGranular.setMaxVoices(GetParam(kMaxVoices)->Int());
      break;
      
    case kRate:
    case kRateRand:
    {
      double lo = GetParam(kRate)->Value();
      double hi = lo + GetParam(kRateRand)->Value();
      mGranular.setRate(lo / 1000.0, hi / 1000.0);
    }
    break;
      
    case kOffset:
    case kOffsetRand:
    {
      double lo = GetParam(kOffset)->Value() / 100.0;
      double hi = GetParam(kOffsetRand)->Value() / 1000.0;
      mGranular.setOffset(lo, hi);
    }
    break;
      
    case kDuration:
    case kDurationRand:
    {
      double lo = GetParam(kDuration)->Value();
      double hi = lo + GetParam(kDurationRand)->Value();
      mGranular.setDuration(lo / 1000.0, hi / 1000.0);
    }
    break;
      
    case kPitch:
    case kPitchRand:
    {
      double center = GetParam(kPitch)->Value();
      double variation = GetParam(kPitchRand)->Value();
      double lo = center - variation;
      double hi = center + variation;
      mGranular.setPitch(lo, hi);
    }
    break;
      
    case kGliss:
    case kGlissRand:
    {
      double center = GetParam(kGliss)->Value();
      double variation = GetParam(kGlissRand)->Value();
      double lo = center - variation;
      double hi = center + variation;
      mGranular.setGliss(lo, hi);
    }
    break;
      
    case kVol:
    case kVolRand:
    {
      double center = GetParam(kVol)->Value();
      double variation = GetParam(kVolRand)->Value();
      double lo = center - variation;
      double hi = center + variation;
      mGranular.setVol(lo, hi);
    }
    break;
      
    case kPan:
    case kPanRand:
    {
      double center = GetParam(kPan)->GetNormalized();
      double variation = GetParam(kPanRand)->GetNormalized();
      double lo = std::max(0.0, std::min(center - variation, 1.0));
      double hi = std::max(0.0, std::min(center + variation, 1.0));
      mGranular.setPan(lo, hi);
    }
    break;
      
    case kWindowType:
      mGranular.setWindowType((Window::Type) GetParam(kWindowType)->Int());
      break;
      
    case kWindowBias:
    case kWindowBiasRand:
    {
      double center = GetParam(kWindowBias)->GetNormalized();
      double variation = GetParam(kWindowBiasRand)->GetNormalized();
      double lo = std::max(0.0, std::min(center - variation, 1.0)) * 4.0 - 2.0;
      double hi = std::max(0.0, std::min(center + variation, 1.0)) * 4.0 - 2.0;
      mGranular.setWindowBias(lo, hi);
    }
    break;
      
    case kDistortionType:
      mGranular.setDistortionType((Nonlinear::Type) GetParam(kDistortionType)->Int());
      break;
      
    case kGain:
    case kGainRand:
    {
      double center = GetParam(kGain)->Value();
      double variation = GetParam(kGainRand)->Value();
      double lo = center - variation;
      double hi = center + variation;
      mGranular.setDrive(lo, hi);
    }
    break;
      
    case kFilterType:
      mGranular.setFilterType((Filter::Type) GetParam(kFilterType)->Int());
      break;
      
    case kFilterFreq:
    case kFilterFreqRand:
    {
      double center = GetParam(kFilterFreq)->Value();
      double variation = GetParam(kFilterFreqRand)->Value();
      mGranular.setFilterFreq(center, variation);
    }
    break;
      
    case kFilterResonance:
    case kFilterResonanceRand:
    {
      double center = GetParam(kFilterResonance)->GetNormalized();
      double variation = GetParam(kFilterResonanceRand)->GetNormalized();
      double lo = std::max(0.0, std::min(center - variation, 0.9995));
      double hi = std::max(0.0, std::min(center + variation, 0.9995));
      mGranular.setFilterResonance(lo, hi);
    }
      break;
  }
}


void HISSToolsGranular::OnParamChangeUI(int paramIdx, EParamSource source)
{
  switch (paramIdx)
  {
    case kMode:
      GUIGrayOutControl(kRate, !GetParam(kMode)->Int());
      GUIGrayOutControl(kRateRand, !GetParam(kMode)->Int());
      break;
      
    case kOffset:
    case kOffsetRand:
      GUIUpdateSelection();
      break;
      
    case kDistortionType:
      GUIGrayOutControl(kGain, !GetParam(kDistortionType)->Int());
      GUIGrayOutControl(kGainRand, !GetParam(kDistortionType)->Int());
      break;
      
    case kFilterType:
      GUIGrayOutControl(kFilterFreq, !GetParam(kFilterType)->Int());
      GUIGrayOutControl(kFilterFreqRand, !GetParam(kFilterType)->Int());
      GUIGrayOutControl(kFilterResonance, !GetParam(kFilterType)->Int());
      GUIGrayOutControl(kFilterResonanceRand, !GetParam(kFilterType)->Int());
      break;
      
    default:
      break;
  }
}

void HISSToolsGranular::SelectFile(const char *file)
{
  WDL_MutexLock lock(&mMutex);
  
  WDL_String str(file);
  
  mGranular.load(str.Get());
  mWaveformL->Set(mGranular.getBufferL(), mGranular.getBufferLength());
  mWaveformR->Set(mGranular.getBufferR(), mGranular.getBufferLength());
  GUIUpdateSelection();
}

void HISSToolsGranular::OnReset()
{
  mGranular.reset(GetSampleRate());
}

bool HISSToolsGranular::SerializeState(IByteChunk& chunk) const
{
  if (!SerializeParams(chunk))
    return false;
  
  return mGranular.save(chunk);
}

int HISSToolsGranular::UnserializeState(const IByteChunk& chunk, int pos)
{
  pos = UnserializeParams(chunk, pos);
  pos = mGranular.recall(chunk, pos);
  
  if (mWaveformL && mWaveformR)
  {
    mWaveformL->Set(mGranular.getBufferL(), mGranular.getBufferLength());
    mWaveformR->Set(mGranular.getBufferR(), mGranular.getBufferLength());
  }
  
  return pos;
}

