
#include "AudioEncoder.h"

AudioEncoder::AudioEncoder()
{
}

AudioEncoder::~AudioEncoder()
{
	if (ctx_) {
		DeInit();
	}
}

int AudioEncoder::InitAAC(int channels, int sample_rate, int bit_rate)
{
	channels_ = channels;
	sample_rate_ = sample_rate;
	bit_rate_ = bit_rate;

	AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
	if (!codec) {
		printf(" %s %d avcodec_find_encoder AV_CODEC_ID_AAC failed\n", __FUNCTION__, __LINE__);
		return -1;
	}

	ctx_ = avcodec_alloc_context3(codec);
	if (!ctx_) {
		printf(" %s %d avcodec_alloc_context3 failed\n", __FUNCTION__, __LINE__);
		return -1;
	}

	ctx_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	ctx_->bit_rate = bit_rate_;
	ctx_->sample_rate = sample_rate_;
	ctx_->sample_fmt = AV_SAMPLE_FMT_FLTP;
	ctx_->channel_layout = av_get_default_channel_layout(ctx_->channels);

	int ret = -1;
	ret = avcodec_open2(ctx_, nullptr, nullptr);
	if (!ctx_) {
		printf(" %s %d avcodec_open2 failed\n", __FUNCTION__, __LINE__);
		char errBuf[1024] = { 0 };
		av_strerror(ret, errBuf, sizeof(errBuf) - 1);
		printf("avcodec_open2 failed:%s\n", errBuf);
		return -1;
	}

	printf("InitAAC success\n");
	return 0;
}

int AudioEncoder::InitMp3()
{
	return 0;
}

void AudioEncoder::DeInit()
{
	if (ctx_) {
		// codec_ctx ±»ÖÃÎª¿Õ
		avcodec_free_context(&ctx_);
	}

}

AVPacket* AudioEncoder::Encode(AVFrame* frame, int stream_index, int64_t pts, int64_t time_base)
{
	if (!ctx_) {
		printf(" %s %d ctx_ is nullptr\n", __FUNCTION__, __LINE__);
		return nullptr;
	}
	pts_ = av_rescale_q(pts, AVRational{ 1,(int)(time_base) }, ctx_->time_base);
	if (frame) {
		frame->pts = pts;
	}
	int ret = -1;
	ret = avcodec_send_frame(ctx_, frame);
	if (ret != 0) {
		char errBuf[1024] = { 0 };
		av_strerror(ret, errBuf, sizeof(errBuf) - 1);
		printf("%s %d avcodec_send_frame failed: %s\n", __FUNCTION__, __LINE__, errBuf);
		return nullptr;
	}

	AVPacket* pkt = av_packet_alloc();
	ret = avcodec_receive_packet(ctx_, pkt);
	if (ret != 0) {
		char errBuf[1024] = { 0 };
		av_strerror(ret, errBuf, sizeof(errBuf) - 1);
		printf("%s %d avcodec_receive_packet failed: %s\n", __FUNCTION__, __LINE__, errBuf);
		av_packet_free(&pkt);
		return nullptr;
	}

	pkt->stream_index = stream_index;

	return pkt;
}

int AudioEncoder::GetFrameSize()
{
	if (ctx_) {
		return ctx_->frame_size;
	}

	return 0;
}

int AudioEncoder::GetSampleFormat()
{
	if (ctx_) {
		return ctx_->sample_fmt;
	}

	// AV_SAMPLE_FMT_NONE
	return -1;
}
