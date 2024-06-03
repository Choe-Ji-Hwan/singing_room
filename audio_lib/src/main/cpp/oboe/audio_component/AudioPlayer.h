#ifndef SINGINGROOM_AUDIOPLAYER_H
#define SINGINGROOM_AUDIOPLAYER_H

#include <oboe/Oboe.h>
#include <android/log.h>
#include "../../tools/SynchronizedQueue.h"
using namespace std;

/**
 * Oboe를 이용한 Audio Player.
 */
class AudioPlayer {
public:
    /**
     * 플레이 준비. 준비.
     * @param sampleRate: 캡처할 샘플 레이트.
     * @param channelCnt: 캡처할 채널 개수.
     * @return: 캡처 준비 여부.
     */
    oboe::Result prepare(int sampleRate, int channelCnt);

    /**
     * 플레이어 실행.
     */
    oboe::Result start();

    /**
     * 데이터 소비 과정..
     * @return: 시작 여부.
     */
    void consumeData(short* shortArray, int size);

    /**
     * 캡처 종료.
     */
    void finish();

private:
    /**
     * 데이터 처리 콜백 클래스.
     */
    class PlayerStreamCallback : public oboe::AudioStreamCallback {
        public:
            /**
             * 생성자
             * @param queue: 사용할 받아온 큐.
             */
            PlayerStreamCallback(SynchronizedQueue<short> &queue);

            /**
             * 오디오가 준비되면 실행되는 콜백.
             * @param audioStream: 쓸 수 있는 스트림.
             * @param audioData: 실제 실행되는 곳. (여기다 쓰면 읽힘) <- start 에서 사용 되지 않음.
             * @param numFrames: 받아온 프레임 수.
             * @return 계속할 지 여부.
             */
            oboe::DataCallbackResult onAudioReady(oboe::AudioStream* audioStream, void *audioData, int32_t numFrames) override;

        private:
            // 가져온 데이터를 저장할 synchronized queue.
            // 큐를 실제로 이용하는 곳.
            SynchronizedQueue<short>& targetQueue;
    };

private:
    // 재생을 위한 stream.
    oboe::AudioStream* playerStream = nullptr;
    // Stream을 통해, 재생하기 위한 Callback.
    PlayerStreamCallback* playerStreamCallback = nullptr;

    // 가져온 데이터를 저장할 synchronized queue.
    SynchronizedQueue<short>* queue = nullptr;
};


#endif //SINGINGROOM_AUDIOPLAYER_H
