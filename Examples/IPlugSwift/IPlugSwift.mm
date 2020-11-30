#include "IPlugSwift.h"
#include "IPlug_include_in_plug_src.h"

IPlugSwift::IPlugSwift(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamGain)->InitGain("Volume");
  
  mFaustProcessor.SetMaxChannelCount(MaxNChannels(ERoute::kInput), MaxNChannels(ERoute::kOutput));
  mFaustProcessor.Init();
//  mFaustProcessor.CompileCPP();
//  mFaustProcessor.SetAutoRecompile(true);
  
  MakePreset("Gain = 0dB", 0.);
  MakePreset("Gain = -10dB", -10.);
  MakePreset("Gain = -20dB", -20.);
}

void IPlugSwift::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  mFaustProcessor.ProcessBlock(inputs, outputs, nFrames);
}

void IPlugSwift::OnIdle()
{
  if(mBufferFull)
  {
    float* pData = nullptr;
    if(mActiveBuffer == mVizBuffer1)
      pData = mVizBuffer2;
    else
      pData = mVizBuffer1;
    
    SendArbitraryMsgFromDelegate(kMsgTagData, kDataPacketSize * sizeof(float), pData);
    mBufferFull = false;
  }
}

void IPlugSwift::ProcessMidiMsg(const IMidiMsg& msg)
{
  mFaustProcessor.ProcessMidiMsg(msg);
}

void IPlugSwift::OnParamChange(int paramIdx)
{
  mFaustProcessor.SetParameterValueNormalised(paramIdx, GetParam(paramIdx)->GetNormalized());
}

void IPlugSwift::OnReset()
{
  mFaustProcessor.SetSampleRate(GetSampleRate());
}

bool IPlugSwift::OnMessage(int msgTag, int ctrlTag, int dataSize, const void* pData)
{
  if(msgTag == kMsgTagHello)
  {
    DBGMSG("MsgTagHello received\n");
    return true;
  }
  else if(msgTag == kMsgTagRestorePreset)
  {
    RestorePreset(ctrlTag);
  }
  
  return CocoaEditorDelegate::OnMessage(msgTag, ctrlTag, dataSize, pData);
}
