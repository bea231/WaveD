#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char *argv[] )
{
  /* Check input parametres */
  if(argc != 2)
  {
    cout << "Display sourceImage program." << endl << "MY.EXE <sourceImage path>" << endl;
    return -1;
  }

  Mat sourceImage;

  /* Load sourceImage */
  sourceImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  
  /* Check for invalid input */
  if(!sourceImage.data)
  {
    cout <<  "Could not open or find the sourceImage" << endl;
    return -1;
  }
   

  char windowName[] = "Hello world";
  /* Create a window for display sourceImage */
  namedWindow(windowName, CV_WINDOW_AUTOSIZE);
  imshow(windowName, sourceImage);
    
  Mat bluredImage = sourceImage.clone();
  int blurSize = 7;
  /* Blur image */
  blur(sourceImage, bluredImage, Size(blurSize, blurSize));

  bool willBlur = true;  
  int key;
  /* Show our sourceImage inside window */
  while ((key = waitKey(0)) != 27 && key > 0)
  {
    imshow(windowName, willBlur ? bluredImage : sourceImage);
    willBlur = !willBlur;
  }
  return 0;
}