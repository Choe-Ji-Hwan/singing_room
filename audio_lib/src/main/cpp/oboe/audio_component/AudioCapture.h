#ifndef SINGINGROOM_AUDIOCAPTURE_H
#define SINGINGROOM_AUDIOCAPTURE_H
#include <oboe/Oboe.h>
#include <android/log.h>
#include "../../tools/SynchronizedQueue.h"
using namespace std;

class AudioCapture {
public:
    /**
     * 캡처 준비.
     * @param sampleRate: 캡처할 샘플 레이트.
     * @param channelCnt: 캡처할 채널 개수.
     * @return: 캡처 준비 여부.
     */
    oboe::Result prepare(int sampleRate, int channelCnt);

    /**
     * 캡처 과정.
     * @param onCapture: 캡처가 되었다고 콜백이 실행되면 실행되는 콜백.
     * @return: 시작 여부.
     */
    oboe::Result capture(function<void(short*, int)> onCapture);

    /**
     * 캡처 종료.
     * caution: captureCallback 는 외부에서 받아오기 때문에 여기서 delete 하면 안된다.
     */
    void finish();

private:
    /**
     * Stream에 캡처 되면 실행 되는 Callback.
     */
    class CaptureStreamCallback : public oboe::AudioStreamCallback {
        public:
            /**
             * oboe 내부에서 스트림에 값이 들어오면 실행되는 객체.
             * @param audioStream: 쓸 수 있는 스트림.
             * @param audioData: 실제 실행되는 곳. (여기다 쓰면 읽힘) <- capture 에서 사용 되지 않음.
             * @param numFrames: 받아온 프레임 수.
             * @return 계속할 지 여부.
             */
            oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;

            /**
             * 캡처 되었을 때 실행되는 콜백.
             * @param callback
             */
            void setCaptureCallback(function<void(short*, int)> callback);

        private:
            // 캡처를 했으면 실행 되는 콜백.
            function<void(short*, int)> onCapture = nullptr;
    };

private:
    // 실제로 캡쳐된 내용이 저장될 Stream.
    oboe::AudioStream *captureStream; // 레코딩된 오디오를 가져오는 입력 스트림
    // Stream에 캡처 되면 실행 되는 Callback.
    CaptureStreamCallback* streamCallback = nullptr;

    // 캡처 되었을 때 callback.
    function<void(short*, int)> captureCallback = nullptr;
};


#endif //SINGINGROOM_AUDIOCAPTURE_H
