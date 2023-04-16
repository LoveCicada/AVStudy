
#include "VideoEncoder.h"

VideoEncoder::VideoEncoder()
{
}

VideoEncoder::~VideoEncoder()
{
	if (ctx_) {
		DeInit();
	}
}

int VideoEncoder::InitH264(int width, int height, int fps, int bit_rate)
{
	width_ = width;
	height_ = height;
	fps_ = fps;
	bit_rate_ = bit_rate;

	AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	if (!codec) {
		printf("%s %d avcodec_find_encoder failed AV_CODEC_ID_H264\n", __FUNCTION__, __LINE__);
		return -1;
	}

	ctx_ = avcodec_alloc_context3(codec);


	return 0;
}

void VideoEncoder::DeInit()
{
	if (ctx_) {
		avcodec_free_context(&ctx_);

	}
}

AVPacket* VideoEncoder::Encode(uint8_t* yuv_data, int yuv_size, int stream_index, int64_t pts, int64_t time_base)
{
	return nullptr;
}
