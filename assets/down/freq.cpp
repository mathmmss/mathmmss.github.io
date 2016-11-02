#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

using namespace cv;
using namespace std;



// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int , char**){
  VideoCapture cap;
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, filter, mag;
  Mat image, imagegray, tmp;
  Mat_<float> realInput, zeros;
  vector<Mat> planos;
  int dft_M, dft_N;
  float gama_l, gama_h, c, d0;
  char key;

  image = imread("lol1.png",CV_LOAD_IMAGE_GRAYSCALE);

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  for(;;){  

  cout << "Escolha os parâmetros" << endl << "gama_h = ";
  cin >> gama_h;
  cout << "gama_l = ";
  cin >> gama_l;
  cout << "c = ";
  cin >> c;
  cout << "d0 = ";
  cin >> d0;

  tmp = Mat(dft_M, dft_N, CV_32F);
  for(int i=0; i<dft_M ;i++){
    for(int j=0; j<dft_N ;j++){
      tmp.at<float>(i,j) = (gama_h - gama_l)*(1.0-exp(-1.0*(float)c*((((float)i-dft_M/2.0)*((float)i-dft_M/2.0) + 
      ((float)j-dft_N/2.0)*((float)j-dft_N/2.0))/(d0*d0))))+ gama_l;
    }
  }
 
  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);

    for(int i=0; i<dft_M; i++){
        for(int j=0; j<dft_N; j++){
          realInput.at<float> (i,j) = log(1 + realInput.at<float> (i,j) );
      }
    }

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filter,complexImage,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    
    for(int i=0; i<dft_M; i++){
      for(int j=0; j<dft_N; j++){
        planos[0].at<float> (i,j) = exp(planos[0].at<float> (i,j));
      }
    }

    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

    imshow("filtrada", planos[0]);
    imshow("original", image);

    key = (char) waitKey(10);


  }
  return 0;
}

