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

#include "lib/headers/FileImg.hpp"
#include "lib/headers/helpers.hpp"
using namespace std;
using namespace cv;
namespace fs = filesystem;

const char sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

ofstream box_file;
bool force_exit = false;

int main(int argc, const char *argv[])
{
    string path = (string)argv[1];
    string labels = (string)argv[2];
    string exts[] = {"*.png", "*.jpeg", "*.jpg"};
    if (*path.end() != sep)
    {
        path += sep;
    }
    if (!fs::exists(path) || !fs::is_directory(path))
    {
        cout << "Invalid path." << endl;
        return 1;
    }
    if (!fs::exists(labels))
    {
        cout << "Invalid labels path." << endl;
        return 2;
    }
    ifstream input(labels);
    vector<string> filenames;
    vector<string> temp_files;
    vector<string> label_lines;
    for (int i = 0; i < size(exts); i++)
    {
        string folder = path + exts[i];
        glob(folder, temp_files);
        filenames.insert(filenames.end(), temp_files.begin(), temp_files.end());
        temp_files.clear();
    }
    for (string line; getline(input, line);)
    {
        label_lines.push_back(line);
    }

    for (size_t i = 0; i < filenames.size(); ++i)
    {
        FileImg *file_img = new FileImg(filenames[i], label_lines);
        file_img->set_last_img(resize_with_aspect(imread(file_img->get_filename())));

        namedWindow(file_img->get_filename());
        setMouseCallback(file_img->get_filename(), handle_selection, &(*file_img));
        imshow(file_img->get_filename(), file_img->get_last_img());

        while (true)
        {
            int key = waitKey(1) & 0xFF;
            if (key == 113)
            { // q
                break;
            }
            else if (key == 114 && file_img->count_history() > 1)
            { // r
                file_img->remove_selection();
                imshow(file_img->get_filename(), file_img->get_last_img());
            }
            else if (key == 101)
            { // e
                file_img->increase_selector();
            }
            else if (key == 27)
            { // ESC
                force_exit = true;
                break;
            }
        }
        if (file_img->count_boxes() > 0)
        {
            box_file.open(
                file_img->get_filename().substr(0, file_img->get_filename().find_last_of(".")) +
                    ".txt",
                fstream::in | fstream::out | fstream::trunc);
            if (box_file.fail())
                throw ios_base::failure(strerror(errno));
            box_file.exceptions(box_file.exceptions() | ios::failbit | ifstream::badbit);
            for (size_t j = 0; j < file_img->count_boxes(); j++)
            {
                box_file << file_img->get_boxes()[j] << endl;
            }
            box_file.close();
        }

        delete file_img;

        destroyAllWindows();

        if (force_exit)
        {
            break;
        }
    }

    return 0;
}
