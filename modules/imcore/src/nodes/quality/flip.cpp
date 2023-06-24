#include "flip.hpp"
#include <opencv2/imgproc.hpp>
#include "FLIP.h"
#include "color.h"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/colimagedata.hpp"
#include "nodes/datatypes/grayimagedata.hpp"

#include <QDebug>

#define INPUT_IMAGE_1 "Image"
#define INPUT_IMAGE_2 "Reference"
#define INPUT_X "Width (m)"
#define INPUT_RES "Res (x)"
#define INPUT_DIST "Dist (m)"
#define OUTPUT_RESULT "Result"
#define OUTPUT_ERROR_MAP "Errors"
#define MODE_DROPDOWN "Mode"

static FLIP::image<FLIP::color3> cvMatToFlipImg(const cv::Mat &img) {
    int width = img.cols;
    int height = img.rows;

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cv::Vec3f col = img.at<cv::Vec3f>(y, x);
            float r = col[2];
            float g = col[1];
            float b = col[0];
            res.set(x, y, FLIP::color3(r, g, b));
        }
    }
    return res;
}

static FLIP::image<FLIP::color3> cvGrayscaleMatToFlipImg(const cv::Mat &img) {
    int width = img.cols;
    int height = img.rows;

    FLIP::image<FLIP::color3> res(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float intensity = img.at<float>(y, x);
            res.set(x, y, FLIP::color3(intensity));
        }
    }
    return res;
}

static cv::Mat flipImgToCvMat(const FLIP::image<FLIP::color3> &img) {
    int width = img.getWidth();
    int height = img.getHeight();

    cv::Mat res(height, width, CV_32FC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            FLIP::color3 col = img.get(x, y);
            res.at<cv::Vec3f>(y, x) = cv::Vec3f(col.b, col.g, col.r);
        }
    }
    return res;
}

static cv::Mat flipImgFloatToGrayMat(const FLIP::image<float> &img) {
    int width = img.getWidth();
    int height = img.getHeight();

    cv::Mat res(height, width, CV_32F);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float value = img.get(x, y);
            res.at<float>(y, x) = value;
        }
    }
    return res;
}

inline static float calculatePPD(const double dist, const double resolutionX,
                                 const double monitorWidth) {
    return dist * (resolutionX / monitorWidth) * (M_PI / 180.0f);
}

// To prevent initializing this thing all the time


void
nitro::FlipOperator::execute(NodePorts &nodePorts) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_1);
    auto im2 = nodePorts.inGetAs<ColImageData>(INPUT_IMAGE_2);
    double screenWidth = nodePorts.inputValue(INPUT_X);
    int resX = nodePorts.inputInteger(INPUT_RES);
    double dist = nodePorts.inputValue(INPUT_DIST);

    FLIP::image<FLIP::color3> fImgA = im1->channels() == 1 ? cvGrayscaleMatToFlipImg(*im1) : cvMatToFlipImg(*im1);
    FLIP::image<FLIP::color3> fImgB = im2->channels() == 1 ? cvGrayscaleMatToFlipImg(*im2) : cvMatToFlipImg(*im2);
    int width = im1->cols;
    int height = im1->rows;

    float ppd = calculatePPD(dist, resX, screenWidth);
    FLIP::image<float> errMap(width, height, 0.0f);
    errMap.FLIP(fImgA, fImgB, ppd);

    FLIP::image<FLIP::color3> colResult(width, height);
    colResult.copyFloat2Color3(errMap);
    FLIP::image<FLIP::color3> magmaMap(FLIP::MapMagma, 256);
    colResult.colorMap(errMap, magmaMap);

    nodePorts.output<ColImageData>(OUTPUT_RESULT, flipImgToCvMat(colResult));
    nodePorts.output<GrayImageData>(OUTPUT_ERROR_MAP, flipImgFloatToGrayMat(errMap));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FlipOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("NVIDIA FLIP", "nvidiaFlip", category);
        return builder.
                withOperator(std::make_unique<nitro::FlipOperator>())->
                withIcon("compare.png")->
                withNodeColor({118, 185, 0})->
                withInputPort<ColImageData>(INPUT_IMAGE_1)->
                withInputPort<ColImageData>(INPUT_IMAGE_2)->
                withInputValue(INPUT_X, 0.7, 0.1, 2, BoundMode::LOWER_ONLY)->
                withInputInteger(INPUT_RES, 1920, 256, 4096, BoundMode::LOWER_ONLY)->
                withInputValue(INPUT_DIST, 0.7, 0, 3, BoundMode::LOWER_ONLY)->
                withOutputPort<ColImageData>(OUTPUT_RESULT)->
                withOutputPort<GrayImageData>(OUTPUT_ERROR_MAP)->
                build();
    };
}
