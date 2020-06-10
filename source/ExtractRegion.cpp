/*****************************************************************************                                                                            *
*  @file     ExtractRegion.cpp                                                       *
*  @brief    对文件的简述                                                    *
*  Details.                                                                  *                                                                  *
*  @author   Henry.Wen                                                       *
*  @email    renhuabest@163.com                                              *
*  @version  1.0.0.1(版本号)                                                 *
*  @date     renhuabest@163.com                                              *
*  @license  GNU General Public License(GPL)                                 *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History : *
*  <Date> | <Version> | <Author> | <Description>                   *
*----------------------------------------------------------------------------*
*  2020 / 06 / 6 | 1.0.0.1 | LiBin | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#include <map>
#include "ExtractRegion.h"
#define EPSINON  1e-5

/********************************
* @brief 计算IOU
* @param rect1，rect2 矩形框
*     -<em>false</em> -99999 or 
*     -<em>true</em> iou
********************************/
static float IOU(const cv::Rect &rect1, const cv::Rect &rect2)
{
	if ((rect1.x < 0 || rect1.y < 0 || rect1.width < 0 || rect1.height < 0) || \
		(rect2.x < 0 || rect2.y < 0 || rect2.width < 0 || rect2.height < 0)|| \
		rect1.area()<=0 || rect2.area()<=0)
		return -99999;
	cv::Rect union_ = rect1 | rect2;//example 7&1=1 b(7)=0111 b(1)=0001 0111 & 0001= 0001
	cv::Rect inter_ = rect1 & rect2;//inter_

	return inter_.area()*1.0 / union_.area();
	
}
/*-------------------------------------
*Merge effective region 
**************************************/
static void coormerge(std::vector<cv::Rect> & rect_, cv::Rect & roi)
{
	if (rect_.size() == 0)
	{
		std::cout << "Found rect_ size =" << rect_.size() << std::endl;
		return;
	}
	cv::Rect &rect = rect_[0];
	cv::Rect effRegion = cv::Rect(-1, -1, 0, 0);


}
static void NMS( std::vector<cv::Rect> &rect_, std::vector<cv::Rect> & ret_rect)
{
	std::vector<cv::Rect> block,sblock;

	if (rect_.size() == 0)
	{
		std::cout << "Rect size = " << rect_.size() << std::endl;
		return;
	}

	int cur_size = 0;
	while(true)
	{
		int pre_size = rect_.size();

		if (rect_.size() == 1)
		{
			block.push_back(rect_[0]);
			rect_.erase(rect_.begin());
			break;
		}
		else if (rect_.size() == 0)
		{
			break;
		}
		
		cv::Rect &rect = rect_[0];//面积最大的box

		for (size_t j = 1;j < rect_.size();++j)
		{

			float iou = IOU(rect, rect_[j]);
			
			if ((iou >= -EPSINON) && (iou <= EPSINON))//不相交
			{
				sblock.push_back(rect_[j]);
				continue;
			}
				
			else
			{
				if (std::min(rect.width, rect.height)>5)
				    rect = rect | rect_[j];
				//删除指定元素
				rect_.erase(rect_.begin() + j);
			}
		}

		cur_size = rect_.size();
		if (std::min(rect.width, rect.height)>5)
		    block.push_back(rect);

		rect_.erase(rect_.begin());//擦除最大的元素

		rect_.clear();
		rect_ = sblock;
		sblock.clear();
		
	}
	ret_rect = block;
}

static bool compare(const cv::Rect &elem1, const cv::Rect &elem2)
{
	
	return elem1.area() > elem2.area();
}
/********************************
* @brief 提取ROI区域
* @param binaryMap   
* @param _rect  roi 坐标      
* @return 返回说明
*     -<em>false</em> fail
*     -<em>true</em> succeed
********************************/
bool extractor(const cv::Mat binaryMap,std::vector<cv::Rect> &ret_rect, cv::Mat &mat)
{
	int scale_szie = 2;
	assert(binaryMap.data != NULL);
	std::vector<cv::Rect> bbox,nms_bbox;
	int image_width = binaryMap.cols;
	int image_height = binaryMap.rows;
	

	std::vector<std::vector<cv::Point> > contours, vaildcontours;
	
	std::vector<cv::Vec4i> hierarchy;
	/*-------------------------------------
	*extractor contours
	--------------------------------------*/
	cv::Mat Search = cv::Mat();
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

	morphologyEx(binaryMap, Search, cv::MORPH_CLOSE, element);
	imshow("Segmentation by ViBe", Search);
	

	findContours(Search, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	if (contours.size() == 0)
	{
		std::cout << "Can't find contour!" << std::endl;
		return false;
	}

	int index = 0;

	for (; index >= 0; index = hierarchy[index][0])
	{
		cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
		//drawContours(mat, contours, index, color, 1, 8, hierarchy);
	}
	

	for (size_t i = 0; i < contours.size(); i++)
	{
		
		//if (contours[i].size() < 7 || contours[i].size()>int(image_height*0.75)) continue;
		if (contours[i].size() < 7 ) continue;
		cv::Rect box = boundingRect(contours[i]);
		if (std::max(box.width, box.height) / std::min(box.width, box.height) > 2*std::min(box.width, box.height)) continue;
		bbox.push_back(box);
		std::cout << "contours size="<<contours[i].size() << "\t" << std::endl;

		vaildcontours.push_back(contours[i]);
	}
	if (bbox.size() == 0)
	{
		std::cout << "Size of bbox=" << bbox.size() << std::endl;
		return false;
	}
	sort(bbox.begin(), bbox.end(),compare);
	

	std::vector<cv::Rect> block_rect;
	block_rect.clear();
	NMS(bbox, block_rect);
	
	cv::Rect &maxRect = block_rect[0];
	double Scale[2] = { -15,-15 };//Two scale Rect

	/**************************************
	*对Rect 放大两个尺度，用已经找到的有效区域
	**************************************/
	for (int s = 0;s < scale_szie;s++)
	{
		std::vector<cv::Rect> block;
		maxRect.x += Scale[s];
		maxRect.y += Scale[s];
		maxRect.width += 2*abs(Scale[s]);
		maxRect.height += 2*abs(Scale[s]);
		NMS(block_rect, block);
		block_rect.clear();
		block_rect = block;
	}
	
	ret_rect.push_back(block_rect[0]);
}