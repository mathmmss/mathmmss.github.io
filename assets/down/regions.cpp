#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <time.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  srand(time(NULL));
  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu a imagem" << endl;

  //lembra que a função fornece altura e largura trocadas!!!

  int x = image.size().width;
  int y = image.size().height;

 // a função rect pega um retangulo onde os 2 primeiros parâmetros são o inicio da matriz e os outros 2 o tamanho. 
  Mat a = image(Rect(0,0,y/2,x/2));
  Mat b = image(Rect(y/2,0,y/2,x/2));
  Mat c = image(Rect(0,x/2,y/2,x/2));
  Mat d = image(Rect(y/2,x/2,y/2,x/2));
  
  // pesquisando na documentação da classe Mat achei esse tipo de dado CV_8UC1 que indica uma matriz de 1 canal.
  Mat novaimg (x, y, CV_8UC1);
  

  // para misturar as regiões aleatóriamente
  int cont, num = 0;
  int aux[4] = {0,0,0,0};
  Mat v_image[4] = {a,b,c,d};
  
  while (cont < 4)
  {
    num=1+rand()%(5-1);
    cout << num << endl;
    if (num != aux[0] && num != aux[1] && num != aux[2] && num != aux[3])
    {
      aux[cont] = num;
      if (num == 1)
      {
        v_image[cont].copyTo(novaimg(Rect(0,0,y/2,x/2)));
      }
      if (num == 2)
      {
        v_image[cont].copyTo(novaimg(Rect(y/2,0,y/2,x/2)));
      }
      if (num == 3)
      {
        v_image[cont].copyTo(novaimg(Rect(0,x/2,y/2,x/2)));
      }
      if (num == 4)
      {
        v_image[cont].copyTo(novaimg(Rect(y/2,x/2,y/2,x/2)));
      }
      cont++;
    }
  }

  imshow("janela", novaimg);  
  waitKey();
  return 0;
}
