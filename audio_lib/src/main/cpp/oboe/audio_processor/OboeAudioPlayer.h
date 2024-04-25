#ifndef SINGINGROOM_OBOEAUDIOPLAYER_H
#define SINGINGROOM_OBOEAUDIOPLAYER_H

#include <oboe/Oboe.h>
#include <android/log.h>
#include "../../tools/SynchronizedQueue.h"

/**
 * Oboe를 이용한 Audio Player.
 */
class OboeAudioPlayer {
public:
    OboeAudioPlayer(int sampleRate, int channelCnt);

    void prepare();

    void consumeData(short* shortArray, int size);

    void finish();

private:
    /**
     * 데이터 처리 콜백 클래스.
     */
    class StreamCallback : public oboe::AudioStreamDataCallback {
    public:
        StreamCallback(SynchronizedQueue<short> &queue);

        oboe::DataCallbackResult onAudioReady(oboe::AudioStream* audioStream, void *audioData, int32_t numFrames) override;

    private:
        SynchronizedQueue<short>& targetQueue;
    };

private:
    SynchronizedQueue<short>* queue = nullptr;
    int sampleRate;
    int channelCnt;

    std::unique_ptr<oboe::AudioStream> oboeStream;
    StreamCallback* callback = nullptr;
};


#endif //SINGINGROOM_OBOEAUDIOPLAYER_H
