declare name "IPlugFaustDSP (mono example)";
import("stdfaust.lib");

g = vslider("[1]Gain", 0, 0., 1, 0.1);

process = os.osc(440), os.osc(441) ;//* g,os.osc(440) * g;

