#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MXPedalAudioProcessorEditor::MXPedalAudioProcessorEditor (ParameterizedAudioProcessor& p, juce::AudioProcessorValueTreeState& vts, juce::UndoManager& um)
: PluginEditor (p, vts, um)
{    
    editorScaledComponent = std::make_unique<CustomUserInterface>(p, vts, um);
    initialize (300, 600);
}
