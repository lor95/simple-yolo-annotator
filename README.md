# Install instructions

Build opencv from source and execute:  
```bash
g++ simple-yolo-annotator/main.cpp simple-yolo-annotator/lib/FileImg.cpp simple-yolo-annotator/lib/helpers.cpp -o Build/Ubuntu/simple-yolo-annotator `pkg-config --cflags --libs opencv4`
```

## Build opencv Ubuntu:
Ubuntu version: `22.04`  

Install dependencies:  
```bash
sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb2 libtbb-dev libdc1394-dev
```
Clone opencv repository:  
```bash
mkdir ~/opencv_build && cd ~/opencv_build
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
```
Build opencv:  
```bash
cd ~/opencv_build/opencv
mkdir build && cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=ON \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D OPENCV_GENERATE_PKGCONFIG=ON \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
    -D BUILD_EXAMPLES=OFF ..

sudo make -j8
sudo make install
cd ~ && rm -rf ~/opencv_build/
pkg-config --modversion opencv4
```

