//在一个项目中只能被一个cpp调用
#pragma once
#include<gpio.h>
#include<chrono>
#include<string>
#include<vector>

#include"CamOpera.h"
#include"CamViewer.h"
#include"app_inc.h"
#include"rtmpPushStream.h"


#include"mineutilshpp/__cvutils__.h"


using std::string;
using std::vector;
using namespace mineutils;


//根据显示屏设置
#define DISP_WIDTH 1024
#define DISP_HEIGHT 768

#define CAM_WIDTH 1280
#define CAM_HEIGHT 720


void yuv_callback(void* mb, int chan_id);   //请手动实现
void enc_callback(void* mb, int chan_id);   //请手动实现


class KiloCams
{
private:
	int _chan_num;
	int _cam_w, _cam_h;   //图像宽和高

public:
	vector<CamOpera*> _cams = { NULL };
	vector<RECT_S> _rects;
	vector<RTMP_CLIENT*> _rtmps = { NULL };
	CamViewer* _viewer = nullptr;

	KiloCams() {}

	void init(int chan_num = 1, bool need_push_stream = false, TYPE_RESOLUTION group1_type = TYPE_720P_30, TYPE_RESOLUTION group2_type = TYPE_720P_30)   //支持多摄像头同时调用
	{
		//int cams_array_size = sizeof(CamOpera*) * chan_num;
		//m_cams = (CamOpera**)malloc(chan_num);
		//memset(m_cams, 0, cams_array_size);
		_cams.resize(chan_num, NULL);
		_rects.resize(chan_num);
		_chan_num = chan_num;
		CamOpera::global_init(group1_type, group2_type);//根据摄像头设置
		CamOpera::config_bg_data(0x40);
		_viewer = new CamViewer(DISP_WIDTH, DISP_HEIGHT);
		if (need_push_stream)
		{
			_rtmps.resize(chan_num, NULL);
			net_stream_initStreamLib();
			app_startRtsp();
		}
	}

	KiloCams(const KiloCams& temp) = delete;
	KiloCams& operator=(const KiloCams& temp) = delete;

	~KiloCams()
	{
		_viewer->stop_viewer();
		_viewer->clear_all_cams();
		for (CamOpera* cam_ptr : _cams)
		{
			if (cam_ptr != nullptr)
			{
				delete cam_ptr;
				cam_ptr = nullptr;
			}
		}
		for (RTMP_CLIENT* rtmp_ptr : _rtmps)
		{
			if (rtmp_ptr != nullptr)
			{
				delete rtmp_ptr;
				rtmp_ptr = nullptr;
			}
		}
		if (_viewer != nullptr)
		{
			delete _viewer;
			_viewer = nullptr;
		}
	}

	void useDefaultSettings()
	{
		CamOpera* cam_ptr = new CamOpera(0, 0);
		_cams[0] = cam_ptr;

		cam_ptr->set_cbs(yuv_callback, enc_callback);
		cam_ptr->get_resolution(_cam_w, _cam_h);
		cam_ptr->set_bitrate(_cam_w * _cam_h * 25 / 7);
		cam_ptr->set_gop(5);
		cam_ptr->start_stream(false);

		RECT_S rect0 = { 0, 0, DISP_WIDTH, DISP_HEIGHT };
		//add_cam_config(m_cams[0], m_viewer, &rect0);   //设置yuv直接上屏
		//设置rgb上屏，rgb和bgr设置疑似反了，图像是RGB的时候反而应该设置IMAGE_TYPE_BGR888
		addRgbConfig(0, CAM_WIDTH, CAM_HEIGHT, IMAGE_TYPE_BGR888, _viewer, &rect0);

	}

	void setCam(int cam_id, int chan_id)
	{
		CamOpera* cam_ptr = new CamOpera(chan_id, cam_id);
		_cams[chan_id] = cam_ptr;

		cam_ptr->set_cbs(yuv_callback, enc_callback);
		cam_ptr->get_resolution(_cam_w, _cam_h);
		cam_ptr->set_bitrate(_cam_w * _cam_h * 25 / 7);
		cam_ptr->set_gop(5);
		cam_ptr->start_stream(false);
	}

	void setViewer(int chan_id, LTRBBox<unsigned int> rect, bool is_rgb)   //也许rgb和yuv上屏不能一起，暂不确定
	{
		//设置rgb上屏，rgb和bgr设置疑似反了，图像是RGB的时候反而应该设置IMAGE_TYPE_BGR888
		_rects[chan_id] = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
		if (is_rgb)
			addRgbConfig(chan_id, CAM_WIDTH, CAM_HEIGHT, IMAGE_TYPE_BGR888, _viewer, &(_rects[chan_id]));
		else addCamConfig(_cams[chan_id], _viewer, &(_rects[chan_id]));
	}

	template<size_t N>
	void setRtmpStream(int chan_id, const char(&url)[N])
	{
		RTMP_CLIENT* rtmp_ptr = new RTMP_CLIENT(const_cast<char*>(url), chan_id);
		_rtmps[chan_id] = rtmp_ptr;
		rtmp_ptr->StartPushStream();
	}

	//开启kilo摄像头子线程
	void start()
	{
		for (CamOpera* cam : _cams)
		{
			// 当参数为false时，用户能在yuv_callback里修改yuv数据，且最终会反映在编码里。
			cam->start_stream();
			print("-----------------------------start_stream-------------------------------");
		}
		_viewer->start_viewer();
	}

	void stop()
	{
		_viewer->stop_viewer();
		_viewer->clear_all_cams();
	}

	//在yuv_callback中使用
	int getRgbData(int chan_id, void* mb, unsigned char* out_data)
	{
		CamOpera* cam = _cams[chan_id];
		int w, h;
		int len = cam->process_yuv_frame(mb, out_data, &w, &h);
		return len;
	}

	//在enc_callback中使用
	int getH264Data(int chan_id, void* mb, unsigned char* out_data)
	{
		CamOpera* cam = _cams[chan_id];
		int len = cam->process_enc_frame(mb, out_data, chan_id);
		return len;
	}

	void sendRgbData(int chan_id, unsigned char* data, int len)
	{
		_viewer->send_yuv_data(chan_id, data, len);
	}

	void sendYuvData(int chan_id, void* mb)
	{
		_viewer->send_yuv_data(chan_id, (unsigned char*)RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb));
	}

	//推流mb数据
	void sendH264Stream(int chan_id, unsigned char* h264_data)
	{
		av_frame_t frame;
		memcpy(&frame.frame_head, h264_data, sizeof(av_frame_head_t));
		frame.frame_data = h264_data + sizeof(av_frame_head_t);
		app_sendDataForRstp(chan_id, 0, &frame);
	}

private:

	static void addCamConfig(CamOpera* cam, CamViewer* viewer, RECT_S* rect)
	{
		int chan_id = cam->get_channel_id();;
		IMAGE_TYPE_E type = cam->get_format();
		int w, h;
		cam->get_resolution(w, h);
		viewer->add_cam(chan_id, false, w, h, type, *rect);
	}

	/* RGB类型上屏 */
	static void addRgbConfig(int chan_id, int w, int h, IMAGE_TYPE_E type, CamViewer* viewer, RECT_S* rect)
	{
		viewer->add_cam(chan_id, true, w, h, type, *rect);
	}
};



