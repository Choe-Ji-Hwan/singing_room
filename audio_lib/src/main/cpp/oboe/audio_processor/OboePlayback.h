#include <oboe/Oboe.h>
#include <android/log.h>
#include "../../tools/SynchronizedQueue.h"
#include "../../audio_element/AudioDataProcessor.h"

class OboePlayback : public oboe::AudioStreamCallback {
public:
    /**
     * 생성자.
     * @param sampleRate: playback(녹음, 재생) 사용할 sample Rate.
     * @param channelCnt: playback(녹음, 재생) 사용할 channel 개수.
     */
    OboePlayback(int sampleRate, int channelCnt);

    /**
     * 소멸자.
     * ptr로 만들어 낸 Stream 해제.
     */
    ~OboePlayback();

    /**
     * playback 준비.
     * @return 준비 여부.
     */
    oboe::Result preparePlayback();

    /**
     * playback 시작.
     * @return 시작 여부.
     */
    oboe::Result startPlayback();

    /**
     * 녹음되는 데이터가 들어왔을 때, 실행되는 콜백.
     * @param audioStream: 하드웨어와 연결된 audioStream.
     * @param audioData: 가져온 audio 데이터.
     * @param numFrames: 가져온 audio 데이터 프레임 수.
     * @return: 콜백 결과.
     */
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;

private:
    // playback elements: sample rate, channel count.
    int sampleRate = -1;
    int channelCnt = -1;

    // streams. (recoding, playback)
    std::shared_ptr<oboe::AudioStream> mPlaybackStream;
    std::shared_ptr<oboe::AudioStream> mRecordingStream; // 레코딩된 오디오를 가져오는 입력 스트림

    AudioDataProcessor* audioDataProcessor = nullptr;
};