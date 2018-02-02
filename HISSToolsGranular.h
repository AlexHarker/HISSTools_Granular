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
  kWidth = 945,
  kHeight = 645,
  
  kSmallDialXOffset = 100,
  kSmallDialYOffset = -10,
  
  kColW = 180,
  kCol1X = 30,
  kCol2X = kCol1X + kColW,
  kCol3X = kCol2X + kColW,
  kCol4X = kCol3X + kColW,
  kCol5X = kCol4X + kColW,

  kRow1Y = 275,
  kRow2Y = kRow1Y + 150,
  kRow3Y = kRow2Y + 140,
  
  kWaveformX = 30,
  kWaveformY = 30,
  kWaveformW = 515,
  kWaveformH = 70,
  
  kValueW = 100,
  kValueH = 25,
  
  kValueWL = 120,
  kValueHL = 25,

  kActiveW = 160,
  
  kYSGapping = 20,
  kYLGapping = 70,
  
  kRateActiveX = kCol4X + 15,
  kVoiceColX = kRateActiveX + 200,
  
  kVoiceRow1Y = kWaveformY + 20,
  kVoiceRow2Y = kVoiceRow1Y + kYLGapping,
  kVoiceRow3Y = kVoiceRow2Y + kYLGapping,
  
  kBeneathWavefromY = kWaveformY + kWaveformH + kWaveformH + kYSGapping,
  kRateY = kWaveformY + 30,
  
  kNameW = 405,
  kNameH = 45,
  
  kNameX = kWidth - (30 + kNameW),
  kNameY = kRow3Y,
  
  kPanel1X = 15,
  kPanel1Y = 15,
  kPanel1W = kWidth - (15 * 2),
  kPanel1H = 220,
  
  kPanel2X = 15,
  kPanel2Y = kRow1Y - 30,
  kPanel2W = kWidth - (15 * 2),
  kPanel2H = 385,
  
  kWaveformOutline = 5,
  kPanel3X = kWaveformX,
  kPanel3Y = kWaveformY,
  kPanel3W = kWaveformW,
  kPanel3H = kWaveformH * 2,
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
