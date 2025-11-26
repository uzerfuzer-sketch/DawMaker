#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
// AudioCompressor - Compresor profesional con tipos VCA y Opto
//==============================================================================
class AudioCompressor
{
public:
    enum class CompressorType
    {
        kVCA,
        kOpto
    };
    
    AudioCompressor() = default;
    ~AudioCompressor() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
        reset();
    }
    
    void reset()
    {
        envelope = 0.0f;
    }
    
    template <typename SampleType>
    SampleType processSample(SampleType input, int channel)
    {
        // Aplicar gain de entrada
        auto sample = input * inputGain;
        
        // Detector de envolvente
        auto inputLevel = std::abs(sample);
        auto inputLevelDB = juce::Decibels::gainToDecibels(inputLevel + 0.00001f);
        
        // Actualizar envolvente
        if (inputLevelDB > envelope)
            envelope += (inputLevelDB - envelope) * alphaAttack;
        else
            envelope += (inputLevelDB - envelope) * alphaRelease;
        
        // Calcular reducción de ganancia
        SampleType gainReduction = 0.0f;
        
        if (envelope > threshold)
        {
            auto overThreshold = envelope - threshold;
            
            // Aplicar knee suave
            if (knee > 0.0f && overThreshold < knee)
            {
                auto kneeRatio = overThreshold / knee;
                gainReduction = overThreshold * kneeRatio * (1.0f - 1.0f / ratio) * 0.5f;
            }
            else
            {
                gainReduction = overThreshold * (1.0f - 1.0f / ratio);
            }
        }
        
        // Convertir a ganancia lineal
        auto gainLinear = juce::Decibels::decibelsToGain(-gainReduction);
        
        // Aplicar tipo de compresor
        if (compressorType == CompressorType::kOpto)
        {
            // Opto es más suave y lento
            gainLinear = std::pow(gainLinear, 0.8f);
        }
        
        // Aplicar compresión
        auto compressed = sample * gainLinear;
        
        // Gain de salida
        compressed *= outputGain;
        
        // Mix (wet/dry)
        auto output = input + (compressed - input) * mix;
        
        // Almacenar reducción para metering
        if (channel == 0)
            currentGainReduction = gainReduction;
        
        return output;
    }
    
    void setThreshold(float newThreshold)
    {
        threshold = newThreshold;
    }
    
    void setRatio(float newRatio)
    {
        ratio = juce::jlimit(1.0f, 20.0f, newRatio);
    }
    
    void setAttack(float newAttack)
    {
        attack = juce::jlimit(0.1f, 100.0f, newAttack);
        alphaAttack = 1.0f - std::exp(-1.0f / (attack * 0.001f * sampleRate));
    }
    
    void setRelease(float newRelease)
    {
        release = juce::jlimit(10.0f, 1000.0f, newRelease);
        alphaRelease = 1.0f - std::exp(-1.0f / (release * 0.001f * sampleRate));
    }
    
    void setKnee(float newKnee)
    {
        knee = juce::jlimit(0.0f, 12.0f, newKnee);
    }
    
    void setInputGain(float newGain)
    {
        inputGain = juce::Decibels::decibelsToGain(newGain);
    }
    
    void setOutputGain(float newGain)
    {
        outputGain = juce::Decibels::decibelsToGain(newGain);
    }
    
    void setMix(float newMix)
    {
        mix = juce::jlimit(0.0f, 1.0f, newMix);
    }
    
    void setCompressorType(CompressorType newType)
    {
        compressorType = newType;
    }
    
    float getGainReduction() const { return currentGainReduction; }

private:
    float sampleRate = 44100.0f;
    float threshold = -10.0f;
    float ratio = 4.0f;
    float attack = 5.0f;
    float release = 50.0f;
    float knee = 3.0f;
    float inputGain = 1.0f;
    float outputGain = 1.0f;
    float mix = 1.0f;
    
    float alphaAttack = 0.0f;
    float alphaRelease = 0.0f;
    float envelope = 0.0f;
    float currentGainReduction = 0.0f;
    
    CompressorType compressorType = CompressorType::kVCA;
};

//==============================================================================
// AudioExpander - Expansor/Gate de ruido
//==============================================================================
class AudioExpander
{
public:
    AudioExpander() = default;
    ~AudioExpander() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
        reset();
    }
    
    void reset()
    {
        envelope = 0.0f;
    }
    
    template <typename SampleType>
    SampleType processSample(SampleType input)
    {
        auto inputLevel = std::abs(input);
        auto inputLevelDB = juce::Decibels::gainToDecibels(inputLevel + 0.00001f);
        
        // Actualizar envolvente
        if (inputLevelDB > envelope)
            envelope += (inputLevelDB - envelope) * alphaAttack;
        else
            envelope += (inputLevelDB - envelope) * alphaRelease;
        
        // Calcular expansión
        SampleType gain = 1.0f;
        
        if (envelope < threshold)
        {
            auto belowThreshold = threshold - envelope;
            auto expansion = belowThreshold * (ratio - 1.0f);
            gain = juce::Decibels::decibelsToGain(-expansion);
        }
        
        return input * gain;
    }
    
    void setThreshold(float newThreshold)
    {
        threshold = newThreshold;
    }
    
    void setRatio(float newRatio)
    {
        ratio = juce::jlimit(1.0f, 10.0f, newRatio);
    }
    
    void setAttack(float newAttack)
    {
        attack = juce::jlimit(0.1f, 50.0f, newAttack);
        alphaAttack = 1.0f - std::exp(-1.0f / (attack * 0.001f * sampleRate));
    }
    
    void setRelease(float newRelease)
    {
        release = juce::jlimit(10.0f, 500.0f, newRelease);
        alphaRelease = 1.0f - std::exp(-1.0f / (release * 0.001f * sampleRate));
    }

private:
    float sampleRate = 44100.0f;
    float threshold = -40.0f;
    float ratio = 2.0f;
    float attack = 1.0f;
    float release = 100.0f;
    
    float alphaAttack = 0.0f;
    float alphaRelease = 0.0f;
    float envelope = 0.0f;
};

//==============================================================================
// AudioDistortion - Distorsión con múltiples tipos de clippers
//==============================================================================
class AudioDistortion
{
public:
    enum class ClipperType
    {
        kSoft,
        kHard,
        kTube,
        kFoldback
    };
    
    AudioDistortion() = default;
    ~AudioDistortion() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
    }
    
    template <typename SampleType>
    SampleType processSample(SampleType input)
    {
        auto sample = input * drive;
        
        // Aplicar tipo de clipper
        switch (clipperType)
        {
            case ClipperType::kSoft:
                sample = softClipper(sample);
                break;
                
            case ClipperType::kHard:
                sample = hardClipper(sample);
                break;
                
            case ClipperType::kTube:
                sample = tubeClipper(sample);
                break;
                
            case ClipperType::kFoldback:
                sample = foldbackClipper(sample);
                break;
        }
        
        // Mix y output gain
        auto output = input + (sample - input) * mix;
        return output * outputGain;
    }
    
    void setDrive(float newDrive)
    {
        drive = juce::jlimit(1.0f, 50.0f, newDrive);
    }
    
    void setMix(float newMix)
    {
        mix = juce::jlimit(0.0f, 1.0f, newMix);
    }
    
    void setOutputGain(float newGain)
    {
        outputGain = juce::jlimit(0.0f, 2.0f, newGain);
    }
    
    void setClipperType(ClipperType newType)
    {
        clipperType = newType;
    }

private:
    template <typename SampleType>
    SampleType softClipper(SampleType input)
    {
        return std::tanh(input);
    }
    
    template <typename SampleType>
    SampleType hardClipper(SampleType input)
    {
        return juce::jlimit(SampleType(-1.0), SampleType(1.0), input);
    }
    
    template <typename SampleType>
    SampleType tubeClipper(SampleType input)
    {
        auto abs_input = std::abs(input);
        
        if (abs_input < 1.0f / 3.0f)
            return input * 2.0f;
        else if (abs_input < 2.0f / 3.0f)
            return (3.0f - std::pow(2.0f - 3.0f * abs_input, 2.0f)) / 3.0f * (input > 0 ? 1.0f : -1.0f);
        else
            return input > 0 ? 1.0f : -1.0f;
    }
    
    template <typename SampleType>
    SampleType foldbackClipper(SampleType input)
    {
        auto sample = input;
        
        while (std::abs(sample) > 1.0f)
        {
            if (sample > 1.0f)
                sample = 2.0f - sample;
            else if (sample < -1.0f)
                sample = -2.0f - sample;
        }
        
        return sample;
    }
    
    float sampleRate = 44100.0f;
    float drive = 1.0f;
    float mix = 1.0f;
    float outputGain = 0.5f;
    ClipperType clipperType = ClipperType::kSoft;
};

//==============================================================================
// AudioBitCrusher - Reducción de bits y sample rate
//==============================================================================
class AudioBitCrusher
{
public:
    AudioBitCrusher() = default;
    ~AudioBitCrusher() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
        phase = 0.0f;
    }
    
    template <typename SampleType>
    SampleType processSample(SampleType input)
    {
        // Reducción de sample rate
        phase += crushRate / sampleRate;
        
        if (phase >= 1.0f)
        {
            phase -= 1.0f;
            lastOutput = input;
            
            // Reducción de bits
            if (bitDepth < 16.0f)
            {
                auto steps = std::pow(2.0f, bitDepth);
                lastOutput = std::floor(lastOutput * steps) / steps;
            }
        }
        
        // Mix
        return input + (lastOutput - input) * mix;
    }
    
    void setCrushRate(float newRate)
    {
        crushRate = juce::jlimit(100.0f, sampleRate, newRate);
    }
    
    void setBitDepth(float newDepth)
    {
        bitDepth = juce::jlimit(1.0f, 16.0f, newDepth);
    }
    
    void setMix(float newMix)
    {
        mix = juce::jlimit(0.0f, 1.0f, newMix);
    }

private:
    float sampleRate = 44100.0f;
    float crushRate = 44100.0f;
    float bitDepth = 16.0f;
    float mix = 1.0f;
    float phase = 0.0f;
    float lastOutput = 0.0f;
};
