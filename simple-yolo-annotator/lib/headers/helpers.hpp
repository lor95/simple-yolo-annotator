//
//  helpers.hpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 09/10/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>

void draw_rectangle(cv::Mat img, cv::Point coords, int width, int height, std::string filename);
cv::Mat resize_with_aspect(cv::Mat img);
void handle_selection(int event, int x, int y, int flags, void* file);
