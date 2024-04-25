#include <oboe/Oboe.h>
#include <android/log.h>
#include "../../tools/SynchronizedQueue.h"

class OboeEngine : public oboe::AudioStreamCallback {
public:
    oboe::Result startPlaybackAndRecording() {
        // 재생 스트림 열기
        oboe::AudioStreamBuilder playbackBuilder;
        playbackBuilder.setDirection(oboe::Direction::Output);
        playbackBuilder.setFormat(oboe::AudioFormat::I16);
        playbackBuilder.setSharingMode(oboe::SharingMode::Exclusive);
        playbackBuilder.setSampleRate(44100);
        playbackBuilder.setChannelCount(1);
        playbackBuilder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
        playbackBuilder.setCallback(this);
        auto result = playbackBuilder.openStream(mPlaybackStream);
        if (result != oboe::Result::OK) return result;

        // 레코딩 스트림 열기
        oboe::AudioStreamBuilder recordingBuilder;
        recordingBuilder.setDirection(oboe::Direction::Input);
        recordingBuilder.setFormat(oboe::AudioFormat::I16);
        recordingBuilder.setSharingMode(oboe::SharingMode::Exclusive);
        recordingBuilder.setSampleRate(44100);
        recordingBuilder.setChannelCount(1);
        recordingBuilder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
        recordingBuilder.setCallback(nullptr); // 레코딩은 콜백 없이 처리
        result = recordingBuilder.openStream(mRecordingStream);
        if (result != oboe::Result::OK) return result;

        // 레코딩 스트림 시작
        mRecordingStream->start();

        // 재생 스트림 시작
        return mPlaybackStream->start();
    }

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override {
        // 레코딩된 오디오 데이터를 출력 스트림으로 전송
        if (audioStream == mPlaybackStream.get()) {
            mRecordingStream->read(audioData, numFrames, 0);
        }
        return oboe::DataCallbackResult::Continue;
    }

private:
    std::shared_ptr<oboe::AudioStream> mPlaybackStream;
    std::shared_ptr<oboe::AudioStream> mRecordingStream; // 레코딩된 오디오를 가져오는 입력 스트림
};