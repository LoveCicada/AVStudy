
#pragma once

#include <string>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}


class VideoEncoder
{
public:
	VideoEncoder();
	~VideoEncoder();

	int InitH264(int width, int height, int fps, int bit_rate);
	void DeInit();

	AVPacket* Encode(uint8_t* yuv_data, int yuv_size,
					int stream_index, int64_t pts, int64_t time_base);

private:
	int width_ = 0;
	int height_ = 0;
	int fps_ = 25;
	int bit_rate_ = 500 * 1024;
	int64_t pts_ = 0;
	AVCodecContext* ctx_ = nullptr;
};