#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
/*
*	Descr: ��ȡ��̬Ŀ������
*	@param srcimg ��binaryMap ǰ����ֵͼ
*	@param rect_ ����̬��Ч����
*	@Author :Libin
*	@return :�ɹ�����true,ʧ�ܷ���false
*/
bool extractor(const cv::Mat binaryMap,std::vector<cv::Rect> &ret_rect, cv::Mat &mat);