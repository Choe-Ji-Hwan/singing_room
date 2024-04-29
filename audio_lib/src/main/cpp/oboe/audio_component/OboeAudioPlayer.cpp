#include "OboeAudioPlayer.h"

OboeAudioPlayer::OboeAudioPlayer(int sampleRate, int channelCnt) {
    this->sampleRate = sampleRate;
    this->channelCnt = channelCnt;

    this->queue = new SynchronizedQueue<short>();
}

void OboeAudioPlayer::prepare() {
    oboe::AudioStreamBuilder builder;
    builder.setFormat(oboe::AudioFormat::I16);
    builder.setChannelCount(channelCnt);
    builder.setSampleRate(sampleRate);
    builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);

    callback = new StreamCallback(*queue);
    builder.setDataCallback(callback);

    oboe::AudioStream *stream;
    oboe::Result result = builder.openStream(&stream);
    if (result == oboe::Result::OK) {
        stream->requestStart(); // 스트림 시작.
        this->playerStream = std::unique_ptr<oboe::AudioStream>(stream);;
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "OboeAudioPlayer", "prepare failed");
    }
}

void OboeAudioPlayer::consumeData(short *shortArray, int size) {

}

void OboeAudioPlayer::finish() {
    playerStream->stop(); // 스트림 정지.
    playerStream->close(); // 스트림 닫기.
    playerStream.reset(); // 스트림 객체 해제.
    delete callback;
}

OboeAudioPlayer::StreamCallback::StreamCallback(SynchronizedQueue<short> &queue): targetQueue(queue) {}

oboe::DataCallbackResult
OboeAudioPlayer::StreamCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    auto *data = static_cast<short *>(audioData);

    for(int i = 0; i < numFrames; ++i) {
        *data++ = this->targetQueue.dequeue();
    }

    return oboe::DataCallbackResult::Continue;
}

