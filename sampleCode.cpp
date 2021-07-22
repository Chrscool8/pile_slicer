#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	if (argc >= 3)
	{
		string file = argv[1];
		Mat image = imread(file, IMREAD_UNCHANGED);
		int depth = atoi(argv[2]);

		file = file.substr(0, file.find_last_of("."));

		bool extend = false;

		vector<Mat> channels(3);
		split(image, channels);
		cout << "> " << to_string(channels[0].at<uchar>(16, 16)) << endl;
		cout << "> " << to_string(channels[1].at<uchar>(16, 16)) << endl;
		cout << "> " << to_string(channels[2].at<uchar>(16, 16)) << endl;
		cout << "> " << to_string(channels[3].at<uchar>(16, 16)) << endl;

		if (argc == 4)
		{
			extend = (argv[3] == "true");
		}

		if (!image.empty())
		{
			cout << image.type() << endl;

			//if (image.channels() == 3)

			fs::create_directories("output");
			vector<Mat> images;

			for (int i = 0; i < image.size().height; i++)
			{
				Mat current = Mat(image.size().width, image.size().height, image.type());
				current = Scalar(0, 0, 0, 0);

				for (int j = 0; j < depth; j++)
				{
					Mat row = image.row(i);
					if (j == 0)
					{
						row.copyTo(current.row(image.size().height / 2 + depth / 2 - j));
					}
					else
					{
						for (int k = 0; k < row.size().height; k++)
						{
							cout << k << endl;

							row.at<Vec4i>(0, k);
							//Vec4d point = row.at<Vec4d>(Point(0,k));
							//if (row.at<Vec4>(k, 0).val[3] != 0)
							//	row.at<Vec4f>(k, 0) = Vec4f(0, 0, 0, 1);
							row.copyTo(current.row(image.size().height / 2 + depth / 2 - j));
						}
					}
				}

				images.push_back(current);

				string output = "output/" + string(file) + "_3d_" + to_string(i) + ".png";
				cout << output << endl;
				imwrite(output, current);
			}
		}
		else
		{
			cout << "Couldn't load pic";
		}
	}
	else
		cout << "./pile_slicer.exe <filename> <depth>" << endl;

	return 0;
}