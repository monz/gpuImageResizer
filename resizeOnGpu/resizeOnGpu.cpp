#include <iostream>
#include <thread>
#include <vector>
#include <experimental/filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ThreadPool.h"
#include "cxxopts.h"

void resizeOnGpu(std::string input_file, std::string output_file, int width = 0, int height = 0) {
	std::cout << "process file " << input_file << std::endl;

	//Read input image from the disk
	cv::Mat inputCpu = cv::imread(input_file, CV_LOAD_IMAGE_COLOR);
	cv::cuda::GpuMat input(inputCpu);
	if (input.empty()) {
		std::cout << "Image Not Found: " << input_file << std::endl;
		return;
	}

	//Create output image
	cv::cuda::GpuMat output;
	//cv::cuda::resize(input, output, cv::Size(40, 30), 0, 0, CV_INTER_AREA, cv::cuda::Stream()); // downscale
	cv::cuda::resize(input, output, cv::Size(width, height), 0, 0, CV_INTER_AREA); // downscale

	cv::Mat outputCpu;
	output.download(outputCpu);
	imwrite(output_file, outputCpu);

	input.release();
	output.release();

	std::cout << "processed: '" << output_file << "'" << std::endl;
}

int main(int argc, char* argv[]) {
	std::cout << "Resize Images!" << std::endl;

	// define cmd line options
	cxxopts::Options options(argv[0], " - GPU Image Resizer");	
	options.add_options()
		("t,threads", "Thread count", cxxopts::value<int>()->default_value("2"))
		("o, output", "Output directory", cxxopts::value<std::string>())
		("w, width", "New image width", cxxopts::value<int>()->default_value("0"))
		("h, height", "New image height", cxxopts::value<int>()->default_value("0"));
	options.parse(argc, argv);
	
	// read thread count
	int threadCount = options["threads"].as<int>();
	nbsdx::concurrent::ThreadPool pool(threadCount);
	// read output directory
	std::string outputDirectory;
	if (options.count("output") > 0) {
		outputDirectory = options["output"].as<std::string>();
	} else {
		std::cout << "Output directory not set!" << std::endl;
		exit(1);
	}
	// read new image sizes
	int width = options["width"].as<int>();
	int height = options["height"].as<int>();

	// resize each file read from stdin
	for (std::string file; std::getline(std::cin, file);) {
		pool.AddJob([file, outputDirectory, width, height] {
			//resizeOnGpu(file, outputDirectory + "/" + file.substr(file.find("pic_")), width, height);
			resizeOnGpu(file, std::experimental::filesystem::path(outputDirectory).append(std::experimental::filesystem::path(file).filename()).string(), width, height);
		});
	}
	pool.JoinAll();

	return 0;
}