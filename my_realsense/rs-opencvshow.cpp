#include <librealsense2/rs.hpp> // 引入RealSense標頭檔
#include <opencv2/opencv.hpp>   // 引入OpenCV標頭檔

using namespace std; // 使用std標準函式命名空間
using namespace cv;  // 使用OpenCV函式命名空間

// 主程式入口
int main(int argc, char * argv[]) try
{
  // 建構一個RealSense抽象設備的管道以容納擷取到的影像
  rs2::pipeline pipe;

  // 創建自定義參數以配置管道
  rs2::config cfg;

  // 設定影像尺寸(寬w，高h)
  const int w = 640;
  const int h = 480;

  // 設定欲顯示的影像流(可依需求啟動不一定要全設)
  cfg.enable_stream(RS2_STREAM_COLOR, w, h, RS2_FORMAT_BGR8, 30); // BGR888格式彩色影像 30fps
  cfg.enable_stream(RS2_STREAM_DEPTH, w, h, RS2_FORMAT_Z16, 30); // 16 bit格式灰階深度影像 30fps
  cfg.enable_stream(RS2_STREAM_INFRARED, 1, w, h, RS2_FORMAT_Y8, 30); // 8 bit格式左紅外線影像 30fps
  cfg.enable_stream(RS2_STREAM_INFRARED, 2, w, h, RS2_FORMAT_Y8, 30); // 8 bit格式右紅外線影像 30fps

  // 根據設定值啟動指定串流影像
  pipe.start(cfg);

  // 宣告深度(灰階)影像著虛擬色彩對應表
  rs2::colorizer color_map;

  while (waitKey(1) < 0) // 若有按鍵則結束顯示串流影像
  {
    // 等待下一組影像	
    rs2::frameset frames = pipe.wait_for_frames();

    // 取得每一張影像(可依需求不一定要全取)
    rs2::frame color_frame = frames.get_color_frame(); // 彩色影像
    rs2::frame depth_frame = frames.get_depth_frame().apply_filter(color_map); // 著虛擬色彩之深度影像
    rs2::frame irL_frame = frames.get_infrared_frame(1); // 左紅外線灰階影像
    rs2::frame irR_frame = frames.get_infrared_frame(2); // 右紅外線灰階影像
		
    // 建立OpenCV Mat格式之影像(可依需求不一定要全建立)
    Mat color_image(Size(w, h), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP); // 彩色影像
    Mat depth_image(Size(w, h), CV_8UC3, (void*)depth_frame.get_data(), Mat::AUTO_STEP); // 著虛擬色彩之深度影像
    Mat irL_image(Size(w, h), CV_8UC1, (void*)irL_frame.get_data(), Mat::AUTO_STEP); // 左紅外線灰階影像
    Mat irR_image(Size(w, h), CV_8UC1, (void*)irR_frame.get_data(), Mat::AUTO_STEP); // 右紅外線灰階影像		

    // 以OpenCV函式顯示擷取到影像(可依需求不一定要全顯示)
    imshow("Color Image", color_image); // 彩色影像
    imshow("Depth Image", depth_image); // 著虛擬色彩之深度影像
    imshow("Left IR Image", irL_image); // 左紅外線灰階影像
    imshow("Right IR Image", irR_image); // 右紅外線灰階影像
  }

  return EXIT_SUCCESS;
}
catch (const rs2::error & e) // 擷取錯誤呼叫碼及顯示
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e) // 擷取意外錯誤及顯示
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}
