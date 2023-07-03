#include "resample.hpp"
#include "nodes/nitronodebuilder.hpp"
#include "util.hpp"
#include "nodes/datatypes/grayimagedata.hpp"
#include <opencv2/imgproc.hpp>
#include "util/distancefield.hpp"

#include <QDebug>

#define INPUT_IMAGE "Image"
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

    int rows = src.rows;
    int cols = src.cols;
    cv::Mat squashed(src.rows, src.cols, CV_8UC2);



    for (int y = 0; y < rows; y++) {
        const uchar *srcRow = src.ptr<uchar>(y);
        cv::Vec2b *rowPtr = squashed.ptr<cv::Vec2b>(y);
        for (int x = 0; x < cols; x++) {
            rowPtr[x][0] = srcRow[x];
            rowPtr[x][1] = srcRow[x] + 1;
        }
    }


#pragma omp parallel for default(none) shared(df, src) firstprivate(numLevels)
    for (int d = 0; d < numLevels; d++) {
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
        for (int x = 0; x < width; x++) {
            // Calculate p from the inverse linear interpolation
            uchar idx = img.at<uchar>(y, x);

            float layer0Col = colTable[idx];
            float layer1Col = colTable[idx + 1];

            float p0 = df[idx].at<float>(y, x);
            float p1 = df[idx + 1].at<float>(y, x);

            float delta = (layer1Col - layer0Col);

            float t = p0 / (p0 - p1);
            float p = t * delta + layer0Col;

            resRow[x] = p;
        }
    }

    return resampled;
}

cv::Mat nitro::resampleImage(const cv::Mat &img, bool brightnessCorrect) {

    std::vector<float> colTable;
    cv::Mat indexed;
    toIndexed(img, indexed, colTable);

    int numLevels = colTable.size();
    std::vector<cv::Mat> dfs = getDfs(indexed, numLevels);
    float brightness = std::min(colTable[colTable.size() - 1] + 0.1f, 1.0f);
    colTable.push_back(brightness);
    dfs.push_back(dfs[dfs.size() - 1] + 100);
    cv::Mat result = resample(indexed, colTable, dfs);

    if (brightnessCorrect) {
        cv::Mat resIn;
        cv::GaussianBlur(result, resIn, cv::Size(33, 33), 32, 32, cv::BorderTypes::BORDER_REFLECT);
        cv::Mat resTarget;
        cv::GaussianBlur(img, resTarget, cv::Size(33, 33), 32, 32, cv::BorderTypes::BORDER_REFLECT);
        result = result - (resIn - resTarget);
    }
    return result;
}

void nitro::ResampleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto imIn = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    bool correctLum = nodePorts.optionEnabled(OPTION_BRIGHTNESS_CORRECT);

    cv::Mat result = resampleImage(*imIn, correctLum);
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
                withCheckBox(OPTION_BRIGHTNESS_CORRECT, true)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}
