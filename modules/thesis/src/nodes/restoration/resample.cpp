#include "resample.hpp"
#include <nodes/nitronodebuilder.hpp>
#include <util.hpp>
#include <grayimagedata.hpp>
#include "util/distancefield.hpp"

#include <opencv2/imgproc.hpp>

namespace nitro::Thesis {

static inline const QString INPUT_IMAGE = "Image";
static inline const QString INPUT_K_SIZE = "Filter Size";
static inline const QString OUTPUT_IMAGE = "Image";
static inline const QString MODE_DROPDOWN = "Mode";
static inline const QString OPTION_BRIGHTNESS_CORRECT = "Correct Luminance";

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
        df[d] = signedDistField(src, d);
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

cv::Mat resampleImage(const cv::Mat &img, bool brightnessCorrect, double kSize) {

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
    colTable.push_back(colTable[colTable.size() - 1] * 2 - colTable[colTable.size() - 2]);


    cv::minMaxIdx(dfs[dfs.size() - 1], &minVal);
    dfs.push_back(dfs[dfs.size() - 1] + minVal);
    cv::Mat result = resample(indexed, colTable, dfs);

    if (brightnessCorrect) {
        int filterSize = std::round(std::max(img.rows, img.cols) * kSize);
        filterSize = std::max(filterSize % 2 == 0 ? filterSize - 1 : filterSize, 1);
        cv::Size fSize = {filterSize, filterSize};
        float sigma = (filterSize - 1.0) / 6.0;

        cv::Mat resIn;
        cv::GaussianBlur(result, resIn, fSize, sigma, sigma, cv::BorderTypes::BORDER_REFLECT);
        cv::Mat resTarget;
        cv::GaussianBlur(img, resTarget, fSize, sigma, sigma, cv::BorderTypes::BORDER_REFLECT);
        result = result - (resIn - resTarget);
    }

    return result;
}

void ResampleOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto imIn = nodePorts.inGetAs<GrayImageData>(INPUT_IMAGE);
    double kSize = nodePorts.inputValue(INPUT_K_SIZE);
    bool correctLum = nodePorts.optionEnabled(OPTION_BRIGHTNESS_CORRECT);

    cv::Mat result = resampleImage(*imIn, correctLum, kSize);
    nodePorts.output<GrayImageData>(OUTPUT_IMAGE, result);
}

std::function<std::unique_ptr<NitroNode>()> ResampleOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Resample", "resample", category);
        return builder.
                withOperator(std::make_unique<ResampleOperator>())->
                withIcon("resample.png")->
                withNodeColor(NITRO_RESTORATION_COLOR)->
                withInputPort<GrayImageData>(INPUT_IMAGE)->
                withInputValue(INPUT_K_SIZE, 0.125, 0, 1, BoundMode::LOWER_ONLY)->
                withCheckBox(OPTION_BRIGHTNESS_CORRECT, true)->
                withOutputPort<GrayImageData>(OUTPUT_IMAGE)->
                build();
    };
}


} // namespace nitro::Thesis
