
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 1
#define RAIO 3

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, frame, points,canny;

  int width, height;
  int x, y;

  Vec3b cor;

  image= imread(argv[1],CV_LOAD_IMAGE_COLOR);

  srand(time(0));

  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);

  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

 

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8UC3, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      cor = image.at<Vec3b>(i,j);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(cor[2],cor[1],cor[0]),
             -1,
             CV_AA);
    }
  }
 
  waitKey();
	Canny(image, canny, 100, 200);
     for(int i=0; i<height; i++ ){
        for(int j=0; j<width; j++){
           if(canny.at<uchar>(i,j)>0){
              cor = image.at<Vec3b>(i,j);
              circle(points,
                     cv::Point(j,i),
                     RAIO,
                     CV_RGB(cor[2],cor[1],cor[0]),
                     -1,
                     CV_AA);
           }
        }
     }


  imshow("Imagem com CannyPoints", points);
  imshow("Imagem com CannyssPoints", canny);
  waitKey();
  return 0;
}

