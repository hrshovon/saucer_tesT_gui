cmake_minimum_required(VERSION 3.21)
project(your_awesome_app LANGUAGES CXX VERSION 1.0)

# --------------------------------------------------------------------------------------------------------
# Create executable
# --------------------------------------------------------------------------------------------------------

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED )
find_package(nadjieb_mjpeg_streamer REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")


target_include_directories(${PROJECT_NAME} PRIVATE "embedded" ${OpenCV_INCLUDE_DIRS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads nadjieb_mjpeg_streamer::nadjieb_mjpeg_streamer ${OpenCV_LIBS})

# --------------------------------------------------------------------------------------------------------
# Link libraries
# --------------------------------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(
    saucer 
    GIT_REPOSITORY "https://github.com/saucer/saucer" 
    GIT_TAG v4.0.1
)

FetchContent_MakeAvailable(saucer)

target_link_libraries(${PROJECT_NAME} PRIVATE saucer::saucer)
