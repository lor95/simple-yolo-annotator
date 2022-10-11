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
    int selector = 0;
    bool show_lbl = true;
    std::string filename;
    std::vector<std::string> boxes;
    std::vector<Label> labels;
    std::vector<cv::Mat> history_img;
    std::vector<cv::Mat> history_lbl;
    cv::Point temp_coords;
    cv::Point lbl_point;

public:
    FileImg(std::string name, std::vector<std::string> labels_data);
    int get_refx();
    int get_refy();
    void set_ref_coords(int x, int y);
    void toggle_show_lbl();
    void increase_selector();
    int get_label_index();
    std::string get_filename();
    cv::Mat get_last_img();
    cv::Mat get_last_lbl_img();
    void set_last_img(cv::Mat img);
    size_t count_history();
    size_t count_boxes();
    std::vector<std::string> get_boxes();
    void img_show();
    void add_selection(cv::Mat img, cv::Mat lbl_img, std::string line);
    void draw_rectangle(cv::Mat img, cv::Mat lbl_img, int width, int height);
    void remove_selection();
};
