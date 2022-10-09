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
string FileImg::get_filename() { return filename; }
Point FileImg::get_ref_coords() { return temp_coords; }
int FileImg::get_refx() { return temp_coords.x; }
int FileImg::get_refy() { return temp_coords.y; }
void FileImg::set_ref_coords(int x, int y) { temp_coords = Point(x, y); }
void FileImg::increase_selector() { selector++; }
int FileImg::get_label_index() { return selector % labels.size(); }
Label FileImg::get_current_label() { return labels[FileImg::get_label_index()]; }
Mat FileImg::get_last_img() { return history_img.back(); }
void FileImg::set_last_img(Mat img) { history_img.push_back(img); }
size_t FileImg::count_history() { return history_img.size(); }
size_t FileImg::count_boxes() { return boxes.size(); }
std::vector<std::string> FileImg::get_boxes() { return boxes; }
void FileImg::add_selection(Mat img, string line)
{
    set_last_img(img);
    boxes.push_back(line);
}
void FileImg::remove_selection()
{
    history_img.pop_back();
    boxes.pop_back();
}
