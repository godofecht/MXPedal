#pragma once
#include <JuceHeader.h>
#include <quilio_core/quilio_core.h>
#include "PluginProcessor.h"

class CustomUserInterface  : public EditorScaledComponent
{
    
public:
    CustomUserInterface (ParameterizedAudioProcessor& processor, juce::AudioProcessorValueTreeState& vts, juce::UndoManager& um)
        : EditorScaledComponent (processor, vts, um)
    {
    }

    ~CustomUserInterface() override
    {

    }

    void onWindowReopen() override
    {

    }

    void paint (juce::Graphics& g) override
    {
    }

    //This is really just a map of components to coordinates...
    void resized() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomUserInterface)
};
