#pragma once
#include <JuceHeader.h>

template <typename... Stages>
class CircuitModeledAudioEffect : public AudioEffect
{
    std::tuple<Stages...> stages;

public:
    CircuitModeledAudioEffect()
    {
        registerParameter("Drive", 0.0, 0.0f, 1.0f, 0.01f, "%");
    }

    void prepare(const juce::dsp::ProcessSpec& spec) override
    {
        std::apply([&spec](auto&... stage) { ((stage.prepare(spec.sampleRate)), ...); }, stages);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        auto inputBlock = context.getInputBlock();
        auto outputBlock = context.getOutputBlock();
        juce::AudioBuffer<float> buffer;
        copyBlockToBuffer(inputBlock, buffer);

        std::apply([&buffer](auto&... stage) { ((stage.process(buffer)), ...); }, stages);

        copyBufferToBlock(buffer, outputBlock);
    }

    void reset() override
    {
        std::apply([](auto&... stage) { ((stage.reset()), ...); }, stages);
    }

    void updateParameters() override
    {
        // Update de-esser parameters from the host
    }

    juce::String getName() const override { return "MXPedal"; }
};

using PhysicallyModeledMXPedal = CircuitModeledAudioEffect<OpAmpStage, DistortionRotaryCircuit, CircuitStage>;

class DSP : public DSPBase
{
    std::unique_ptr<SingleEffectGraphBuilder<PhysicallyModeledMXPedal>> pedal;
    ParameterizedAudioProcessor* processor;
public:
    
    DSP (ParameterizedAudioProcessor* _processor)
    {
        processor = _processor;
    }
    
    void initialize()
    {
        pedal = std::make_unique<SingleEffectGraphBuilder<PhysicallyModeledMXPedal>> (processor->getValueTreeParameters());
    }
    
    void prepare (double sampleRate, int samplesPerBlock)
    {
        pedal->prepare ({sampleRate, static_cast<juce::uint32>(samplesPerBlock), 1});
        pedal->initialize();
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
        
        pedal->process (context);
        
        for (auto i = numInputChannels; i < numOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
    }
};
