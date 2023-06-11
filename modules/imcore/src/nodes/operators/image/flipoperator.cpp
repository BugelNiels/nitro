#include "flipoperator.hpp"
#include <opencv2/imgproc.hpp>
#include "FLIP.h"
#include "color.h"

#include <QDebug>

FLIP::image<FLIP::color3> cvMatToFlipImg(const cv::Mat& img) {
    int width = img.cols;
    int height = img.rows;

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cv::Vec3b col = img.at<cv::Vec3b>(y, x);
            float r = col[2] / 255.0f;
            float g = col[1] / 255.0f;
            float b = col[0] / 255.0f;
            res.set(x, y, FLIP::color3(r, g, b));
        }
    }
    return res;
}

FLIP::image<FLIP::color3> cvGrayscaleMatToFlipImg(const cv::Mat& img) {
    int width = img.cols;
    int height = img.rows;

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        const uchar* row = img.ptr(y);
        for (int x = 0; x < width; x++) {
            float intensity = row[x] / 255.0f;
            res.set(x, y, FLIP::color3(intensity));
        }
    }
    return res;
}

static cv::Mat flipImgToCvMat(const FLIP::image<FLIP::color3>& img) {
    int width = img.getWidth();
    int height = img.getHeight();

    cv::Mat res(height, width, CV_8UC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            FLIP::color3 col = img.get(x, y);
            int r = 255 * col.r + 0.5f;
            int g = 255 * col.g + 0.5f;
            int b = 255 * col.b + 0.5f;
            res.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
        }
    }
    return res;
}

cv::Mat flipImgFloatToGrayMat(const FLIP::image<float>& img) {
    int width = img.getWidth();
    int height = img.getHeight();

    cv::Mat res(height, width, CV_8UC1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float value = img.get(x, y);
            res.at<uchar>(y, x) = static_cast<uchar>(value * 255.0f);
        }
    }
    return res;
}

inline static float calculatePPD(const double dist, const double resolutionX,
                                 const double monitorWidth) {
    return dist * (resolutionX / monitorWidth) * (M_PI / 180.0f);
}

// To prevent initializing this thing all the time
static FLIP::image<FLIP::color3> magmaMap(FLIP::MapMagma, 256);

void nitro::FlipOperator::execute(nitro::NodePorts &nodePorts, const std::map<QString, int> &options) const {
    bool im1Present, im2Present, sPres, resPres, distPres;
    auto im1 = nodePorts.getInputImage("Image1", im1Present);
    auto im2 = nodePorts.getInputImage("Image2", im2Present);
    double screenWidth = nodePorts.getInputValue("Width (m)", sPres);
    int resX = nodePorts.getInputInteger("Res (x)", resPres);
    double dist = nodePorts.getInputValue("Dist (m)", distPres);

    if (!im1Present || !im2Present || !sPres || !resPres || !distPres) {
        return;
    }

    qDebug() << screenWidth << resX << dist;

    FLIP::image<FLIP::color3> fImgA = im1->channels() == 1 ? cvGrayscaleMatToFlipImg(*im1) : cvMatToFlipImg(*im1);
    FLIP::image<FLIP::color3> fImgB = im2->channels() == 1 ? cvGrayscaleMatToFlipImg(*im2) : cvMatToFlipImg(*im2);
    int width = im1->rows;
    int height = im1->cols;

    float ppd = calculatePPD(dist, resX, screenWidth);
    FLIP::image<float> errMap(width, height, 0.0f);
    errMap.FLIP(fImgA, fImgB, ppd);

    FLIP::image<FLIP::color3> colResult(width, height);
    colResult.copyFloat2Color3(errMap);

    colResult.colorMap(errMap, magmaMap);

    nodePorts.setOutputImage("Error Map", std::make_shared<cv::Mat>(flipImgToCvMat(colResult)));
    nodePorts.setOutputImage("Errors", std::make_shared<cv::Mat>(flipImgFloatToGrayMat(errMap)));
}
