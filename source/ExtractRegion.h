#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
/*
*	Descr: 提取动态目标区域
*	@param srcimg ：binaryMap 前景二值图
*	@param rect_ ：动态有效区域
*	@Author :Libin
*	@return :成功返回true,失败返回false
*/
bool extractor(const cv::Mat binaryMap,std::vector<cv::Rect> &ret_rect, cv::Mat &mat);