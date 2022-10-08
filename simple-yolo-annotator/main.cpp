//
//  main.cpp
//  simple-yolo-annotator
//
//  Created by Lorenzo Giuliani on 08/10/22.
//

#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
namespace fs = filesystem;

const char sep =
#ifdef _WIN32
'\\';
#else
'/';
#endif

const float MAX_WIDTH = 1280.0;
ofstream box_file;

class FileImg {
public:
    string filename;
    vector<string> boxes;
    vector<Mat> history_img;
    Point temp_coords;
};

Mat resize_with_aspect(Mat img) {
    Mat _img = img.clone();
    if (_img.cols > MAX_WIDTH) {
        float ratio = (float)_img.cols / MAX_WIDTH;
        Size s = Size((int)_img.cols / ratio, (int)_img.rows / ratio);
        resize(_img, _img, s);
    }
    return _img;
}

void draw_rectangle(Mat img, Point coords, int width, int height, string filename) {
    rectangle(img, coords, Point(coords.x + width, coords.y + height), Scalar(0, 0, 255), 1);
    imshow(filename, img);
}

void handle_selection(int event, int x, int y, int flags, void* file) {
    FileImg* file_img = (FileImg*)file;
    
    Mat src_img = file_img->history_img.back().clone();
    int rect_width = x - file_img->temp_coords.x;
    int rect_height = y - file_img->temp_coords.y;
    
    if (event == EVENT_LBUTTONDOWN) {
        (*file_img).temp_coords = Point(x, y);
    } else if (event == EVENT_LBUTTONUP) {
        float rel_w = abs((float)rect_width / src_img.cols);
        float rel_h = abs((float)rect_height / src_img.rows);
        float rel_x = (float)file_img->temp_coords.x / src_img.cols;
        float rel_y = (float)file_img->temp_coords.y / src_img.rows;
        if (rect_width < 0) {
            rel_x = ((float)file_img->temp_coords.x - abs(rect_width)) / src_img.cols;
        }
        if (rect_height < 0) {
            rel_y = ((float)file_img->temp_coords.y - abs(rect_height)) / src_img.rows;
        }
        draw_rectangle(src_img, file_img->temp_coords, rect_width, rect_height, file_img->filename);
        (*file_img).history_img.push_back(src_img);
        (*file_img).boxes.push_back("1 " + to_string(rel_x) + " " + to_string(rel_y) + " " +
                                    to_string(rel_w) + " " + to_string(rel_h));
        
    } else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
        draw_rectangle(src_img, file_img->temp_coords, rect_width, rect_height, file_img->filename);
    }
}

int main(int argc, const char* argv[]) {
    string path = (string)argv[1];
    string exts[] = {"*.png", "*.jpeg", "*.jpg"};
    if (*path.end() != sep) {
        path += sep;
    }
    if (!fs::exists(path) || !fs::is_directory(path)) {
        cout << "Invalid path." << endl;
        return 1;
    }
    vector<string> filenames;
    vector<string> temp_files;
    for (int i = 0; i < size(exts); i++) {
        string folder = path + exts[i];
        glob(folder, temp_files);
        filenames.insert(filenames.end(), temp_files.begin(), temp_files.end());
        temp_files.clear();
    }
    
    for (size_t i = 0; i < filenames.size(); ++i) {
        FileImg file_img;
        file_img.filename = filenames[i];
        file_img.history_img.push_back(resize_with_aspect(imread(file_img.filename)));
        
        namedWindow(file_img.filename);
        setMouseCallback(file_img.filename, handle_selection, &file_img);
        imshow(file_img.filename, file_img.history_img.back());
        
        while (true) {
            int key = waitKey(1) & 0xFF;
            if (key == 113) {
                break;
            } else if (key == 114 && file_img.history_img.size() > 1) {
                file_img.history_img.pop_back();
                file_img.boxes.pop_back();
                imshow(file_img.filename, file_img.history_img.back());
            }
        }
        box_file.open(file_img.filename.substr(0, file_img.filename.find_last_of(".")) + ".txt",
                      fstream::in | fstream::out | fstream::trunc);
        if (box_file.fail()) throw ios_base::failure(strerror(errno));
        box_file.exceptions(box_file.exceptions() | ios::failbit | ifstream::badbit);
        for (size_t j = 0; j < file_img.boxes.size(); j++) {
            box_file << file_img.boxes[j] << endl;
        }
        box_file.close();
        
        destroyAllWindows();
    }
    
    return 0;
}
