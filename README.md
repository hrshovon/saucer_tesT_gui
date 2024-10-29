# saucer_test_gui

# Steps to run the example code
1. Build docker ```dockerbuild -t saucerdev .```
2. Enter docker ```./run_docker```. You may need to changed the params of docker depedning on your system.
3. Assuming you are in the /work folder which should be this source folder, run ```saucer embed gui``` to convert contents of the gui folder to cpp
4. ```mkdir build``` > ```cd build``` > ```cmake ..``` > ```make```
5. ```./build/./upir_awesome_app``` to run.
