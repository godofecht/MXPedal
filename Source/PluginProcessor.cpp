/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MXPedalAudioProcessor::MXPedalAudioProcessor() : ParameterizedAudioProcessor()

{
#if PERFETTO
    MelatoninPerfetto::get().beginSession();
#endif
    
    init();
    dsp = std::make_unique<DSP> (this);
    dsp->initialize();
} 

MXPedalAudioProcessor::~MXPedalAudioProcessor()
{
#if PERFETTO
    MelatoninPerfetto::get().endSession();
#endif
}

std::vector<ParameterizedAudioProcessor::ParamInfo> MXPedalAudioProcessor::getParamInfo ()
{    
    return dsp->getParamInfo();
}

juce::AudioProcessorEditor* MXPedalAudioProcessor::createEditor()
{
    auto editor = new MXPedalAudioProcessorEditor (*this, *parameters, undoManager);
    
#ifdef SET_ROUNDED_WINDOW_CORNERS
    if (wrapperType == wrapperType_Standalone)
    {
        if (TopLevelWindow::getNumTopLevelWindows () == 1)
        {
            TopLevelWindow* w = TopLevelWindow::getTopLevelWindow (0);
            w->setUsingNativeTitleBar (true);
            
#if JUCE_WINDOWS || JUCE_MAC
            if (editor->getApplicationWindowRounded())
                if (auto peer = editor->getPeer ())
                    setRoundedCorners (peer->getNativeHandle (), false);
#endif
        }
    }
#endif
    
    return editor;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MXPedalAudioProcessor();
}
