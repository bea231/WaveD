#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/* Include compiled wavelet library from: https://code.google.com/p/wavelet1d */
#include "wavelet.h"
#pragma comment(lib, "wavelet.lib")

using namespace cv;
using namespace std;

/* Search maximum element in 2D vector function */
double maxValue( vector<vector<double>> &array )
{
  double max = 0;

  for (unsigned int i = 0; i < array.size(); i++)
    for (unsigned int j = 0; j < array[0].size(); j++)
    {
      if (max < array[i][j])
        max = array[i][j];
    }
  return max;
}

int main( int argc, char *argv[] )
{
  /* Check input parametres */
  if(argc != 2)
  {
    cout << "Display wavelet transformation of image program." << endl << "MY.EXE <sourceImage path>" << endl;
    return -1;
  }

  /* Create a window for display images */
  char windowName[] = "Hello world";
  namedWindow(windowName, CV_WINDOW_AUTOSIZE);

  /* Load sourceImage */
  Mat sourceImage;
  int sourceWidth, sourceHeight;

  sourceImage = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);  
  /* Check for invalid input */
  if(!sourceImage.data)
  {
    cout <<  "Could not open or find the sourceImage" << endl;
    return -1;
  }
  sourceWidth = sourceImage.size().width;
  sourceHeight = sourceImage.size().height;

  /* Show image in program window */
  imshow(windowName, sourceImage);
  waitKey(0);

  /*** Forward Wavelet transformation ***/

  /* Create array of image pixels for wavlet library */
  vector<vector<double> > sourcePixels(sourceHeight, vector<double>(sourceWidth));

  for (int i = 0; i < sourceHeight; i++)
    for (int j = 0; j < sourceWidth; j++)
    {
      unsigned char temp;


      temp = ((unsigned char *)sourceImage.data + i * sourceImage.step)[j  * sourceImage.elemSize() + 1];
      sourcePixels[i][j] = (double)temp;
    }

  /* Get transform coefficient and apply convulation */
  string waveletName = "haar";
  vector<double> coeffLD,  // Low Pass Decomposition Filter Coefficients 
                 coeffHD,  // High Pass Decomposition Filter Coefficients
                 coeffLR,  // Low Pass Reconstruction Filter Coefficients
                 coeffHR;  // High Pass Reconstruction Filter Coefficients

  filtcoef(waveletName, coeffLD, coeffHD, coeffLR, coeffHR);

  vector<double> transformedSubbands,  // 1D vector of output sub-bands (A - approximation sub-band, Dh(J) - horizontal details sub-band
                                    // Dv(J) - vertical details sub-band, Dd(J) - diagonal details sub-band, Dh(J - 1), ...
                 flag;              // Stores values for IDWT function
  vector<int> length;               // Vector of lengths of sub-bands in 'transformedImage'
  int decompositionLevels = 2;      // Number of decompositions level

  /* Finding 2D DWT Transform of the image using symetric extension algorithm */
  dwt_2d_sym(sourcePixels, decompositionLevels, waveletName, transformedSubbands, flag, length, 0);

  vector<int> length2;
  // This algorithm computes DWT of image of any given size. Together with convolution and
  // subsampling operations it is clear that subsampled images are of different length than
  // dyadic length images. In order to compute the "effective" size of DWT we do additional
  // calculations.
  dwt_output_dim_sym(length, length2, decompositionLevels);  // The last two entries of length2 gives the size of DWT( rows number by columns number)
  
  int rowsNumber = length2[length2.size() - 2];
  int colsNumber = length2[length2.size() - 1];

  vector<vector<double>> transformedPixels(rowsNumber, vector<double>(colsNumber));

  /* Convert 1D vector of sub-bands to 2D vector of pixels, which can be displayed by OpenCV */
  dispDWT(transformedSubbands, transformedPixels, length, length2, decompositionLevels);

  /* Max value is needed to normalize transformedImage to unsigned 8-bit image */
  double maxPixel = maxValue(transformedPixels);

  /* Create transformed OpenCV-valid image */
  Mat transformedImage(rowsNumber, colsNumber, CV_8UC1);

  /* Setting coefficients of created image to the scaled DWT output values */
  for (int i = 0; i < rowsNumber; i++)
    for (int j = 0; j < colsNumber; j++)
    {
      if (transformedPixels[i][j] <= 0.0)
        transformedPixels[i][j] = 0.0;
      if (i <= (length2[0]) && j <= (length2[1]))
      {
        transformedImage.at<unsigned char>(i, j) = (char)((transformedPixels[i][j] / maxPixel) * 255.0);
        /* ((uchar*)(transformedImage.data + transformedImage.step * i))[j] =
          (char)((transformedPixels[i][j] / maxPixel) * 255.0); */
      }
      else 
      {
        transformedImage.at<unsigned char>(i, j) = (char)((transformedPixels[i][j]));
        /* ((uchar*)(transformedImage.data + transformedImage.step * i))[j] =
          (char)(transformedPixels[i][j]); */
      }
    }

  imshow(windowName, transformedImage);
  waitKey(0);


  /*** Inverse wavelet transformation ***/

  vector<vector<double> > inverseTransformedPixels(sourceHeight, vector<double>(sourceWidth));

  /* Finding 2D inverse DWT transform of the image using symetric extension algorithm */
  idwt_2d_sym(transformedSubbands, flag, waveletName, inverseTransformedPixels, length); 

  int reconstructedWidth = inverseTransformedPixels[0].size(), reconstructedHeight = inverseTransformedPixels.size();
  Mat inverseTransformedImage(reconstructedWidth, reconstructedHeight, CV_8UC1);

  for (int i = 0; i < reconstructedHeight; i++)
    for (int j = 0; j < reconstructedWidth; j++ )
      inverseTransformedImage.at<unsigned char>(i, j) = (char)(inverseTransformedPixels[i][j]);
  imshow(windowName, inverseTransformedImage);
  waitKey(0);

  return 0;
}