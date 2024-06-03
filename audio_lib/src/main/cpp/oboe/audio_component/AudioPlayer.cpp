#include "AudioPlayer.h"

oboe::Result AudioPlayer::prepare(int sampleRate, int channelCnt) {
    // 큐 준비.
    this->queue = new SynchronizedQueue<short>();
    // 스트림 콜백 준비.
    this->playerStreamCallback = new PlayerStreamCallback(*queue);

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
    playbackBuilder.setCallback(playerStreamCallback);
    return playbackBuilder.openStream(&playerStream);
}

oboe::Result AudioPlayer::start() {
    // 플레이어 실행.
    return playerStream->start();
}

void AudioPlayer::consumeData(short *shortArray, int size) {
    for (int i = 0; i < size; ++i) {
        queue->enqueue(shortArray[i]);
    }
    __android_log_print(ANDROID_LOG_DEBUG, "test2", "enque: %d", shortArray[0]);
}

void AudioPlayer::finish() {
    playerStream->stop(); // 스트림 정지.
    playerStream->close(); // 스트림 닫기.
    playerStream->release(); // 스트림 객체 해제.

    // Stream 콜백 삭제.
    delete playerStreamCallback;
}


AudioPlayer::PlayerStreamCallback::PlayerStreamCallback(SynchronizedQueue<short> &queue): targetQueue(queue) {}

oboe::DataCallbackResult
AudioPlayer::PlayerStreamCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    auto *data = static_cast<short *>(audioData);

    for(int i = 0; i < numFrames; ++i) {
        *data++ = this->targetQueue.dequeue();
    }

    return oboe::DataCallbackResult::Continue;
}

