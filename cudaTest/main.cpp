#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/cudawarping.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/highgui/highgui.hpp"

void processUsingOpenCvGpu(std::string input_file, std::string output_file) {
	//Read input image from the disk
	cv::Mat inputCpu = cv::imread(input_file, CV_LOAD_IMAGE_COLOR);
	cv::cuda::GpuMat input(inputCpu);
	if (input.empty())
	{
		std::cout << "Image Not Found: " << input_file << std::endl;
		return;
	}

	//Create output image
	cv::cuda::GpuMat output;
	cv::cuda::resize(input, output, cv::Size(), .25, 0.25, CV_INTER_AREA); // downscale 4x

	cv::Mat outputCpu;
	output.download(outputCpu);
	imwrite(output_file, outputCpu);

	input.release();
	output.release();
}

int main() {
	std::cout << "Hello, World!" << std::endl;

	processUsingOpenCvGpu("D:\\pics\\2017-04-21_auto\\pic_1492746709.jpg", "C:\\Users\\markus\\Desktop\\gna\\pic_1492746709_small.jpg");
	return 0;
}