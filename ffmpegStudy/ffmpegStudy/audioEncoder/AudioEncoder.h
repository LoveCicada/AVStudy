
#pragma once

#include <string>

extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}


class AudioEncoder
{
public:
	AudioEncoder();
	~AudioEncoder();

	int InitAAC(int channels, int sample_rate, int bit_rate);
	int InitMp3();
	void DeInit();

	AVPacket* Encode(AVFrame* frame, int stream_index, int64_t pts, int64_t time_base);
	// ��ȡһ֡���� ÿ��ͨ����Ҫ���ٸ�������
	int GetFrameSize();
	// ��������Ҫ�Ĳ�����ʽ
	int GetSampleFormat();

private:
	int channels_ = 2;
	int sample_rate_ = 44100;
	int bit_rate_ = 128 * 1024;
	int64_t pts_ = 0;
	AVCodecContext* ctx_ = nullptr;
};