#include "face.h"

// Facemark

cv::Ptr<cv::face::Facemark> facemarkHandles[10];

Facemark FacemarkLBF_New() {
    facemarkHandles[0] = cv::face::FacemarkLBF::create();
    return facemarkHandles[0];
}

void Facemark_Close(Facemark fm) {
}

void Facemark_LoadModel(Facemark fm, const char* name) {
    facemarkHandles[0]->loadModel(name);
}

Contours2f Facemark_Fit(Facemark fm, Mat img, Rects faces) {

    std::vector<cv::Rect> facesVec;

    for (size_t i = 0; i < faces.length; i++) {
        facesVec.push_back(cv::Rect(
            faces.rects[i].x,
            faces.rects[i].y,
            faces.rects[i].width,
            faces.rects[i].height));
    }

    std::vector<std::vector<cv::Point2f>> contours;

    facemarkHandles[0]->fit(*img, facesVec, contours);

    // fprintf(stdout, "faces=%d landmarks=%d\n", facesVector.size(), landmarks.size());

    Contour2f* points = new Contour2f[contours.size()];

    for (size_t i = 0; i < contours.size(); i++) {
        Point2f* pts = new Point2f[contours[i].size()];

        for (size_t j = 0; j < contours[i].size(); j++) {
            Point2f pt = {contours[i][j].x, contours[i][j].y};
            pts[j] = pt;
        }

        points[i] = (Contour2f){pts, (int)contours[i].size()};
    }

    Contours2f cons = {points, (int)contours.size()};
    return cons;
}