
#pragma once

#include <string>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}


class Muxer
{
public:
	Muxer();
	~Muxer();

	// 输出文件, 返回<0异常
	// 初始化
	int Init(const char* url);
	// 资源释放
	int DeInit();
	// 创建流
	int AddStream(AVCodecContext* ctx);

	// 写流
	int SendHeader();
	int SendPacket(AVPacket* pkt);
	int SendTrailer();

	// avio open
	int Open();

private:
	AVFormatContext* fmt_ctx_ = nullptr;
	std::string  url_ = "";
	// 编码器上下文 audio video
	AVCodecContext *acodec_ctx_ = nullptr;
	AVStream* astream_ = nullptr;
	AVCodecContext* vcodec_ctx_ = nullptr;
	AVStream* vstream_ = nullptr;

	//
	int audio_index = -1;
	int video_index = -1;

};