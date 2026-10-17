#include <iostream>
#include <unordered_map>
#include <chrono>
#include <opencv2/opencv.hpp>

constexpr auto maskImagePath = "img/mask.png";
constexpr auto colorImagePath = "img/color_mask.png";
constexpr auto resultImagePath = "img/result_output.png";

// Custom hash for cv::Vec3b to use it in std::unordered_map
struct Vec3bHash
{
    size_t operator()(const cv::Vec3b &v) const
    {
        return static_cast<size_t>(v[0]) |
               (static_cast<size_t>(v[1]) << 8) |
               (static_cast<size_t>(v[2]) << 16);
    }
};

// Fill polygons with their dominant color
cv::Mat fillPolygonsWithDominantColor(const cv::Mat &polyMask, const cv::Mat &colorImg)
{
    cv::Mat labels, stats, centroids;
    // Find connected components (polygons)
    int nLabels = cv::connectedComponentsWithStats(polyMask, labels, stats, centroids, 8, CV_32S);

    cv::Mat result = colorImg.clone();

    // Iterate over all detected components (skip the background at index 0)
    for (int i = 1; i < nLabels; ++i)
    {
        int left = stats.at<int>(i, cv::CC_STAT_LEFT);
        int top = stats.at<int>(i, cv::CC_STAT_TOP);
        int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
        int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);

        std::unordered_map<cv::Vec3b, int, Vec3bHash> colorCounts;
        colorCounts.reserve(width * height);

        // Count color frequencies within the bounding polygon's
        for (int y = top; y < top + height; ++y)
        {
            const int *labelRow = labels.ptr<int>(y);
            const cv::Vec3b *colorRow = colorImg.ptr<cv::Vec3b>(y);
            for (int x = left; x < left + width; ++x)
            {
                if (labelRow[x] == i)
                {
                    colorCounts[colorRow[x]]++;
                }
            }
        }

        // Find the most frequent color
        cv::Vec3b dominantColor(0, 0, 0);
        int maxCount = -1;
        for (const auto &[color, count] : colorCounts)
        {
            if (count > maxCount)
            {
                maxCount = count;
                dominantColor = color;
            }
        }

        // Fill the polygon with the selected color
        for (int y = top; y < top + height; ++y)
        {
            const int *labelRow = labels.ptr<int>(y);
            cv::Vec3b *resultRow = result.ptr<cv::Vec3b>(y);
            for (int x = left; x < left + width; ++x)
            {
                if (labelRow[x] == i)
                {
                    resultRow[x] = dominantColor;
                }
            }
        }
    }

    return result;
}

int main()
{
    cv::Mat maskImage = cv::imread(maskImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat colorImage = cv::imread(colorImagePath, cv::IMREAD_COLOR);

    if (maskImage.empty() || colorImage.empty())
    {
        std::cerr << "Error loading images!\n";
        return -1;
    }

    cv::Mat polyMask;
    cv::threshold(maskImage, polyMask, 128, 255, cv::THRESH_BINARY);

    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat finalResult = fillPolygonsWithDominantColor(polyMask, colorImage);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Processing time (excluding I/O): " << duration.count() << " ms\n";
    cv::imwrite(resultImagePath, finalResult);
    std::cout << "Result successfully saved to '" << resultImagePath << "'\n";

    return 0;
}