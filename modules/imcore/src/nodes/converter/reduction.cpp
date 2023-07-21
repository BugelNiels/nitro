#include "reduction.hpp"
#include <util.hpp>
#include <nodes/nitronodebuilder.hpp>
#include "include/colimagedata.hpp"
#include <nodes/datatypes/decimaldata.hpp>
#include <nodes/datatypes/integerdata.hpp>

#include <opencv2/imgproc.hpp>
#include <unordered_set>

namespace nitro::ImCore {

inline const QString INPUT_IMAGE = "Image";
inline const QString MODE_DROPDOWN = "Mode";
inline const QString OUTPUT_VALUE = "Value";

static double minMat(const cv::Mat &in) {
    double minValue, maxValue;
    std::vector<cv::Mat> channels;
    cv::split(in, channels);
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(channels[0], &minValue, &maxValue, &minLoc, &maxLoc);
    for (int i = 1; i < channels.size(); i++) {
        double minValueTemp, maxValueTemp;
        cv::minMaxLoc(channels[i], &minValueTemp, &maxValueTemp, &minLoc, &maxLoc);
        minValue = std::min(minValue, minValueTemp);
        maxValue = std::max(maxValue, maxValueTemp);
    }
    return minValue;
}

static double maxMat(const cv::Mat &in) {
    double minValue, maxValue;
    std::vector<cv::Mat> channels;
    cv::split(in, channels);
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(channels[0], &minValue, &maxValue, &minLoc, &maxLoc);
    for (int i = 1; i < channels.size(); i++) {
        double minValueTemp, maxValueTemp;
        cv::minMaxLoc(channels[i], &minValueTemp, &maxValueTemp, &minLoc, &maxLoc);
        minValue = std::min(minValue, minValueTemp);
        maxValue = std::max(maxValue, maxValueTemp);
    }
    return maxValue;
}

static double sumMat(const cv::Mat &in) {
    double sum = 0;
    auto scalarSum = cv::sum(in);
    for (int i = 0; i < in.channels(); i++) {
        sum += scalarSum[i];
    }
    return sum;
}

static double averageMat(const cv::Mat &in) {
    double sum = 0;
    auto scalarSum = cv::sum(in);
    for (int i = 0; i < in.channels(); i++) {
        sum += scalarSum[i];
    }
    return sum / double(in.rows * in.cols * in.channels());
}

template<typename T>
int countUniqueValues(const cv::Mat &mat) {
    std::unordered_set<T> uniqueValues;

    // Iterate over each element in the matrix
    for (int i = 0; i < mat.rows; ++i) {
        const T *row_ptr = mat.ptr<T>(i);
        for (int j = 0; j < mat.cols; ++j) {
            uniqueValues.insert(row_ptr[j]);
        }
    }

    return int(uniqueValues.size());
}

// Note: does not count the unique number of colors in case of color images.
static int uniqueMat(const cv::Mat &in) {
    if (in.depth() == CV_8U) {
        return countUniqueValues<uchar>(in);
    } else if (in.depth() == CV_8S) {
        return countUniqueValues<char>(in);
    } else if (in.depth() == CV_16U) {
        return countUniqueValues<ushort>(in);
    } else if (in.depth() == CV_16S) {
        return countUniqueValues<short>(in);
    } else if (in.depth() == CV_32S) {
        return countUniqueValues<int>(in);
    } else if (in.depth() == CV_32F) {
        return countUniqueValues<float>(in);
    } else if (in.depth() == CV_64F) {
        return countUniqueValues<double>(in);
    }
    return 0;
}

void ReductionOperator::execute(NodePorts &nodePorts) {
    if (!nodePorts.allInputsPresent()) {
        return;
    }
    auto img = *nodePorts.inGetAs<ColImageData>(INPUT_IMAGE);
    int option = nodePorts.getOption(MODE_DROPDOWN);
    switch (option) {
        case 0:
            nodePorts.output<DecimalData>(OUTPUT_VALUE, minMat(img));
            break;
        case 1 :
            nodePorts.output<DecimalData>(OUTPUT_VALUE, maxMat(img));
            break;
        case 2 :
            nodePorts.output<DecimalData>(OUTPUT_VALUE, sumMat(img));
            break;
        case 3 :
            nodePorts.output<DecimalData>(OUTPUT_VALUE, averageMat(img));
            break;
        case 4 :
            nodePorts.output<IntegerData>(OUTPUT_VALUE, uniqueMat(img));
            break;
        default:
            nodePorts.output<DecimalData>(OUTPUT_VALUE, minMat(img));
            break;
    }
}

std::function<std::unique_ptr<NitroNode>()> ReductionOperator::creator(const QString &category) {
    return [category]() {
        NitroNodeBuilder builder("Reduction", "reduction", category);
        return builder.
                withOperator(std::make_unique<ReductionOperator>())->
                withIcon("sum.png")->
                withNodeColor(NITRO_CONVERTER_COLOR)->
                withDropDown(MODE_DROPDOWN, {"Min", "Max", "Sum", "Average", "Count Unique"})->
                withInputPort<ColImageData>(INPUT_IMAGE)->
                withOutputValue(OUTPUT_VALUE)->
                build();
    };
}

} // namespace nitro::ImCore
