//
//  FileImg.cpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 09/10/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "headers/Colors.hpp"
#include "headers/FileImg.hpp"
using namespace std;
using namespace cv;

const Scalar available_colors[] = {COLORS_red, COLORS_aqua, COLORS_yellow, COLORS_green};

void merge_images(cv::Mat *bgr, cv::Mat *lbl)
{
    for (int i = 0; i < bgr->cols; i++)
    {
        for (int j = 0; j < bgr->rows; j++)
        {
            if (!(lbl->at<Vec3b>(j, i) == Vec3b(255, 255, 255)))
            {
                bgr->at<Vec3b>(j, i) = lbl->at<Vec3b>(j, i);
            }
        }
    }
}

FileImg::FileImg(string name, vector<string> labels_data)
{
    filename = name;
    for (size_t i = 0; i < labels_data.size(); ++i)
    {
        Label label;
        label.label = labels_data[i];
        label.label_color = available_colors[i % size(available_colors)];
        labels.push_back(label);
    }
}
string FileImg::get_filename()
{
    return filename;
}
int FileImg::get_refx()
{
    return temp_coords.x;
}
int FileImg::get_refy()
{
    return temp_coords.y;
}
void FileImg::set_ref_coords(int x, int y)
{
    temp_coords = Point(x, y);
}
void FileImg::toggle_show_lbl()
{
    show_lbl = !show_lbl;
    if (show_lbl)
    {
        Mat img = get_last_img().clone();
        Mat lbl_img = get_last_lbl_img().clone();
        merge_images(&img, &lbl_img);
        imshow(filename, img);
    }
    else
    {
        imshow(filename, get_last_img());
    }
}
void FileImg::increase_selector()
{
    selector++;
}
int FileImg::get_label_index()
{
    return selector % labels.size();
}
Mat FileImg::get_last_img()
{
    return history_img.back();
}
Mat FileImg::get_last_lbl_img()
{
    return history_lbl.back();
}
void FileImg::set_last_img(Mat img)
{
    Mat lbl(Size(img.cols, img.rows), CV_8UC3, Scalar(255, 255, 255));
    history_img.push_back(img);
    history_lbl.push_back(lbl);
}
size_t FileImg::count_history()
{
    return history_img.size();
}
size_t FileImg::count_boxes()
{
    return boxes.size();
}
std::vector<std::string> FileImg::get_boxes()
{
    return boxes;
}
void FileImg::add_selection(Mat img, Mat lbl_img, string line)
{
    history_img.push_back(img);
    putText(lbl_img, labels[get_label_index()].label, lbl_point, FONT_HERSHEY_PLAIN, 1, labels[get_label_index()].label_color, 1);
    history_lbl.push_back(lbl_img);
    boxes.push_back(line);
    if (show_lbl)
    {
        Mat bgr = img.clone();
        merge_images(&bgr, &lbl_img);
        imshow(filename, bgr);
    }
    else
    {
        imshow(filename, img);
    }
}
void FileImg::draw_rectangle(Mat img, Mat lbl_img, int width, int height)
{
    lbl_point = Point(min(temp_coords.x, temp_coords.x + width), min(temp_coords.y, temp_coords.y + height) - 3);
    rectangle(img, temp_coords, Point(temp_coords.x + width, temp_coords.y + height), labels[FileImg::get_label_index()].label_color, 2);
    imshow(filename, img);
}
void FileImg::remove_selection()
{
    history_img.pop_back();
    history_lbl.pop_back();
    boxes.pop_back();
}