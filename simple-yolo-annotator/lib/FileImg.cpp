//
//  FileImg.cpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 09/10/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "headers/FileImg.hpp"
using namespace std;
using namespace cv;

FileImg::FileImg(string name) { filename = name; }
string FileImg::get_filename() { return filename; }
Point FileImg::get_ref_coords() { return temp_coords; }
int FileImg::get_refx() { return temp_coords.x; }
int FileImg::get_refy() { return temp_coords.y; }
void FileImg::set_ref_coords(int x, int y) { temp_coords = Point(x, y); }
Mat FileImg::get_last_img() { return history_img.back(); }
void FileImg::set_last_img(Mat img) { history_img.push_back(img); }
size_t FileImg::count_history() { return history_img.size(); }
size_t FileImg::count_boxes() { return boxes.size(); }
std::vector<std::string> FileImg::get_boxes() { return boxes; }
void FileImg::add_selection(Mat img, string line) {
    set_last_img(img);
    boxes.push_back(line);
}
void FileImg::remove_selection() {
    history_img.pop_back();
    boxes.pop_back();
}
