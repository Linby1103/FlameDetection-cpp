/**
 * @file main.cpp
 * @date 2020/6/8 
 * @brief Flame detection 
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Vibe/vibe-background-sequential.h"
#include "ExtractRegion.h"
#include "flame_detector.h"
using namespace cv;
using namespace std;

void processVideo(char* videoFilename);

/**
 * Main program
 */
int main(int argc, char* argv[])
{
  /* Create GUI windows. */
  namedWindow("Frame");
  namedWindow("Segmentation by ViBe");
  string path = "D:/work/doc/CqHx-lsjN0yEaoEsAAAAAFeofcc669.mp4";
  
  processVideo((char*)path.c_str());


  /* Destroy GUI windows. */
  destroyAllWindows();
  return EXIT_SUCCESS;
}

/**
 * Processes the video. The code of ViBe is included here. 
 *videoFilename = 0x00000215a6aefbb0 "D:/work/code/workspace/BackgroundSplit-Open/Camera Road 01.avi"
 * @param videoFilename  The name of the input video file. 
 */
void processVideo(char* videoFilename)
{
  /* Create the capture object. */
  VideoCapture capture(videoFilename);

  if (!capture.isOpened()) {
    /* Error in opening the video input. */
    cerr << "Unable to open video file: " << videoFilename << endl;
    exit(EXIT_FAILURE);
  }
  /********************************
  *local Variables.  
  ********************************/
  static int frameNumber = 1; /* The current frame number */
  Mat frame;                  /* Current frame. */
  Mat segmentationMap;        /* Will contain the segmentation map. This is the binary output map. */
  int keyboard = 0;           /* Input from keyboard. Used to stop the program. Enter 'q' to quit. */

  
  /********************************
  * Model for ViBe.
  ********************************/
  vibeModel_Sequential_t *model = NULL; /* Model used by ViBe. */

  /* Read input data. ESC or 'q' for quitting. */
  while ((char)keyboard != 'q' && (char)keyboard != 27) {
    /* Read the current frame. */
    if (!capture.read(frame)) {
      cerr << "Unable to read next frame." << endl;
      cerr << "Exiting..." << endl;
      exit(EXIT_FAILURE);
    }

    if ((frameNumber % 100) == 0) { cout << "Frame number = " << frameNumber << endl; }

    /* Applying ViBe.
     * (1) remplace C1R by C1R in this file.
     * (2) uncomment the next line (cvtColor).
     */
	cv::Rect _rect_ = cv::Rect(50, 50, 1000,500);
	frame = frame(_rect_).clone();
	cv::Mat paint = frame.clone();
    cvtColor(frame, frame, CV_BGR2GRAY); 
	/*--------------------------------------------
	*Init vibeModel use first frame.
	---------------------------------------------*/
    if (frameNumber == 1) {
      segmentationMap = Mat(frame.rows, frame.cols, CV_8UC1);
	  /*--------------------------------------------
	  *Creat vibeModel.
	  ---------------------------------------------*/
      model = (vibeModel_Sequential_t*)libvibeModel_Sequential_New();
      //libvibeModel_Sequential_AllocInit_8u_C3R(model, frame.data, frame.cols, frame.rows);
	  /*--------------------------------------------
	  *Creat vibeModel.
	  ---------------------------------------------*/
	  libvibeModel_Sequential_AllocInit_8u_C1R(model, frame.data, frame.cols, frame.rows);
	}
    
	/*--------------------------------------------
	*ViBe: Segmentation and updating.
	---------------------------------------------*/

    //libvibeModel_Sequential_Segmentation_8u_C3R(model, frame.data, segmentationMap.data);
	libvibeModel_Sequential_Segmentation_8u_C1R(model, frame.data, segmentationMap.data);
	//libvibeModel_Sequential_Update_8u_C3R(model, frame.data, segmentationMap.data);
	libvibeModel_Sequential_Update_8u_C1R(model, frame.data, segmentationMap.data);
    
    medianBlur(segmentationMap, segmentationMap, 3); /* 3x3 median filtering */

	std::vector<cv::Rect> rect;
	std::vector<std::vector<cv::Point> > contours;

	/*--------------------------------------------
	*Extract moving target.
	---------------------------------------------*/
	extractor(segmentationMap, rect,paint);

	if (rect.size() > 0)
	{
		FlameDetector detector;

		cv::Rect rect_ = rect[0];
		/*--------------------------------------------
		*Extract the region of flame.
		---------------------------------------------*/
		detector.extract(paint, rect_);
		cv::rectangle(paint, rect_, Scalar(0, 255, 0), 2.0);

	}
		
    /* Shows the current frame and the segmentation map. */
    
   /* imshow("Segmentation by ViBe", segmentationMap);*/
	imshow("Frame", paint);

    ++frameNumber;

    /* Gets the input from the keyboard. */
    keyboard = waitKey(1);
  }

  /* Delete capture object. */
  capture.release();

  /* Frees the model. */
  libvibeModel_Sequential_Free(model);
  system("pause");
}

