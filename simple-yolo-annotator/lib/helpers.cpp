//
//  helpers.cpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 09/10/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "headers/FileImg.hpp"
#include "headers/helpers.hpp"
using namespace std;
using namespace cv;

const float MAX_WIDTH = 1280.0;
bool dragging = false;

Mat resize_with_aspect(Mat img)
{
    Mat _img = img.clone();
    if (_img.cols > MAX_WIDTH)
    {
        float ratio = (float)_img.cols / MAX_WIDTH;
        Size s = Size((int)_img.cols / ratio, (int)_img.rows / ratio);
        resize(_img, _img, s, INTER_AREA);
    }
    return _img;
}

void handle_selection(int event, int x, int y, int flags, void *file)
{
    FileImg *file_img = (FileImg *)file;

    Mat src_img = file_img->get_last_img().clone();
    Mat lbl_img = file_img->get_last_lbl_img().clone();
    int rect_width = x - file_img->get_refx();
    int rect_height = y - file_img->get_refy();

    if (event == EVENT_LBUTTONDOWN)
    {
        dragging = true;
        file_img->set_ref_coords(x, y);
    }
    else if (event == EVENT_LBUTTONUP)
    {
        dragging = false;
        file_img->draw_rectangle(src_img, lbl_img, rect_width, rect_height);
        float rel_w = abs((float)rect_width / src_img.cols);
        float rel_h = abs((float)rect_height / src_img.rows);
        float rel_x = abs((float)(file_img->get_refx() + rect_width / 2) / src_img.cols);
        float rel_y = abs((float)(file_img->get_refy() + rect_height / 2) / src_img.rows);
        file_img->add_selection(src_img, lbl_img, to_string(file_img->get_label_index()) + " " +
                                             to_string(rel_x) + " " + to_string(rel_y) + " " +
                                             to_string(rel_w) + " " + to_string(rel_h));
    }
    else if (event == EVENT_MOUSEMOVE && dragging)
    {
        file_img->draw_rectangle(src_img, lbl_img, rect_width, rect_height);
    }
}
