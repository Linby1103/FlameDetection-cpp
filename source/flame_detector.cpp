#include "flame_detector.h"
void FlameDetector::PoinCorrect(cv::Mat &img, cv::Rect &rect_)
{
	cv::Point tl = rect_.tl();
	cv::Point br = rect_.br();
	if (tl.x < 0) tl.x=0;
	if (tl.y < 0) tl.y = 0;
	if (br.x  > img.cols - 1) br.x=img.cols - 1;
	if (br.y  > img.rows - 1) br.y = img.rows - 1;
	rect_ = cv::Rect(tl, br);
}
bool FlameDetector::extract(cv::Mat &srcimg, cv::Rect &rect_)
{
	if (srcimg.data == NULL)
	{
		std::cout << "Image data is null!" << std::endl;
		return false;
	}
	PoinCorrect(srcimg, rect_);
	cv::Mat tar_region= srcimg(rect_).clone();
	StatisticPiexl(tar_region, targetpixels);
	if (targetpixels.size() == 0)
	{
		std::cout << "Gets " << targetpixels.size() << "pixels";
		return false;
	}
	/****************************************
	*坐标映射到原图
	****************************************/
	
	for (int k = 0;k < targetpixels.size();++k)
	{
		cv::Point p1= targetpixels[k] + rect_.tl();
		
		cv::circle(srcimg, p1, 1, (0, 255, 255));
	}

	cv::Rect true_rect = boundingRect(targetpixels);
	rect_.x += true_rect.x;
	rect_.y += true_rect.y;
	rect_.width = true_rect.width;
	rect_.height = true_rect.height;
	PoinCorrect(srcimg, rect_);
}

bool FlameDetector::StatisticPiexl(cv::Mat &bgrimg, std::vector<cv::Point> &pixels)
{
	int red_t = 156; // 红色分量阈值  
	int saturation_t = 7; //亮度阈值 

	if (bgrimg.channels() < 3)
	{
		std::cout << "Image channels=" << bgrimg.channels() << std::endl;
		return false;
	}
	cv::Mat draw = cv::Mat(bgrimg.size(), CV_8UC1);
	cv::Mat SplitColor[3];
	split(bgrimg, SplitColor);

	int img_w = bgrimg.cols;
	int img_h = bgrimg.rows;
	for (int i = 0; i < img_h; i++)
	{
		for (int j = 0; j < img_w; j++)
		{
			/**************************************
			*统计各分量颜色信息
			***************************************/
			float b_color, g_color, r_color;
			b_color = SplitColor[0].at<uchar>(i, j);
			g_color = SplitColor[1].at<uchar>(i, j);
			r_color = SplitColor[2].at<uchar>(i, j);

			float maxValue = std::max(std::max(b_color, g_color), r_color);
			float minValue = std::min(std::min(b_color, g_color), r_color);
			//转S分量(饱和度)
			double S = (1 - 3.0*minValue / (b_color + g_color + r_color));//
			
		   /*********************************************
		   *condition :  1、ed > r_threshold  2、red>=green>=blue 3、 S>=((255-R)*ST/RT)
		   **********************************************/
			if (r_color > red_t &&r_color >= g_color && g_color >= b_color && \
				S > ((255 - r_color) * saturation_t / red_t))
			{
				pixels.push_back(cv::Point(j, i));
			}
		}
	}

	return true;
}
	
	
	
