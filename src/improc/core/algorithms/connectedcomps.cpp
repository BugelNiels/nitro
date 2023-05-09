
#include <stack>
#include "connectedcomps.hpp"

int nitro::operations::numComponents(const nitro::CbdImage& image, int greyVal) {
    int rows = image.height();
    int cols = image.width();
    int numComponents = 0;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::stack<std::pair<int, int>> stack;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j] && image.get(j, i) == greyVal) {
                numComponents++;
                stack.emplace(i, j);
                visited[i][j] = true;

                while (!stack.empty()) {
                    int row = stack.top().first;
                    int col = stack.top().second;
                    stack.pop();

                    // Check the 8-connected neighbors
                    for (int dx = -1; dx <= 1; dx++) {
                        for (int dy = -1; dy <= 1; dy++) {
                            int new_row = row + dy;
                            int new_col = col + dx;
                            if (new_row >= 0 && new_row < rows &&
                                new_col >= 0 && new_col < cols &&
                                !visited[new_row][new_col] &&
                                image.get(new_col, new_row) >= greyVal) {
                                stack.emplace(new_row, new_col);
                                visited[new_row][new_col] = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return numComponents;
}

std::vector<int> nitro::operations::componentHistogram(const nitro::CbdImage &image) {
    std::vector<int> hist(256);
    for(int i = 0; i < 256; i++) {
        hist[i] = numComponents(image, i);
    }
    return hist;
}
