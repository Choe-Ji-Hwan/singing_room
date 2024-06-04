#include "OboePlayback.h"

/**
 * 생성자.
 * @param sampleRate: playback(녹음, 재생) 사용할 sample Rate.
 * @param channelCnt: playback(녹음, 재생) 사용할 channel 개수.
 */
OboePlayback::OboePlayback(int sampleRate, int channelCnt) {
    this->sampleRate = sampleRate;
    this->channelCnt = channelCnt;
}

/**
 * 소멸자.
 * ptr로 만들어 낸 Stream 해제.
 */
OboePlayback::~OboePlayback() {
    mRecordingStream->close();
    mRecordingStream->release();
    mRecordingStream = nullptr;

    mPlaybackStream->close();
    mPlaybackStream->release();
    mPlaybackStream = nullptr;
}

/**
 * playback 준비.
 * @return 준비 여부.
 */
oboe::Result OboePlayback::preparePlayback() {
    // 재생 스트림 열기
    oboe::AudioStreamBuilder playbackBuilder;
    playbackBuilder.setDirection(oboe::Direction::Output);
    playbackBuilder.setFormat(oboe::AudioFormat::I16);
    playbackBuilder.setSharingMode(oboe::SharingMode::Exclusive);
    playbackBuilder.setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium);
    playbackBuilder.setSampleRate(sampleRate);
    playbackBuilder.setChannelCount(channelCnt);
    playbackBuilder.setUsage(oboe::Usage::Game);
    playbackBuilder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    playbackBuilder.setCallback(this);
    auto result = playbackBuilder.openStream(mPlaybackStream);
    if (result != oboe::Result::OK) return result;

    // 레코딩 스트림 열기
    oboe::AudioStreamBuilder recordingBuilder;
    recordingBuilder.setDirection(oboe::Direction::Input);
    recordingBuilder.setFormat(oboe::AudioFormat::I16);
    recordingBuilder.setSharingMode(oboe::SharingMode::Exclusive);
    recordingBuilder.setSampleRate(sampleRate);
    recordingBuilder.setChannelCount(channelCnt);
    recordingBuilder.setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium);
    recordingBuilder.setUsage(oboe::Usage::Game);
    recordingBuilder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    recordingBuilder.setCallback(nullptr); // 레코딩은 콜백 없이 처리
    result = recordingBuilder.openStream(mRecordingStream);
    if (result != oboe::Result::OK) return result;

    return oboe::Result::OK;
}

oboe::Result OboePlayback::startPlayback() {
    // 레코딩 스트림 시작
    oboe::Result recordingResult = mRecordingStream->start();
    if (recordingResult != oboe::Result::OK) return oboe::Result::ErrorInvalidState;

    // 재생 스트림 시작
    oboe::Result playbackResult = mPlaybackStream->start();
    if (playbackResult != oboe::Result::OK) return oboe::Result::ErrorInvalidState;

    return oboe::Result::OK;
}

/**
 * playback 시작.
 * @return 시작 여부.
 */
oboe::DataCallbackResult
OboePlayback::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    auto* resultData = new int16_t[numFrames];
    auto* outputData = static_cast<int16_t *>(audioData);
    mRecordingStream->read(resultData, numFrames, 0);

    // 이제 resultData에서 읽어왔으니, 여기서 데이터 변환.
    // -----------------------------------------
    // resultData 를 가지고 작업하면 됨.
    //
    // -----------------------------------------
    for(int i = 0; i < numFrames; ++i) {
        resultData[i] *= 4; // 음량 4배.
        // 만약 오버플로우 발생 시 최대값으로 설정
        if (resultData[i] > std::numeric_limits<int16_t>::max()) {
            resultData[i] = std::numeric_limits<int16_t>::max();
        } else if (resultData[i] < std::numeric_limits<int16_t>::min()) {
            resultData[i] = std::numeric_limits<int16_t>::min();
        }
    }
    for (int i = 0; i < numFrames; ++i) {
        outputData[i] = resultData[i];
    }
    return oboe::DataCallbackResult::Continue;
}



