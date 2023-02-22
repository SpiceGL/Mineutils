//在一个项目中只能被一个cpp调用
#pragma once
#include"CamOpera.h"
#include"CamViewer.h"
#include"app_inc.h"
#include<gpio.h>
#include<chrono>
#include<string>
#include<vector>

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
	CamViewer* _viewer = nullptr;

	KiloCams() {}

	void init(int chan_num = 1, TYPE_RESOLUTION group1_type=TYPE_720P_30, TYPE_RESOLUTION group2_type=TYPE_720P_30)   //支持多摄像头同时调用
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
	}

	void setViewer(int chan_id, LTRBBox<unsigned int> rect, bool is_rgb)   //也许rgb和yuv上屏不能一起，暂不确定
	{
		//设置rgb上屏，rgb和bgr设置疑似反了，图像是RGB的时候反而应该设置IMAGE_TYPE_BGR888
		_rects[chan_id] = { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
		if (is_rgb)
			addRgbConfig(chan_id, CAM_WIDTH, CAM_HEIGHT, IMAGE_TYPE_BGR888, _viewer, &(_rects[chan_id]));
		else addCamConfig(_cams[chan_id], _viewer, &(_rects[chan_id]));
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

	//在yuv_callback和enc_callback中使用
	int getRgbData(int chan_id, void* mb, unsigned char* data)
	{
		CamOpera* cam = _cams[chan_id];
		int w, h;
		int len = cam->process_yuv_frame(mb, data, &w, &h);
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



