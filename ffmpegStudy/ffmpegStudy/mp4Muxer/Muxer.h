
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

	// ����ļ�, ����<0�쳣
	// ��ʼ��
	int Init(const char* url);
	// ��Դ�ͷ�
	int DeInit();
	// ������
	int AddStream(AVCodecContext* ctx);

	// д��
	int SendHeader();
	int SendPacket(AVPacket* pkt);
	int SendTrailer();

	// avio open
	int Open();

private:
	AVFormatContext* fmt_ctx_ = nullptr;
	std::string  url_ = "";
	// ������������ audio video
	AVCodecContext *acodec_ctx_ = nullptr;
	AVStream* astream_ = nullptr;
	AVCodecContext* vcodec_ctx_ = nullptr;
	AVStream* vstream_ = nullptr;

	//
	int audio_index = -1;
	int video_index = -1;

};