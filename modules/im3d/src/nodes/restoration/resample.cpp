#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "util.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>
#include "util/distancefield.hpp"

#include <QDebug>

#define INPUT_IMAGE "Image"
#define INPUT_WHITE_POINT "White point"
#define OUTPUT_IMAGE "Image"
#define MODE_DROPDOWN "Mode"
#define OPTION_BRIGHTNESS_CORRECT "Correct Luminance"

static void toIndexed(const cv::Mat &src, cv::Mat &dest, std::vector<float> &colTable) {

    std::vector<float> uniqueColors;
    std::set<float> uniqueColorsSet(src.begin<float>(), src.end<float>());
    colTable.assign(uniqueColorsSet.begin(), uniqueColorsSet.end());

    int height = src.rows;
    int width = src.cols;
    dest = cv::Mat(height, width, CV_8UC1);

    std::unordered_map<int, int> colMap;
    int numCols = colTable.size();

    for (int i = 0; i < numCols; i++) {
        colMap[colTable[i] * 255 + 0.5] = i;
    }
    for (int y = 0; y < height; y++) {
        uchar *rowPtr = dest.ptr<uchar>(y);
        const float *srcPtr = src.ptr<float>(y);
        for (int x = 0; x < width; x++) {
            rowPtr[x] = colMap[srcPtr[x] * 255 + 0.5];
        }
    }

}

static std::vector<cv::Mat> getDfs(const cv::Mat &src, int numLevels) {
    std::vector<cv::Mat> df(numLevels);
#pragma omp parallel for default(none) shared(df, src) firstprivate(numLevels)
    for (int d = 1; d < numLevels; d++) {
        df[d] = nitro::signedDistField(src, d);
    }
    return df;
}

static cv::Mat resample(const cv::Mat &img, const std::vector<float> &colTable,
                        const std::vector<cv::Mat> &df) {
    int width = df[0].cols;
    int height = df[0].rows;

    cv::Mat resampled = cv::Mat::zeros(height, width, CV_32FC1);

#pragma omp parallel for default(none) firstprivate(height, width) shared(df, resampled, colTable, img)
    for (int y = 0; y < height; y++) {
        float *resRow = resampled.ptr<float>(y);
        const uchar *indexRow = img.ptr<uchar>(y);
        for (int x = 0; x < width; x++) {
            // Calculate p from the inverse linear interpolation
            uchar idx = indexRow[x];

            double layer0Col = colTable[idx];

            double p0 = df[idx].at<double>(y, x);
            double p1 = df[idx + 1].at<double>(y, x);

            double delta = (colTable[idx + 1] - layer0Col);

            double t = p0 / (p0 - p1);
            if (p0 == p1) {
                t = 0;
            }
            if (t < 0) {
                delta = layer0Col - colTable[idx - 1];
            }
            double p = t * delta + layer0Col;

            resRow[x] = float(p);
        }
    }

    return resampled;
}

cv::Mat nitro::resampleImage(const cv::Mat &img, bool brightnessCorrect, double whitePoint) {

    std::vector<float> colTable;
    cv::Mat indexed;
    toIndexed(img, indexed, colTable);

    int numLevels = colTable.size();
    if (numLevels < 2) {
        return img;
    }
    std::vector<cv::Mat> dfs = getDfs(indexed, numLevels);
    double minVal;
    cv::minMaxIdx(dfs[1], &minVal);
    dfs[0] = dfs[1] - minVal;
    colTable.push_back(whitePoint);


    cv::minMaxIdx(dfs[dfs.size() - 1], &minVal);
    dfs.push_back(dfs[dfs.size() - 1] + minVal);
    cv::Mat result = resample(indexed, colTable, dfs);

    if (brightnessCorrect) {
        int filterSize = std::max(img.rows, img.cols) / 8;
        filterSize = filterSize % 2 == 0 ? filterSize - 1 : filterSize;
        cv::Size fSize = {filterSize, filterSize};
        float sigma = 32;

        cv::Mat resIn;
        cv::GaussianBlur(result, resIn, fSize, sigma, sigma, cv::BorderTypes::BORDER_REFLECT);
        cv::Mat resTarget;
        cv::GaussianBlur(img, resTarget, fSize, sigma, sigma, cv::BorderTypes::BORDER_REFLECT);
        result = result - (resIn - resTarget);
    }

    return result;
}

void nitro::ResampleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto imIn = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    double whitePoint = nodePorts.inputValue(INPUT_WHITE_POINT);
    bool correctLum = nodePorts.optionEnabled(OPTION_BRIGHTNESS_CORRECT);

    cv::Mat result = resampleImage(*imIn, correctLum, whitePoint);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<nitro::NitroNode>()> nitro::ResampleOperator::creator(const QString &category) {
    return [category]() {
        nitro::NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<nitro::ResampleOperator>())->
                withIcon("resample.png")->
                withNodeColor(NITRO_RESTORATION_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputValue(INPUT_WHITE_POINT, 1, 0, 1, BoundMode::UNCHECKED)->
                withCheckBox(OPTION_BRIGHTNESS_CORRECT, true)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
