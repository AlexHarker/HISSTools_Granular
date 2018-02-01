#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "IControls.h"
#include "HISSTools_Controls.hpp"
#include "src/Granular.h"
#include "src/Waveform.h"

const int kNumPrograms = 1;

enum EParams
{
  kActive = 0,
  kMode,
  kMaxVoices,
  kDensity,
  kRate,
  kRateRand,
  kOffset,
  kOffsetRand,
  kDuration,
  kDurationRand,
  kPitch,
  kPitchRand,
  kGliss,
  kGlissRand,
  kVol,
  kVolRand,
  kPan,
  kPanRand,
  kWindowType,
  kWindowBias,
  kWindowBiasRand,
  kDistortionType,
  kGain,
  kGainRand,
  kFilterType,
  kFilterFreq,
  kFilterFreqRand,
  kFilterResonance,
  kFilterResonanceRand,
  kSelect,
  kNumParams
};

enum ELayout
{
  kWidth = 940,
  kHeight = 740,
  
  kCol1X = 50,
  kCol2X = 220,
  kCol3X = 390,
  kCol4X = 560,
  kCol5X = 730,

  kRow1Y = 70,
  kRow2Y = 220,
  kRow3Y = 350,
  
  kValueW = 100,
  kValueH = 30,
  kOpenW = 150,
};

class HISSToolsGranular : public IPlug
{
public:
  HISSToolsGranular(IPlugInstanceInfo instanceInfo);
  ~HISSToolsGranular();
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void ProcessBlock(double** inputs, double** outputs, int nFrames) override;
  
  bool SerializeState(IByteChunk& chunk) override;
  int UnserializeState(IByteChunk& chunk, int pos) override;

  void SelectFromGUI(double click, double drag);
  
private:
  
  void GUIUpdateSelection();
  void AddDualControl(IGraphics* graphics, double x, double y, int idx, int idxRand, const char *options);
  void AddBiPolarDualControl(IGraphics* graphics, double x, double y, int idx, int idxRand, const char *options, const char *mainOptions = "");

  HISSTools_VecLib mVecLib;
  HISSTools_FileSelector* mSelector;
  Waveform* mWaveformL;
  Waveform* mWaveformR;
  Granular mGranular;
};
