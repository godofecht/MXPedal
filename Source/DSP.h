#pragma once
#include <JuceHeader.h>

class PhysicallyModeledMXPedal : public AudioEffect
{
    enum Parameters { HighFrequency, LowFrequency, Threshold, Ratio, Attack, Release };
    
    OpAmpStage stage1;
    DistortionRotaryCircuit stage2;
    CircuitStage stage3;
    
public:
    PhysicallyModeledMXPedal()
    {
        registerParameter ("HighFrequency", 1400.0f, 1400.0f, 8000.0f, 0.1f, "Hz");
        registerParameter ("LowFrequency", 8000.0f, 8000.0f, 16000.0f, 0.1f, "Hz");
        registerParameter ("Threshold", -20.0f, -40.0f, 0.0f, 0.1f, "dB");
        registerParameter ("Ratio", 3.0f, 1.0f, 10.0f, 1.0f, ":1");
        registerParameter ("Attack", 10.0f, 10.0f, 100.0f, 1.0f, "ms");
        registerParameter ("Release", 50.0f, 10.0f, 500.0f, 1.0f, "ms");
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override 
    {
        stage1.prepare (spec.sampleRate);
        stage2.prepare (spec.sampleRate);
        stage3.prepare (spec.sampleRate);
    }
    
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        auto inputBlock = context.getInputBlock();
        auto outputBlock = context.getOutputBlock();
        juce::AudioBuffer<float> buffer;
        copyBlockToBuffer (inputBlock, buffer);
        
        stage1.process (buffer);
//        stage2.process (buffer);
//        stage3.process (buffer);
        
        copyBufferToBlock (buffer, outputBlock);
    }
    
    void reset() override {/* stage.reset();*/ }

    void updateParameters() override
    {
        // Update de-esser parameters from the host
    }

    juce::String getName() const override { return "MXPedal"; }
};

class DSP : public DSPBase
{
    PhysicallyModeledMXPedal pedal;
public:
    
    DSP (ParameterizedAudioProcessor* _processor)
    {
    }
    
    void initialize()
    {
    }
    
    void prepare (double sampleRate, int samplesPerBlock)
    {
        pedal.prepare (juce::dsp::ProcessSpec {sampleRate, static_cast<juce::uint32>(samplesPerBlock), 1});
    }
    
    std::vector<ParameterizedAudioProcessor::ParamInfo> getParamInfo ()
    {
        return
        {
        };
    }
    
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        juce::ScopedNoDenormals noDenormals;
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        
        pedal.process (context);
        
        for (auto i = numInputChannels; i < numOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
    }
};