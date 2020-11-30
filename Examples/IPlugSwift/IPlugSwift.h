#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "Oscillator.h"
#include "Smoothers.h"
#include "IPlugSwiftSharedConstants.h"
#include "IPlugFaustGen.h"

#define DSP_FILE ""

using namespace iplug;

class IPlugSwift final : public Plugin
{
public:
  IPlugSwift(const InstanceInfo& info);
  
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
  bool OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData) override;
  void OnParamChange(int paramIdx) override;
  void OnIdle() override;
  void OnReset() override;
  
private:
  FastSinOscillator<float> mOsc;
  float mFreqCPS = 440.f;
  LogParamSmooth<sample> mGainSmoother { 20.f };
  float mVizBuffer1[kDataPacketSize];
  float mVizBuffer2[kDataPacketSize];
  float* mActiveBuffer = &mVizBuffer2[0];
  int mCount = 0;
  bool mBufferFull = false;
  FAUST_BLOCK(Faust1, mFaustProcessor, DSP_FILE, 1, 1);
};
