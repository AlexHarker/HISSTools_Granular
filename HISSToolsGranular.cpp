#include "HISSToolsGranular.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "config.h"

// Visual Design

class Design : public HISSTools_Design_Scheme
{
  
public:
  
  Design() : HISSTools_Design_Scheme(true)
  {
    HISSTools_Color_Spec *col1 = new HISSTools_Color_Spec(0.7, 0.0, 0.0, 0.9);
    HISSTools_Color_Spec *col2 = new HISSTools_Color_Spec(0.2, 0.7, 0.3, 0.9);
    HISSTools_Color_Spec *col3 = new HISSTools_Color_Spec(0.5, 0.0, 0.5, 0.9);
    HISSTools_Color_Spec *col4 = new HISSTools_Color_Spec(0.0, 0.5, 0.5, 0.9);
    HISSTools_Color_Spec *col5 = new HISSTools_Color_Spec(0.5, 0.5, 0.0, 0.9);
    HISSTools_Color_Spec *col6 = new HISSTools_Color_Spec(0.1, 0.4, 0.7, 0.9);

    addColorSpec("DialIndicator", "1", col1);
    addColorSpec("DialIndicator", "2", col2);
    addColorSpec("DialIndicator", "3", col3);
    addColorSpec("DialIndicator", "4", col4);
    addColorSpec("DialIndicator", "5", col5);
    addColorSpec("DialIndicator", "6", col6);

    addColorSpec("ButtonHandleOff", "remove", col5);
    addColorSpec("ButtonHandleOn", "remove", col6);
    
    addFlag("ValueDrawTriangle", "small", false);
    addFlag("ValueDrawLabel", "nolabel", false);
    
    addDimension("DialRefValue", "gain", 2.0/7.0);
    addDimension("DialRefValue", "vol", 6.0/7.0);
    
    addDimension("ValueTextArea", "spacious", 25);

    addFlag("ValueLabelBelow", true);
    addFlag("DialDrawValOnlyOnMO", true);
  }
};

Design designScheme;

void HISSToolsGranular::AddDualControl(IGraphics* graphics, double x, double y, int idx, int idxRand, const char *options)
{
  WDL_String val(options);
  WDL_String var("small");
  
  var.Append(" ");
  var.Append(options);
  
  graphics->AttachControl(new HISSTools_Dial(this, idx, &mVecLib, x, y, val.Get(), &designScheme));
  graphics->AttachControl(new HISSTools_Dial(this, idxRand, &mVecLib, x + 100, y - 30, var.Get(), &designScheme));
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
  
  graphics->AttachControl(new HISSTools_Dial(this, idx, &mVecLib, x, y, val.Get(), &designScheme));
  graphics->AttachControl(new HISSTools_Dial(this, idxRand, &mVecLib, x + 100, y - 30, var.Get(), &designScheme));
}

HISSToolsGranular::HISSToolsGranular(IPlugInstanceInfo instanceInfo)
: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mVecLib(nullptr)
{
  TRACE; 

  //arguments are: name, defaultVal, minVal, maxVal, step, label
 
  mVecLib.setSize(kWidth, kHeight);

  GetParam(kActive)->InitBool("Active", true);

  GetParam(kMode)->InitEnum("Mode", 0, 2);
  GetParam(kMode)->SetDisplayText(0, "Streams");
  GetParam(kMode)->SetDisplayText(1, "Clouds");
  
  GetParam(kDensity)->InitDouble("Density", 100.0, 0.0, 100.0, 0.1, "%");
  GetParam(kDensity)->SetShape(2.0);

  GetParam(kMaxVoices)->InitInt("Max Voices", 50, 1, 100);
  
  GetParam(kRate)->InitDouble("Rate", 10.0, 0.1, 2000.0, 0.1, "ms");
  GetParam(kRate)->SetShape(2.0);
  GetParam(kRateRand)->InitDouble("Rand Rate", 20.0, 0.0, 4000.0, 0.1, "ms");
  GetParam(kRateRand)->SetShape(2.0);
  
  GetParam(kOffset)->InitDouble("Offset", 0.0, 0.0, 100.0, 0.1, "%");
  GetParam(kOffsetRand)->InitDouble("Rand Offset", 200.0, 0.0, 10000.0, 0.1, "ms");
  GetParam(kOffsetRand)->SetShape(2.0);
  
  GetParam(kDuration)->InitDouble("Duration", 100.0, 0.1, 5000.0, 0.1, "ms");
  GetParam(kDuration)->SetShape(2.0);
  GetParam(kDurationRand)->InitDouble("Rand Dur", 20.0, 0.0, 4000.0, 0.1, "ms");
  GetParam(kDurationRand)->SetShape(2.0);

  GetParam(kPitch)->InitDouble("Pitch", 0.0, -36.0, 36.0, 0.01, "st");
  GetParam(kPitchRand)->InitDouble("Rand Pitch", 0.0, 0.0, 48.0, 0.01, "st");
  GetParam(kPitchRand)->SetShape(2.0);

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
  
  GetParam(kFilterFreq)->InitDouble("Filter Freq", 500.0, 20.0, 16000.0, 0.1, "Hz");
  GetParam(kFilterFreq)->SetShape(2.0);
  GetParam(kFilterFreqRand)->InitDouble("Rand Freq", 48.0, 0.0, 48.0, 0.1, "st");
  
  GetParam(kFilterResonance)->InitDouble("Filter Reson", 50.0, 0.0, 100.0, 0.1, "%");
  GetParam(kFilterResonanceRand)->InitDouble("Rand Reson", 10.0, 0.0, 100.0, 0.1, "%");
 
  GetParam(kSelect)->InitBool("Select", false);
  GetParam(kSelect)->SetCanAutomate(false);
  
  //create user interface
  IGraphics* pGraphics = MakeGraphics(*this, kWidth, kHeight, 30);
  pGraphics->AttachPanelBackground(COLOR_GRAY);
  
  mSelector = new HISSTools_FileSelector(this, kSelect, &mVecLib, kCol4X, kRow3Y, kOpenW, kValueH, EFileAction::kFileOpen, "", "aif wav", "", &designScheme);

  mWaveformL = new Waveform(*this, &mVecLib, 20, 570, 500, 50);
  mWaveformR = new Waveform(*this, &mVecLib, 20, 620, 500, 50);
  
  pGraphics->AttachControl(mWaveformL);
  pGraphics->AttachControl(mWaveformR);
  
  AddDualControl(pGraphics, kCol1X, kRow1Y, kOffset, kOffsetRand, "1");
  AddDualControl(pGraphics, kCol2X, kRow1Y, kDuration, kDurationRand, "1");
  AddBiPolarDualControl(pGraphics, kCol3X, kRow1Y, kPitch, kPitchRand, "2");
  AddBiPolarDualControl(pGraphics, kCol4X, kRow1Y, kGliss, kGlissRand, "2");
  
  pGraphics->AttachControl(new HISSTools_Value(this, kWindowType, &mVecLib, kCol1X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
  AddBiPolarDualControl(pGraphics, kCol1X, kRow2Y, kWindowBias, kWindowBiasRand, "3");

  pGraphics->AttachControl(new HISSTools_Value(this, kDistortionType, &mVecLib, kCol2X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
  AddBiPolarDualControl(pGraphics, kCol2X, kRow2Y, kGain, kGainRand, "4", "gain");
  
  pGraphics->AttachControl(new HISSTools_Value(this, kFilterType, &mVecLib, kCol3X, kRow3Y, kValueW, kValueH, "spacious", &designScheme));
  AddDualControl(pGraphics, kCol3X, kRow2Y, kFilterFreq, kFilterFreqRand, "5");
  AddDualControl(pGraphics, kCol4X, kRow2Y, kFilterResonance, kFilterResonanceRand, "5");

  AddBiPolarDualControl(pGraphics, kCol5X, kRow1Y, kPan, kPanRand, "6");
  AddBiPolarDualControl(pGraphics, kCol5X, kRow2Y, kVol, kVolRand, "6", "vol");

  pGraphics->AttachControl(new HISSTools_Value(this, kDensity, &mVecLib, kCol5X, kRow3Y, kOpenW, kValueH, "spacious", &designScheme));
  pGraphics->AttachControl(new HISSTools_Button(this, kActive, &mVecLib, kCol5X, kRow3Y + 90, kOpenW, kValueH, "spacious", &designScheme));

  pGraphics->AttachControl(new HISSTools_Value(this, kMaxVoices, &mVecLib, kCol4X, kRow3Y + 90, kOpenW, kValueH, "spacious", &designScheme));
  
  pGraphics->AttachControl(new HISSTools_Value(this, kMode, &mVecLib, kCol2X, kRow3Y + 90, kOpenW, kValueH, "spacious", &designScheme));
  
  AddDualControl(pGraphics, kCol1X, kRow3Y + 90, kRate, kRateRand, "1");

  pGraphics->AttachControl(mSelector);
  
  pGraphics->HandleMouseOver(true);
  pGraphics->SetStrictDrawing(false);
  
  AttachGraphics(pGraphics);
  
  MakeDefaultPreset("-", kNumPrograms);
}

HISSToolsGranular::~HISSToolsGranular() {}

void HISSToolsGranular::ProcessBlock(double** inputs, double** outputs, int nFrames)
{
  mParams_mutex.Enter();
  
  mGranular.processBlock(outputs[0], outputs[1], nFrames, mSampleRate);

  mParams_mutex.Leave();

}

void HISSToolsGranular::GUIUpdateSelection()
{
  double L = GetParam(kOffset)->Value() / 100.0;
  double R = L + (GetParam(kOffsetRand)->Value() / (1000.0 * mGranular.getBufferDuration()));
  
  mWaveformL->SetSelect(L, R);
  mWaveformR->SetSelect(L, R);
}


void HISSToolsGranular::OnParamChange(int paramIdx)
{
  mParams_mutex.Enter();
  
  WDL_String str;
  
  switch (paramIdx)
  {
    case kDensity:
    case kActive:
      mGranular.setDensity(GetParam(kActive)->Value() * (GetParam(kDensity)->Value() / 100.0));
      break;
      
    case kMode:
      GetGUI()->GrayOutControl(kRate, !GetParam(kMode)->Int());
      GetGUI()->GrayOutControl(kRateRand, !GetParam(kMode)->Int());
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
      GUIUpdateSelection();
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
      GetGUI()->GrayOutControl(kGain, !GetParam(kDistortionType)->Int());
      GetGUI()->GrayOutControl(kGainRand, !GetParam(kDistortionType)->Int());
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
      
      GetGUI()->GrayOutControl(kFilterFreq, !GetParam(kFilterType)->Int());
      GetGUI()->GrayOutControl(kFilterFreqRand, !GetParam(kFilterType)->Int());
      GetGUI()->GrayOutControl(kFilterResonance, !GetParam(kFilterType)->Int());
      GetGUI()->GrayOutControl(kFilterResonanceRand, !GetParam(kFilterType)->Int());
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
      
    case kSelect:
      mSelector->GetLastSelectedFileForPlug(&str);
      mGranular.load(str.Get());
      mGranular.setWaveformL(mWaveformL);
      mGranular.setWaveformR(mWaveformR);
      GUIUpdateSelection();
      break;
  }
  
  mParams_mutex.Leave();
}

void HISSToolsGranular::OnReset()
{
  mGranular.reset(mSampleRate);
}

bool HISSToolsGranular::SerializeState(IByteChunk& chunk)
{
  if (!SerializeParams(chunk))
    return false;
  
  return mGranular.save(chunk);
}

int HISSToolsGranular::UnserializeState(IByteChunk& chunk, int pos)
{
  pos = UnserializeParams(chunk, pos);
  return mGranular.recall(chunk, pos);
}
