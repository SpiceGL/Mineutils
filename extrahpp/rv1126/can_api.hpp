#pragma once
#include"CanManager.h"
#include<chrono>
#include<iostream>


class CanCommunication
{
private:
	int can_id;
	int mode = 0;
	ui_package data_send;
	can_info data_get;
	
public:
	CanCommunication(int _can_id)
	{
		can_id = _can_id;
		data_send.info.buf_len = 8;
	}
	CanCommunication(const CanCommunication& _can) = delete;
	CanCommunication& operator=(const CanCommunication& _can) = delete;

	int init()
	{
		/*	mode: 0: 无模式   1: 只有白名单   2: 只有心跳名单   3: 白名单和心跳都有
			return: -1: 超时   0 : 设备没打开   1 : 正常
			!!!目前似乎只能使用0模式！
		*/

		return initDev(mode, NULL, NULL, can_id);
	}

	/*发送can数据*/
	int send(int info_id, const unsigned char (&in_buffer)[8])
	{
		/*	info_id: 数据id
		*	in_buffer: 数据，8位数组
		*	loop_time: 弃用
		*	delay_time: 弃用
		*/
		data_send.info.id = info_id;
		for (int i = 0; i < 8; ++i)
			data_send.info.buf[i] = in_buffer[i];
		data_send.loopTime = 1;
		data_send.delayTime = 1;
		return can_write(&data_send, can_id); 
	}

	/*根据data id取数据，但会丢弃其他id的数据*/
	int get(int info_id, unsigned char (&out_buffer)[8], int wait_time = 1000)
	{
		/*	return: 1 正常   0 设备未打开   -1 超时	*/
		auto start_t = std::chrono::high_resolution_clock::now();
		while (1)
		{
			int status = canread(&data_get, can_id);
			if (status > 0)
			{
				if (data_get.id == info_id)
				{
					for (int i = 0; i < 8; ++i)
						out_buffer[i] = data_get.buf[i];
					return 1;
				}
				auto now_t = std::chrono::high_resolution_clock::now();
				auto waited_time = std::chrono::duration_cast<std::chrono::milliseconds>(now_t - start_t).count();
				if (waited_time > wait_time)
					return -1;
			}
			else return 0;
		}
	}

	/*获取缓存队列中最旧的数据和id*/
	int read(int& out_buf_id, unsigned char (&out_buffer)[8])
	{
		/*	return: 1 正常   0 设备未打开	*/
		while (1)
		{
			int status = canread(&data_get, can_id);
			if (status > 0)
			{
				out_buf_id = data_get.id;
				for (int i = 0; i < 8; ++i)
					out_buffer[i] = data_get.buf[i];
				return 1;
			}
			else return 0;
		}
	}
};



