#include <iostream>
#include<opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include"opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

int matchvalue(float array[], float target) {
	float value = 256;
	for (int i = 0; i < 256; i++) {
		float x = abs(array[i] - target);
		if (x <= value) {
			value = x;
			continue;
		}
		else {
			return i;
		}
	}
	return 0;
}

Mat myhisteq(Mat img, Mat temp) {
	if (img.channels() != temp.channels()) {
		cerr << "the channels of the origin picture is not match the template picture" << endl;
	}

	Mat res(img.rows, img.cols, img.type());

	if (temp.channels() == 1) {
		//计算概率分布
		int temp_nums[256] = { 0 };
		for (int x = 0; x < temp.rows; x++) {
			for (int y = 0; y < temp.cols; y++) {
				temp_nums[temp.at<uchar>(x, y)]++;
			}
		}
		int img_nums[256] = { 0 };
		for (int x = 0; x < img.rows; x++) {
			for (int y = 0; y < img.cols; y++) {
				img_nums[img.at<uchar>(x, y)]++;
			}
		}

		for (int i = 1; i < 256; i++) {
			temp_nums[i] += temp_nums[i - 1];
			img_nums[i] += img_nums[i - 1];
		}
		//计算累计分布
		float temp_cdf[256] = { 0 };
		float img_cdf[256] = { 0 };
		for (int i = 0; i < 256; i++) {
			temp_cdf[i] = temp_nums[i] / float(temp.rows * temp.cols);
			img_cdf[i] = img_nums[i] / float(img.rows * img.cols);
		}

		for (int x = 0; x < img.rows; x++) {
			for (int y = 0; y < img.cols; y++) {
				int img_value = img.at<uchar>(x, y);
				float target = img_cdf[img_value];
				int res_value = matchvalue(temp_cdf, target);
				res.at<uchar>(x, y) = res_value;
			}
		}
	}
	else {
		for (int c = 0; c < img.channels(); c++) {
			//计算概率分布
			int temp_nums[256] = { 0 };
			for (int x = 0; x < temp.rows; x++) {
				for (int y = 0; y < temp.cols; y++) {
					temp_nums[temp.at<Vec3b>(x, y)[c]]++;
				}
			}
			int img_nums[256] = { 0 };
			for (int x = 0; x < img.rows; x++) {
				for (int y = 0; y < img.cols; y++) {
					img_nums[img.at<Vec3b>(x, y)[c]]++;
				}
			}

			for (int i = 1; i < 256; i++) {
				temp_nums[i] += temp_nums[i - 1];
				img_nums[i] += img_nums[i - 1];
			}
			//计算累计分布
			float temp_cdf[256] = { 0 };
			float img_cdf[256] = { 0 };
			for (int i = 0; i < 256; i++) {
				temp_cdf[i] = temp_nums[i] / float(temp.rows * temp.cols);
				img_cdf[i] = img_nums[i] / float(img.rows * img.cols);
			}

			for (int x = 0; x < img.rows; x++) {
				for (int y = 0; y < img.cols; y++) {
					int img_value = img.at<Vec3b>(x, y)[c];
					float target = img_cdf[img_value];
					int res_value = matchvalue(temp_cdf, target);
					res.at<Vec3b>(x, y)[c] = res_value;
				}
			}
		}
	}
	return res;
}

int main() {
	Mat img = imread("C://Users//Chrysanthemum//Desktop//2.png");
	Mat temp = imread("C://Users//Chrysanthemum//Desktop//3.png");

	Mat res = myhisteq(img,temp);

	imshow("origin pic1", img);
	imshow("template pic", temp);
	imshow("result pic", res);
	waitKey();
}