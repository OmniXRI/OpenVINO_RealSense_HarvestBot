// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

// include the librealsense C++ header file
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void my_video_play()
{
	VideoCapture my_cap("my_video.mpg");
	Mat frame;

	namedWindow("my video play");

	if (!my_cap.isOpened()){
		cout << "Read video failed !" << endl;
		exit(0);
	}

	int frame_num = my_cap.get(CAP_PROP_FRAME_COUNT);
	double frame_rate = my_cap.get(CV_CAP_PROP_FPS);
	int frame_delay = 1000 / frame_rate;

	cout << "total frame number is: " << frame_num << endl;
	cout << "FPS = " << frame_rate << endl;

	for (int i = 0;i < frame_num;i++) {
		my_cap >> frame;
		imshow("my video play", frame);

		//char img_name[15];
		//
		//if (i % 10 == 0) {
		//	sprintf(img_name, "img_%d.jpg", (2000 + i));
		//	imwrite(img_name, frame);
		//}		

		if (waitKey(frame_delay) == 'q') {
			break;
		}
	}

	destroyWindow("my video play");
	my_cap.release();
	cout << "play done." << endl;
}

int main(int argc, char * argv[]) try
{
	//Contruct a pipeline which abstracts the device
	rs2::pipeline pipe;

	//Create a configuration for configuring the pipeline with a non default profile
	rs2::config cfg;

	// setting frame size (width and height)
	const int w = 640;
	const int h = 480;

	//Add desired streams to configuration
	cfg.enable_stream(RS2_STREAM_COLOR, w, h, RS2_FORMAT_BGR8, 30);
	cfg.enable_stream(RS2_STREAM_DEPTH, w, h, RS2_FORMAT_Z16, 30);
	cfg.enable_stream(RS2_STREAM_INFRARED, 1, w, h, RS2_FORMAT_Y8, 30); // left
	cfg.enable_stream(RS2_STREAM_INFRARED, 2, w, h, RS2_FORMAT_Y8, 30); // right	

	//Instruct pipeline to start streaming with the requested configuration
	pipe.start(cfg);

	// Declare depth colorizer for pretty visualization of depth data
	rs2::colorizer color_map;

	//my_video_play();

	//VideoWriter my_video("my_video.mpg", CV_FOURCC('D', 'I', 'V', 'X'), 30.0, cv::Size(w, h), true);

	while (waitKey(1) < 0)
	{
		// Wait for next set of frames from the camera	
		rs2::frameset frames = pipe.wait_for_frames();
				
	    //Get each frame
		rs2::frame color_frame = frames.get_color_frame();
		//rs2::frame depth_frame = frames.get_depth_frame().apply_filter(color_map);
		//rs2::frame irL_frame = frames.get_infrared_frame(1);
		//rs2::frame irR_frame = frames.get_infrared_frame(2);		
		
		// Creating OpenCV Matrix from a color image
		Mat color_image(Size(w, h), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
		//Mat depth_image(Size(w, h), CV_8UC3, (void*)depth_frame.get_data(), Mat::AUTO_STEP);
		//Mat irL_image(Size(w, h), CV_8UC1, (void*)irL_frame.get_data(), Mat::AUTO_STEP);
		//Mat irR_image(Size(w, h), CV_8UC1, (void*)irR_frame.get_data(), Mat::AUTO_STEP);		

		// Display in a GUI
		imshow("Color Image", color_image);
		//moveWindow("Color Image", 0, 0);
		//my_video << color_image;
		//imshow("Depth Image", depth_image);
		//moveWindow("Depth Image", w, 0);
		//imshow("Left IR Image", irL_image);
		//moveWindow("Left IR Image", 0, h);
		//imshow("Right IR Image", irR_image);
		//moveWindow("Right IR Image", w, h);		
	}

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
