#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
class FlameDetector {
public:
	FlameDetector() { targetpixels.clear();}
	/*
	*	Descr: ��ȡ��������
	*	@param srcimg ��ԭʼRGB image(����)
	*	@param rect_ ��������Ч����
	*	@Author :Libin
	*	@return :�ɹ�����true,ʧ�ܷ���false
	*/
	bool extract(cv::Mat &srcimg, cv::Rect &rect_);

	/*
	*	Descr: ͳ�ƻ������ɫ������Ϣ
	*	@param cv::Mat & �����ݻ�����Ч�����ȡ��roi
	*	@param std::vector<cv::Point> & �����������Ӧ���ص�����
	*	@Author :Libin
	*	@return :�ɹ�����true,ʧ�ܷ���false
	*/

	bool StatisticPiexl(cv::Mat &, std::vector<cv::Point> &);
	~FlameDetector() { targetpixels.clear(); }
	/*
	*	Descr: �������
	*	@param img ��
	*	@param rect_ ��
	*	@Author :Libin
	*	@return :�ɹ�����true,ʧ�ܷ���false
	*/
	void PoinCorrect(cv::Mat &img, cv::Rect &rect_);
private:
	std::vector<cv::Point> targetpixels;
};

