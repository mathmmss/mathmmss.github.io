#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m){
  for(int i=0; i<m.size().height; i++){
    for(int j=0; j<m.size().width; j++){
      cout << m.at<float>(i,j) << ",";
    }
    cout << endl;
  }
}

void menu(){
  cout << "\npressione a tecla para ativar o filtro: \n"
	"a - calcular modulo\n"
    "m - media\n"
    "g - gauss\n"
    "v - vertical\n"
	"h - horizontal\n"
    "l - laplaciano\n"
    "p - laplaciando do gaussiano\n"
	"esc - sair\n";
}

int main(int argvc, char** argv){
  Mat cap, frame, frame32f, frameFiltered, framelapgau;
  Mat mask(3,3,CV_32F), mask1;
  Mat result, result1;
  double width, height, min, max;
  int absolut;
  char key;
  bool lapgau = false;

  cap = imread("biel.png",CV_LOAD_IMAGE_COLOR);

  width=cap.size().width;
  height=cap.size().height;

  float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};
  float gauss[] = {1,2,1,
				   2,4,2,
				   1,2,1};
  float horizontal[]={-1,0,1,
					  -2,0,2,
					  -1,0,1};
  float vertical[]={-1,-2,-1,
					0,0,0,
					1,2,1};
  float laplacian[]={0,-1,0,
					 -1,4,-1,
					 0,-1,0};
  menu();
  cin >> key;
  switch(key){
    case 'a':
    menu();      
      absolut=!absolut;
      break;
    case 'm':
    menu();
      mask = Mat(3, 3, CV_32F, media);
      scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      break;
    case 'g':
    menu();
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      break;
    case 'h':
    menu();
      mask = Mat(3, 3, CV_32F, horizontal);
      printmask(mask);
      break;
    case 'v':
    menu();
      mask = Mat(3, 3, CV_32F, vertical);
      printmask(mask);
      break;
    case 'l':
    menu();
      mask = Mat(3, 3, CV_32F, laplacian);
      printmask(mask);
      break;
    case 'p':
    menu();      
      lapgau = true;
      break;  
    default:
      break;
    } 

    
  if (lapgau)
  {
    mask = Mat(3, 3, CV_32F, gauss);
    scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    cvtColor(cap, frame, CV_BGR2GRAY);
    frame.convertTo(frame32f, CV_32F);
    filter2D(frame32f, framelapgau, frame32f.depth(), mask, Point(1,1), 0);
    mask = Mat(3, 3, CV_32F, laplacian);
    printmask(mask);
    filter2D(framelapgau, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);

  } else 
  {    
    cvtColor(cap, frame, CV_BGR2GRAY);
    frame.convertTo(frame32f, CV_32F);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);

  }

  if(absolut){
      frameFiltered=abs(frameFiltered);
    }

  frameFiltered.convertTo(result, CV_8U);
  imshow("original", cap);
  imshow("filtroespacial", result);
  waitKey(0); //wait for key press
  return 0;
}