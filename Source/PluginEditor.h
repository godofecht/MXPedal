#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI.h"

//==============================================================================
class MXPedalAudioProcessorEditor : public PluginEditor
{
public:

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    MXPedalAudioProcessorEditor (ParameterizedAudioProcessor&, juce::AudioProcessorValueTreeState&, juce::UndoManager&);
    
    ~MXPedalAudioProcessorEditor() override { setLookAndFeel (nullptr); }

    bool keyPressed (const juce::KeyPress& key)
    {
        if (KeyShortcutManager::isCmdZ (key) && valueTreeState.undoManager->canUndo())
        {
            valueTreeState.undoManager->undo();
            return true;
        }

        if (KeyShortcutManager::isCmdY (key) && valueTreeState.undoManager->canRedo())
        {
            valueTreeState.undoManager->redo();
            return true;
        }

        if (KeyShortcutManager::isCtrlShiftZ (key))
        {
            // Your code to handle Ctrl + Shift + Z goes here
            return true;
        }

        if (KeyShortcutManager::isCtrlShiftR (key))
        {
            // Your code to handle Ctrl + Shift + Z goes here
            auto customNode = valueTreeState.state.getOrCreateChildWithName ("Custom", nullptr);
            customNode.setProperty ("width", getRawWidth()/4, nullptr);
            customNode.setProperty ("height", getRawHeight()/4, nullptr);
            setSize (getRawWidth()/4, getRawHeight()/4);
            return true;
        }

        return false;
    }

    //==============================================================================
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MXPedalAudioProcessorEditor)
};
