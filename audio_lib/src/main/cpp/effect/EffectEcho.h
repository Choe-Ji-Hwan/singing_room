#ifndef SINGINGROOM_EFFECTECHO_H
#define SINGINGROOM_EFFECTECHO_H


#include <utility>
#include <android/log.h>

class EffectEcho {
public:
    EffectEcho(int sampleRate, int delayTimeMs, float decayScale) {
        maxBufSize = int(sampleRate * delayTimeMs / 1000.0);
        delayBuffer = new short[maxBufSize];
        bufIdx = 0;

        this->decayScale = decayScale;
    }

    ~EffectEcho() {
        delete[] delayBuffer;
    }

    std::pair<short*, int> apply(short* inputData, int size) {
        for (int i = 0; i < size; ++i) {
            short input = inputData[i];
            short echoSample = delayBuffer[bufIdx];

            short output = short(input + echoSample * decayScale);
            inputData[i] = output;

            delayBuffer[bufIdx] = output;
            bufIdx = (bufIdx + 1) % maxBufSize;
        }
        return std::make_pair(inputData, size);
    }

private:
    short* delayBuffer = nullptr;
    int bufIdx = 0;
    int maxBufSize = 0;

    float decayScale = 0.0f;
};


#endif //SINGINGROOM_EFFECTECHO_H
