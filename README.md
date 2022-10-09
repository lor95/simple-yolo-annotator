### Install instructions

Build opencv from source and execute:  
```[bash]
g++ simple-yolo-annotator/main.cpp simple-yolo-annotator/lib/FileImg.cpp simple-yolo-annotator/lib/helpers.cpp -o Build/Ubuntu/simple-yolo-annotator `pkg-config --cflags --libs opencv4`
```
