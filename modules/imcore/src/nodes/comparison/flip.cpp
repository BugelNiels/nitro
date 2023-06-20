#include "flip.hpp"
#include <opencv2/imgproc.hpp>
#include "FLIP.h"
#include "color.h"
#include "nodes/nitronodebuilder.hpp"
#include "nodes/datatypes/imagedata.hpp"

#include <QDebug>

#define INPUT_IMAGE_1 "Image 1"
#define INPUT_IMAGE_2 "Image 2"
#define INPUT_WIDTH "Width (m)"
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
nitro::FlipOperator::execute(NodePorts &nodePorts, const std::map<QString, int> &options) {
    if(!nodePorts.allInputsPresent()) {
        return;
    }
    auto im1 = nodePorts.inGet<ImageData>(INPUT_IMAGE_1).data();
    auto im2 = nodePorts.inGet<ImageData>(INPUT_IMAGE_2).data();
    double screenWidth = nodePorts.inputValue(INPUT_WIDTH);
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

    nodePorts.output<ImageData>(OUTPUT_RESULT, flipImgToCvMat(colResult));
    nodePorts.output<ImageData>(OUTPUT_ERROR_MAP, flipImgFloatToGrayMat(errMap));
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::FlipOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("NVIDIA FLIP", "nvidiaFlip", category);
        return builder.
                withOperator(std::make_unique<nitro::FlipOperator>())->
                withIcon("compare.png")->
                withNodeColor({118, 185, 0})->
                withInputPort<ImageData>(INPUT_IMAGE_1)->
                withInputPort<ImageData>(INPUT_IMAGE_2)->
                withInputValue(INPUT_WIDTH, 0.7, 0.1)->
                withInputInteger(INPUT_RES, 1920, 1024)->
                withInputValue(INPUT_DIST, 0.7, 0)->
                withOutputPort<ImageData>(OUTPUT_RESULT)->
                withOutputPort<ImageData>(OUTPUT_ERROR_MAP)->
                build();
    };
}
