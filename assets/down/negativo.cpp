#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;

  int x1,x2,y1,y2;
  cout << "Digite as dimensões do eixo x" << endl;
  cin >> x1;
  cin >> x2;
  cout << "Digite as dimensões do eixo y" << endl;
  cin >> y1;
  cin >> y2;
  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu a imagem" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);
  for(int i=x1;i<x2;i++){
    for(int j=y1;j<y2;j++){
      image.at<uchar>(i,j)=255-image.at<uchar>(i,j);
    }
  }
  
  imshow("janela", image);  
  waitKey();
  return 0;
}
