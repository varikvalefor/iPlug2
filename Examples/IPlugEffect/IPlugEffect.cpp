#include "IPlugEffect.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

IPlugEffect::IPlugEffect(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kGain)->InitDouble("Gain", 0., 0., 100.0, 0.01, "%");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    pGraphics->EnableMouseOver(true);

    const IRECT b = pGraphics->GetBounds();
    pGraphics->ShowFPSDisplay(true);
    //pGraphics->AttachControl(new ITextControl(b.GetMidVPadded(50), "Hello iPlug 2!", IText(50)));
    pGraphics->AttachControl(new IVKnobControl(b.GetFromBLHC(100, 100), kGain));
    IVTabSwitchControl* pTabs;
    pGraphics->AttachControl(pTabs = new IVTabSwitchControl(b.GetCentredInside(450, 50), -1, { "CPU", "OpenGL", "Metal", "Direct3D" }, "Graphics Backend", DEFAULT_STYLE.WithValueText({ 36 })))
      ->SetAnimationEndActionFunction([](IControl* pCaller) {
      pCaller->GetDelegate()->SetBackendMode(static_cast<EBackendMode>(pCaller->As<IVTabSwitchControl>()->GetSelectedIdx()));
        })
      ->As<IVTabSwitchControl>()->SetValueByIdx(int(GetBackendMode()));

        pTabs->SetStateDisabled(2, true);
  };
#endif
}

#if IPLUG_DSP
void IPlugEffect::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kGain)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = inputs[c][s] * gain;
    }
  }
}
#endif
