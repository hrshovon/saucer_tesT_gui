#include <iostream>
#include <saucer/smartview.hpp>
#include <saucer/utils/future.hpp>
#include "embedded/all.hpp"
#include <cstdlib>
#include <thread>
#include <vector>
#include <print>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <nadjieb/mjpeg_streamer.hpp>
#include <string>

using MJPEGStreamer = nadjieb::MJPEGStreamer;

void streamVideo()
{
    cv::VideoCapture cap(0);
    MJPEGStreamer streamer;

    // By default 1 worker is used for streaming
    // if you want to use 4 workers:
    //      streamer.start(8080, 4);
    streamer.start(8000,1);
    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};

    while (streamer.isRunning())
    {
        cv::Mat frame;
        cv::Mat outFrame;
        cap.read(frame);

        cv::resize(frame, outFrame, cv::Size(640, 480));
        cv::Mat gray, edge;
        cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);
        cv::blur( gray, edge, cv::Size(3,3) );
        cv::Canny( edge, edge, 50, 255, 3 );
        cv::Mat op;
        cv::cvtColor(edge,op,cv::COLOR_GRAY2BGR);       
        std::vector<uchar> buff_bgr;
        cv::imencode(".jpg", op, buff_bgr, params);
        streamer.publish("/bgr", std::string(buff_bgr.begin(), buff_bgr.end()));
        usleep(1000);
    }
    std::cout << "end of stream" << std::endl;
    streamer.stop();
    cap.release();
}

void processData(saucer::smartview<saucer::serializers::glaze::serializer> *smartview)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen1(rd()); // seed the generator
    std::mt19937 gen2(rd()); // seed the generator
    
    std::uniform_int_distribution<> distrVel(0, 3000);
    std::uniform_int_distribution<> distrBatt(0, 12);
    
    while (true)
    {
        smartview->evaluate<void>("gaugeSpeed.set({})", saucer::make_args(distrVel(gen1))) | saucer::forget();
        smartview->evaluate<void>("gaugeBattery.set({})", saucer::make_args(distrBatt(gen2))) | saucer::forget();
        sleep(1);
    }
}

int main()
{

    std::thread videoThread(streamVideo);

    auto app = saucer::application::acquire({
        // Instantiate application
        .id = "hello-world", // Choose a suitable ID for your program
    });

    saucer::smartview smartview{{
        // Instantiate smartview
        .application = app,
    }};

    smartview.set_title("Hello World!"); // Set the window title to "Hello World!"
    smartview.set_size(1280, 720);
    // Expose a synchronous native function "add_ten" using the default serializer

    smartview.set_dev_tools(true);
    smartview.embed(saucer::embedded::all());
    smartview.serve("index.html");
    smartview.show(); // Show the smartview
    
    std::thread processDataThread(processData,&smartview);


    app->run(); // And finally enter the run-loop.
    return 0;
}
