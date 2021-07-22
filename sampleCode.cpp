#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>

//using namespace std;
using namespace cv;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	if (argc >= 3)
	{
		std::string file = argv[1];
		Mat image = imread(file, IMREAD_UNCHANGED);
		int depth = atoi(argv[2]);

		file = file.substr(0, file.find_last_of("."));

		bool extend = false;
		if (argc == 4)
		{
			if (std::string(argv[3]) == std::string("true"))
			{
				std::cout << "extend" << std::endl;
				extend = true;
			}
			else
				std::cout << "don't extend" << std::endl;
		}
		else
			std::cout << "don't extend" << std::endl;

		if (!image.empty())
		{
			//std::cout << image.type() << std::endl;

			//if (image.channels() == 3)

			fs::create_directories("output");
			std::vector<Mat> images;

			for (int i = 0; i < image.size().height; i++)
			{
				Mat current = Mat(image.size().width, image.size().height, image.type());
				current = Scalar(0, 0, 0, 0);

				for (int j = 0; j < depth; j++)
				{
					Mat row = image.row(i);
					if (j == 0 || extend)
					{
						row.copyTo(current.row(image.size().height / 2 + depth / 2 - j));
					}
					else
					{
						for (int k = 0; k < row.size().width; k++)
						{
							//cout << k << endl;

							std::vector<Mat> channels(3);
							split(row, channels);
							//cout << "> " << to_string(channels[0].at<uchar>(0, k)) << endl;
							//cout << "> " << to_string(channels[1].at<uchar>(0, k)) << endl;
							//cout << "> " << to_string(channels[2].at<uchar>(0, k)) << endl;
							//cout << "> " << to_string(channels[3].at<uchar>(0, k)) << endl;

							if (channels[3].at<uchar>(0, k) != 0)
							{
								channels[0].at<uchar>(0, k) = 0;
								channels[1].at<uchar>(0, k) = 0;
								channels[2].at<uchar>(0, k) = 0;
								merge(channels, row);
							}

							//row.at<Vec4u>(0, k);
							//Vec4d point = row.at<Vec4d>(Point(0,k));
							//if (row.at<Vec4>(k, 0).val[3] != 0)
							//	row.at<Vec4f>(k, 0) = Vec4f(0, 0, 0, 1);
							row.copyTo(current.row(image.size().height / 2 + depth / 2 - j));
						}
					}
				}

				images.push_back(current);

				std::string output = "output/" + std::string(file) + "_3d_" + std::to_string(image.size().height - i - 1) + ".png";
				std::cout << output << std::endl;
				imwrite(output, current);
			}
		}
		else
		{
			std::cout << "Couldn't load pic" << std::endl;
		}
	}
	else
		std::cout << "./pile_slicer.exe <filename> <depth> (<depth color>)" << std::endl;

	return 0;
}