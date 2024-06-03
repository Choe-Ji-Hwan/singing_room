#ifndef SINGINGROOM_AUDIOCAPTURE_H
#define SINGINGROOM_AUDIOCAPTURE_H
#include <oboe/Oboe.h>
#include <android/log.h>
using namespace std;

/**
 * Oboe를 이용한 AudioCapture.
 */
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
     * 캡처 시 콜백 재생.
     * @param onCapture
     */
    void prepareActionOnCapture(function<void(short*, int)> onCapture) {
        captureCallback = onCapture;

        streamCallback->setActionOnCapture(captureCallback);
    }

    /**
     * 캡처 과정.
     * @param onCapture: 캡처가 되었다고 콜백이 실행되면 실행되는 콜백.
     * @return: 시작 여부.
     */
    oboe::Result start();

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
             * @param audioData: 실제 실행되는 곳. (여기다 쓰면 읽힘) <- start 에서 사용 되지 않음.
             * @param numFrames: 받아온 프레임 수.
             * @return 계속할 지 여부.
             */
            oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;

            /**
             * @param action: 캡처 되었을 때 실행되는 콜백.
             */
            void setActionOnCapture(function<void(short*, int)> action);

        private:
            // 캡처를 했으면 실행 되는 콜백.
            function<void(short*, int)> onCapture = nullptr;
    };

private:
    // 실제로 캡쳐된 내용이 저장될 Stream.
    oboe::AudioStream *captureStream = nullptr;
    // Stream에 캡처 되면 실행 되는 Callback.
    CaptureStreamCallback* streamCallback = nullptr;

    // 캡처 되었을 때 streamCallback.
    function<void(short*, int)> captureCallback = nullptr;
};


#endif //SINGINGROOM_AUDIOCAPTURE_H
