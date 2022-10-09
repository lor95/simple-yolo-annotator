//
//  FileImg.hpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 09/10/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>

class Label
{
public:
    std::string label;
    cv::Scalar label_color;
};

class FileImg
{
private:
    std::string filename;
    std::vector<std::string> boxes;
    std::vector<Label> labels;
    int selector = 0;
    std::vector<cv::Mat> history_img;
    cv::Point temp_coords;

public:
    FileImg(std::string name, std::vector<std::string> labels_data);
    cv::Point get_ref_coords();
    int get_refx();
    int get_refy();
    void set_ref_coords(int x, int y);
    void increase_selector();
    Label get_current_label();
    int get_label_index();
    std::string get_filename();
    cv::Mat get_last_img();
    void set_last_img(cv::Mat img);
    size_t count_history();
    size_t count_boxes();
    std::vector<std::string> get_boxes();
    void add_selection(cv::Mat img, std::string line);
    void remove_selection();
};
