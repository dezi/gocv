#ifndef _OPENCV3_FACE_H_
#define _OPENCV3_FACE_H_

#include <stdbool.h>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
//#include <opencv2/face/facemark.hpp>
//#include <opencv2/face/facemarkLBF.hpp>
extern "C" {
#endif

#include "core.h"

#ifdef __cplusplus
//typedef cv::Ptr<cv::face::Facemark> Facemark;
typedef cv::face::Facemark* Facemark;
#else
typedef void* Facemark;
#endif

// FacemarkLBF
Facemark FacemarkLBF_New();
void Facemark_Close(Facemark fm);
void Facemark_LoadModel(Facemark fm, const char* name);
Contours2f Facemark_Fit(Facemark fm, Mat img, Rects rects);

#ifdef __cplusplus
}
#endif

#endif //_OPENCV3_FACE_H_
