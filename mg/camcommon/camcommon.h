#pragma once


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv/cvaux.h>


#include "../common/common.h"
using namespace common;

#include "../network/network.h"


using std::pair;


struct sSharedData
{
	int count;
	cv::Point2f pts[3];
};




#include "linecontour.h"
#include "CameraDistortion.h"
#include "camera.h"
#include "config.h"
#include "RecognitionConfig.h"
#include "recognitionedgeconfig.h"
#include "detectpoint.h"
 #include "rectcontour.h"
 #include "squarecontour.h"
 #include "roi.h"
 #include "screen.h"
// #include "sharedmemcommunication.h"
// #include "wndmsgcommunication.h"
// #include "udpcommunication.h"
// #include "dxcapture.h"
#include "movie.h"
#include "pidcontrol.h"
#include "skewdetect.h"
#include "streaming.h"
#include "streamingsender.h"
#include "streamingreceiver.h"
// #include "streamingviewer.h"
#include "searchpoint.h"
#include "thresholdfinder.h"
#include "LEDCalibrater.h"
//#include "LEDBlinkCapture.h"
#include "videowriter.h"
// #include "qrcode.h"
// #include "arqrcode.h"
#include "fps.h"
 #include "util.h"


