#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int p_vertical_slider = 1;
int p_vertical_max;

int h_vertical_slider = 1;
int h_vertical_max;

float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};

Mat image1, image2, blended;
Mat imageTop;

char TrackbarName[50];


double alfa_calc(double i,double l1,double l2){
    return 0.5*(tanh((i-l1)/(double)(alfa_slider))-tanh((i-l2)/(double)(alfa_slider)));
}


void on_trackbar_blend(int, void*){
	int l1 = p_vertical_slider - h_vertical_slider/2;
	int l2 = p_vertical_slider + h_vertical_slider/2;

	for(int i=0; i<imageTop.rows ;i++)
	{
		alfa = alfa_calc(i,l1,l2);
		addWeighted(image1.row(i), alfa, image2.row(i), 1.0-alfa,0.0,imageTop.row(i));
	}
	
	imshow("Tiltshift",imageTop);
	
}


int main(int argvc, char** argv){


  	Mat mask(3,3,CV_32F), mask1;
  	image1 = imread("ny1.jpg");
	mask = Mat(3, 3, CV_32F, media);
	scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
	mask = mask1;
 	filter2D(image1, image2, image1.depth(), mask, Point(1,1), 0);
 	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
 	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
 	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
 	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
 	filter2D(image2, image2, image2.depth(), mask, Point(1,1), 0);
	image2.copyTo(imageTop);

	namedWindow("Tiltshift", 1);
	p_vertical_max=image1.size().height;

  	h_vertical_max=image1.size().height;

	sprintf( TrackbarName, "Posicao x %d", p_vertical_max );
  	createTrackbar( TrackbarName, "Tiltshift",&p_vertical_slider,p_vertical_max, on_trackbar_blend); 

  	sprintf( TrackbarName, "Tamanhox %d", h_vertical_max );
  	createTrackbar( TrackbarName, "Tiltshift",&h_vertical_slider, h_vertical_max, on_trackbar_blend);

	sprintf( TrackbarName, "Coeficente D x %d", alfa_slider_max );
  	createTrackbar( TrackbarName, "Tiltshift", &alfa_slider, alfa_slider_max, on_trackbar_blend);


	
	waitKey(0);
  return 0;
}