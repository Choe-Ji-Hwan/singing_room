#ifndef SINGINGROOM_AUDIODATAPROCESSOR_H
#define SINGINGROOM_AUDIODATAPROCESSOR_H

#include <utility>
#include "../effect/EffectEcho.h"


/**
 * 오디오의 변화를 시키는 곳.
 */
class AudioDataProcessor {
public:
    AudioDataProcessor(int sampleRate, int channelCnt) {
        this->sampleRate = sampleRate;
        this->channelCnt = channelCnt;

        echoEffect = new EffectEcho(sampleRate, 200, 0.3f);
    }

    ~AudioDataProcessor() {
        delete echoEffect;
    }

    std::pair<short*, int> process(short* inputData, int size) {
        return echoEffect->apply(inputData, size);
    }

private:
    // todo echo test. (이펙트 중에 하나인 Echo 테스트. -> 컴포지트 패턴 쓸 예정)
    EffectEcho* echoEffect = nullptr;

    int sampleRate = 0;
    int channelCnt = 0;
};


#endif //SINGINGROOM_AUDIODATAPROCESSOR_H
