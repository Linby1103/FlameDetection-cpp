#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
class FlameDetector {
public:
	FlameDetector() { targetpixels.clear();}
	/*
	*	Descr: 提取火焰区域
	*	@param srcimg ：原始RGB image(输入)
	*	@param rect_ ：火焰有效区域
	*	@Author :Libin
	*	@return :成功返回true,失败返回false
	*/
	bool extract(cv::Mat &srcimg, cv::Rect &rect_);

	/*
	*	Descr: 统计火焰的颜色分量信息
	*	@param cv::Mat & ：根据火焰有效区域扣取的roi
	*	@param std::vector<cv::Point> & ：火焰区域对应像素点坐标
	*	@Author :Libin
	*	@return :成功返回true,失败返回false
	*/

	bool StatisticPiexl(cv::Mat &, std::vector<cv::Point> &);
	~FlameDetector() { targetpixels.clear(); }
	/*
	*	Descr: 坐标矫正
	*	@param img ：
	*	@param rect_ ：
	*	@Author :Libin
	*	@return :成功返回true,失败返回false
	*/
	void PoinCorrect(cv::Mat &img, cv::Rect &rect_);
private:
	std::vector<cv::Point> targetpixels;
};

