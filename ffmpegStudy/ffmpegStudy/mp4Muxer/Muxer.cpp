

#include "Muxer.h"

Muxer::Muxer()
{
}

Muxer::~Muxer()
{
}

int Muxer::Init(const char* url)
{
	int ret = -1;
	ret = avformat_alloc_output_context2(&fmt_ctx_, nullptr, nullptr, url);
	if (ret != 0){
		char errbuf[1024] = {0};
		av_strerror(ret, errbuf, sizeof(errbuf) -1);
		printf("avformat_alloc_output_context2 failed: %s\n", errbuf);
		return -1;
	}
	url_ = url;

	return 0;
}

int Muxer::DeInit()
{
	if (fmt_ctx_) {
		avformat_close_input(&fmt_ctx_);
		//fmt_ctx_ = nullptr;
	}
	url_ = "";
	acodec_ctx_ = nullptr;
	astream_ = nullptr;
	audio_index = -1;

	vcodec_ctx_ = nullptr;
	vstream_ = nullptr;
	video_index = -1;

	return 0;
}

int Muxer::AddStream(AVCodecContext* ctx)
{
	if (!fmt_ctx_) {
		printf("%s %d fmt_ctx_ is nullptr\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if (!ctx) {
		printf("%s %d ctx is nullptr\n", __FUNCTION__, __LINE__);
		return -1;
	}

	AVStream* st = avformat_new_stream(fmt_ctx_, nullptr);
	if (!st) {
		printf("%s %d avformat_new_stream failed\n", __FUNCTION__, __LINE__);
		return -1;
	}

	// 从编码器上下文复制
	avcodec_parameters_from_context(st->codecpar, ctx);
	av_dump_format(fmt_ctx_, 0, url_.c_str(), 1);

	// 判断当前的是视频流还是音频流
	if (ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
		acodec_ctx_ = ctx;
		astream_ = st;
		audio_index = st->index;
	}
	else if (ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
		vcodec_ctx_ = ctx;
		vstream_ = st;
		video_index = st->index;
	}


	return 0;
}

int Muxer::SendHeader()
{
	if (!fmt_ctx_) {
		printf("%s %d fmt ctx is nullptr\n", __FUNCTION__, __LINE__);
		return -1;
	}

	int ret = -1;
	ret = avformat_write_header(fmt_ctx_, nullptr);
	if (ret != 0) {
		char errbuf[1024] = { 0 };
		av_strerror(ret, errbuf, sizeof(errbuf) - 1);
		printf("avformat_alloc_output_context2 failed: %s\n", errbuf);
		return -1;
	}


	return 0;
}

int Muxer::SendPacket(AVPacket* pkt)
{
	int stream_index = pkt->stream_index;
	if (!pkt || pkt->size <= 0 || pkt->data) {

		printf("%s %d !pkt || pkt->size <= 0 || pkt->data\n", __FUNCTION__, __LINE__);
		if (pkt) {
			av_packet_free(&pkt);
		}
		return -1;
	}

	// 编码后的包
	AVRational src_time_base{ 1,1 };
	// mp4输出文件对应流的time_base
	AVRational dst_time_base{ 1,1 };
	if (vstream_ && vcodec_ctx_ && stream_index == video_index) {
		src_time_base = vcodec_ctx_->time_base;
		dst_time_base = vstream_->time_base;
	}else if (astream_ && acodec_ctx_ && stream_index == audio_index) {
		src_time_base = acodec_ctx_->time_base;
		dst_time_base = astream_->time_base;
	}

	// 时间基转换
	pkt->pts = av_rescale_q(pkt->pts, src_time_base, dst_time_base);
	pkt->dts = av_rescale_q(pkt->dts, src_time_base, dst_time_base);
	pkt->duration = av_rescale_q(pkt->duration, src_time_base, dst_time_base);

	int ret = 0;
	// 对packet进行缓存和pts检查
	ret = av_interleaved_write_frame(fmt_ctx_, pkt);
	// 输出一帧视音频数据，直接将包写进Mux，没有缓存和重新排序，一切都需要用户自己设置
	//ret = av_write_frame(fmt_ctx_, pkt);
	av_packet_free(&pkt);
	if (ret == 0) {
		return 0;
	}
	else{
		char errbuf[1024] = { 0 };
		av_strerror(ret, errbuf, sizeof(errbuf) - 1);
		printf("avformat_alloc_output_context2 failed: %s\n", errbuf);
		return -1;
	}
}

int Muxer::SendTrailer()
{
	if (!fmt_ctx_) {
		printf("%s %d fmt ctx is nullptr\n", __FUNCTION__, __LINE__);
		return -1;
	}

	int ret = -1;
	ret = av_write_trailer(fmt_ctx_);
	if (ret != 0) {
		char errbuf[1024] = { 0 };
		av_strerror(ret, errbuf, sizeof(errbuf) - 1);
		printf("avformat_alloc_output_context2 failed: %s\n", errbuf);
		return -1;
	}



	return 0;
}

int Muxer::Open()
{
	return 0;
}
