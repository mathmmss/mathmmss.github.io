#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  int cont=0;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  // limpando as bordas!!!
  nobjects=0;
  for(int i=0; i<width; i=i+(width-1)){
    for(int j=0; j<height; j++){
      if(image.at<uchar>(j,i) == 255){
		    nobjects++;
		    p.x=i;
		    p.y=j;
		    floodFill(image,p,0);
	   }
	 }
  }
  for(int i=0; i<width; i=i+(width-1)){
    for(int j=0; j<height; j++){
      if(image.at<uchar>(i,j) == 255){
		    nobjects++;
		    p.x=j;
		    p.y=i;
		    floodFill(image,p,0);
	   }
	 }
  }

  //para diferenciar o buraco do fundo
  floodFill(image,p,10);


  for (int i=0; i<height; i++)
  {
  	for (int j=0; j<width; j++)
  	{
      //encontrar objetos e salva sua coordenada
  		if(image.at<uchar>(i,j) == 255)
  		{
  			p.x = j;
  			p.y = i;
  		}
  		else if(image.at<uchar>(i,j) == 0)
  		{
        //encontra buraco e entende-se que o objeto anterior é o dono desse buraco
  			if (image.at<uchar>(p.y,p.x) == 255)
  			{
          //seedfill na imagem
  				floodFill(image, p, 150);
  			}
          //seedfill no buraco
  			p.x = j;
  			p.y = i;
  			cont++;
  			floodFill(image, p, 150);
  			

  		}
  	}	
  }

  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
    // achou um objeto
    nobjects++;
    p.x=j;
    p.y=i;
    floodFill(image,p,nobjects);
    }
  }
  }

  cout << "Número de objetos: " << nobjects+cont << endl;
  cout << "Numero de objetos com buracos: " << cont << endl;
  cout << "Número de objetos sem buracos: " << nobjects << endl;
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
