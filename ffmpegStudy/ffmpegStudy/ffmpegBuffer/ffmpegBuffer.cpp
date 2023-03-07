

#include <iostream>
#include <functional>

#ifdef __cplusplus
extern "C"
{
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}
#endif // __cplusplus


#define MEM_ITEM_SIZE (20*1024*1024)
#define AVPACKET_LOOP_COUNT 1000

typedef std::unique_ptr<AVFrame, std::function<void(AVFrame*)>> AVFramePtr;

class AVFrameRef final
{
public:
    static void Release(void* pRef)
    {
        delete (AVFrameRef*)pRef;
    }

public:
    AVFrameRef(AVFramePtr&& pObject)
        : m_pObject(std::move(pObject))
    {
    }

private:
    AVFramePtr m_pObject;
};

inline AVFramePtr AllocAVFrame()
{
    return AVFramePtr(av_frame_alloc(),
        [](AVFrame* pObject)
        {
            av_frame_free(&pObject);
        });
}

typedef std::unique_ptr<AVPacket, std::function<void(AVPacket*)>> AVPacketPtr;

inline AVPacketPtr AllocAVPacket()
{
    return AVPacketPtr(av_packet_alloc(),
        [](AVPacket* pObject)
        {
            av_packet_free(&pObject);
        });
}

/*
* @brief 测试内存泄漏
* 测试av_packet_alloc和av_packet_free的配对使用
*/
void av_packet_test1()
{
	std::cout << __FUNCTION__ << std::endl;

	AVPacket* pkt = nullptr;
	int ret = 0;

	pkt = av_packet_alloc();
	ret = av_new_packet(pkt, MEM_ITEM_SIZE);
	//memcpy(pkt->data, (void*)&av_pack)
	memset(pkt->data, 0, pkt->size);
	av_packet_unref(pkt);
	// 如果不free将会发生内存泄漏
	av_packet_free(&pkt);

}

void av_packet_test2()
{
    AVPacket* pkt = nullptr;
    AVPacket* pkt2 = nullptr;
    int ret = 0;

    pkt = av_packet_alloc();
    ret = av_new_packet(pkt, MEM_ITEM_SIZE);
    memset(pkt->data, 0, pkt->size);
    pkt2 = av_packet_alloc();
    av_packet_move_ref(pkt2, pkt);
    av_init_packet(pkt);
    av_packet_free(&pkt);
    av_packet_free(&pkt2);

}

void av_packet_test3()
{

}

void av_packet_test4()
{
    AVPacket* pkt = nullptr;
    AVPacket* pkt2 = nullptr;
    int ret = 0;
    pkt = av_packet_alloc();
    if (pkt && pkt->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt) = "
            << av_buffer_get_ref_count(pkt->buf) << std::endl;
    }

    ret = av_new_packet(pkt, MEM_ITEM_SIZE);
    if (pkt && pkt->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt) = "
            << av_buffer_get_ref_count(pkt->buf) << std::endl;
    }

    if (!pkt)
        return;

    memset(pkt->data, 0, pkt->size);

    pkt2 = av_packet_alloc();
    av_packet_move_ref(pkt2, pkt);
    av_packet_ref(pkt, pkt2);
    av_packet_ref(pkt, pkt2);
    if (pkt && pkt->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt) = "
            << av_buffer_get_ref_count(pkt->buf) << std::endl;
    }
    if (pkt2 && pkt2->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt2) = "
            << av_buffer_get_ref_count(pkt2->buf) << std::endl;
    }

    av_packet_unref(pkt);
    av_packet_unref(pkt);
    if (pkt && pkt->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt) = "
            << av_buffer_get_ref_count(pkt->buf) << std::endl;
    }
    if (pkt2 && pkt2->buf)
    {
        std::cout << __FUNCTION__ << " " << __LINE__ << " ref_count(pkt2) = "
            << av_buffer_get_ref_count(pkt2->buf) << std::endl;
    }
}

void av_frame_test1()
{
    AVFrame* frame = nullptr;
    int ret = 0;
    frame = av_frame_alloc();
    // 1024 * 2 * (16/8)
    frame->nb_samples = 1024;
    frame->format = AV_SAMPLE_FMT_S16;
    frame->channel_layout = AV_CH_LAYOUT_STEREO;
    // 根据格式分配内存
    ret = av_frame_get_buffer(frame, 0);
    if (frame->buf && frame->buf[0]) {
        printf("l frame->buf[0]->size = %d\n", frame->buf[0]->size);
    }

    if (frame->buf && frame->buf[1]) {
        printf("l frame->buf[1]->size = %d\n", frame->buf[1]->size);
    }

    //打印referenc-counted, 必须保证传入的是有效指针
    if (frame->buf && frame->buf[0]) {
        printf("ref_count1(frame) = %d\n", av_buffer_get_ref_count(frame->buf[0]));
    }

    // 当frame本身为空时不能make_writable
    ret = av_frame_make_writable(frame);
    printf("av_frame_make_writeable ret = %d\n", ret);
    if (frame->buf && frame->buf[0]) {
        printf("ref_count1(frame) = %d\n", av_buffer_get_ref_count(frame->buf[0]));
    }

    av_frame_free(&frame);

}

void test()
{
	//av_packet_test1();
    //av_packet_test2();
    //av_packet_test3();
    //av_packet_test4();
    av_frame_test1();


}


int main(int argc, char* argv[])
{
	test();


	return 0;
}
