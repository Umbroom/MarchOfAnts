#include<iostream>
#include<string>
#include<vector>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define blockR 100

int main() {
	int a, i, j, l, m, n, p, q, x, y;

	cout << "Please write a image file's name (with the extension)." << endl;

	string fileName;
	cin >> fileName;

	cout << "Please wait...." << endl;

	Mat readImage = imread(fileName);

	Vec3b *readImagePtr = readImage.ptr<Vec3b>();

	int width = readImage.cols, height = readImage.rows;

	vector<vector<int>> ImageBrightness;

	ImageBrightness.resize(width);
	ImageBrightness.resize(width);
	for (x = 0; x < width; x++) {
		ImageBrightness[x].resize(height);
	}

	Mat writeImage(Size(width, height), CV_8UC4);
	Vec4b *writeImagePtr = writeImage.ptr<Vec4b>();

	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			ImageBrightness[x][y] = (readImagePtr[width*y + x][0] + readImagePtr[width*y + x][1] + readImagePtr[width*y + x][2]) / 3;

	int widthFraction = width % blockR, heightFraction = height % blockR;
	int widthDiv = widthFraction == 0 ? width / blockR : width / blockR + 1;
	int heightDiv = heightFraction == 0 ? height / blockR : height / blockR + 1;
	int blockWidth, blockHeight;
	int pct, percent = 0;
	int eachColorSums[256];
	int dustpan, blackSum;

	for (x = 0; x < widthDiv; x++)
		for (y = 0; y < heightDiv; y++) {
			for (j = 0; j < 256; j++)
				eachColorSums[j] = 0;

			blockWidth = width >= blockR * (x + 1) ? blockR : widthFraction;
			blockHeight = height >= blockR * (y + 1) ? blockR : heightFraction;

			for (p = 0; p < blockWidth; p++)
				for (q = 0; q < blockHeight; q++) {
					l = ImageBrightness[blockR*x + p][blockR*y + q];
					eachColorSums[l] += 1;
				}

			for (j = 0, m = 0; j < 256; j++)
				if (eachColorSums[j] >= eachColorSums[m])
					m = j;

			for (i = m; i >= 0; i--)
				if (eachColorSums[i] <= (blockWidth*blockHeight) / 200)
					break;

			for (p = 0; p < blockWidth; p++)
				for (q = 0; q < blockHeight; q++) {
					l = ImageBrightness[blockR*x + p][blockR*y + q];

					if (l <= i) {
						a = i - l + 200;


						if (a > 255)
							a = 255;

						ImageBrightness[blockR*x + p][blockR*y + q] = a;
					}
					else
						ImageBrightness[blockR*x + p][blockR*y + q] = 0;
				}


			pct = (int)(100.0 * (heightDiv * x + y) / (widthDiv*heightDiv));
			if (percent < pct) {
				percent = pct;
				cout << percent << "%" << endl;
			}
		}

	for (n = 1; n <= 10; n++) {
		percent = 0;
		dustpan = 0;
		for (x = 0; x < width; x++) {
			for (y = 0; y < height; y++) {
				if (ImageBrightness[x][y] != 0) {
					blackSum = 0;
					for (i = 3; i <= 13; i += 2) {
						for (int j = 0; j < i - 1; j++) {
							if (0 <= x - i / 2 + j && x - i / 2 + j < width && 0 <= y - i / 2 && y - i / 2 < height)
								if (ImageBrightness[x - i / 2 + j][y - i / 2] != 0)
									blackSum += 1;

							if (0 <= x + i / 2 && x + i / 2 < width && 0 <= y - i / 2 + j && y - i / 2 + j < height)
								if (ImageBrightness[x + i / 2][y - i / 2 + j] != 0)
									blackSum += 1;

							if (0 <= x + i / 2 - j && x + i / 2 - j < width && 0 <= y + i / 2 && y + i / 2 < height)
								if (ImageBrightness[x + i / 2 - j][y + i / 2] != 0)
									blackSum += 1;

							if (0 <= x - i / 2 && x - i / 2 < width && 0 <= y + i / 2 - j && y + i / 2 - j < height)
								if (ImageBrightness[x - i / 2][y + i / 2 - j] != 0)
									blackSum += 1;
						}

						if (blackSum <= 2 * (int)(i / 2)) {
							dustpan += 1;
							ImageBrightness[x][y] = 0;
							break;
						}
					}
				}

				pct = (int)(100.0 * (height * x + y) / (width*height));
				if (percent < pct) {
					percent = pct;
					cout << "dust/" << n << "; " << percent << "%" << endl;
				}
			}
		}

		cout << "dustpan/" << n << "; " << dustpan << endl;
		if (dustpan == 0)
			break;
	}


	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			writeImagePtr[width*y + x][0] = 0;
			writeImagePtr[width*y + x][1] = 0;
			writeImagePtr[width*y + x][2] = 0;
			writeImagePtr[width*y + x][3] = ImageBrightness[x][y];
		}

	imwrite(fileName.substr(0, fileName.rfind(".")) + "_new.png", writeImage);

	cout << "completed." << endl;
	return 0;
}