#include "AudioCapture.h"

#include <utility>

/**
 * 캡처 준비.
 * @param sampleRate: 캡처할 샘플 레이트.
 * @param channelCnt: 캡처할 채널 개수.
 * @return: 캡처 준비 여부.
 */
oboe::Result AudioCapture::prepare(int sampleRate, int channelCnt, function<void(short*, int)> onCapture)  {
    // 콜백 생성.
    this->streamCallback = new CaptureStreamCallback();

    // 콜백에 action 콜백 전달.
    streamCallback->setActionOnCapture(std::move(onCapture));

    // 레코딩 스트림 열기
    oboe::AudioStreamBuilder captureBuilder;
    captureBuilder.setDirection(oboe::Direction::Input);
    captureBuilder.setFormat(oboe::AudioFormat::I16);
    captureBuilder.setSharingMode(oboe::SharingMode::Exclusive);
    captureBuilder.setSampleRate(sampleRate);
    captureBuilder.setChannelCount(channelCnt);
    captureBuilder.setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium);
    captureBuilder.setUsage(oboe::Usage::Game);
    captureBuilder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    captureBuilder.setCallback(streamCallback);
    return captureBuilder.openStream(&captureStream);
}

/**
 * 캡처 과정.
 * @param onCapture: 캡처가 되었다고 콜백이 실행되면 실행되는 콜백.
 * @return: 시작 여부.
 */
oboe::Result AudioCapture::start() {
    // 캡처 스트림 시작.
    return captureStream->start();
}

/**
 * 캡처 종료.
 * caution: captureCallback 는 외부에서 받아오기 때문에 여기서 delete 하면 안된다.
 */
void AudioCapture::finish() {
    captureStream->stop(); // 스트림 정지.
    captureStream->close(); // 스트림 닫기.
    captureStream->release(); // 스트림 객체 해제.

    // Stream 콜백 삭제.
    delete streamCallback;
}

/**
 * oboe 내부에서 스트림에 값이 들어오면 실행되는 객체.
 * @param audioStream: 쓸 수 있는 스트림.
 * @param audioData: 실제 실행되는 곳. (여기다 쓰면 읽힘) <- start 에서 사용 되지 않음.
 * @param numFrames: 받아온 프레임 수.
 * @return 계속할 지 여부.
 */
oboe::DataCallbackResult AudioCapture::CaptureStreamCallback::onAudioReady(
        oboe::AudioStream *audioStream,
        void *audioData,
        int32_t numFrames
) {
    auto* resultData = new short[numFrames];
    // resultData 배열에 읽는다. 데이터 방향: audioStream -> resultData
    audioStream->read(resultData, numFrames, 0);

    // 읽어온 pcm audio data, 콜백을 통해 전달.
    onCapture(resultData, numFrames);
    delete[] resultData;

    return oboe::DataCallbackResult::Continue;
}

/**
 * 캡처 되었을 때 실행되는 콜백.
 * @param action
 */
void AudioCapture::CaptureStreamCallback::setActionOnCapture(function<void(short *, int)> action) {
    this->onCapture = std::move(action);
}
