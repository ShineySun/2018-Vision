#include <highgui.h>
#include <iostream>
#include <cv.h>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>

using namespace std;
using namespace cv;

const int sobel_Mask_X[3][3] = {1,2,1,0,0,0,-1,-2,-1};
const int sobel_Mask_Y[3][3] = {-1,0,1,-2,0,2,-1,0,1};

Mat make_Canny(Mat gray,Mat return_img,int HIGH,int LOW)
{

  Mat direct = Mat(gray.rows,gray.cols,CV_8UC1);  // theta 값 저장공간

  for(int y = 1;y < gray.rows-1;y++)
  {
    for(int x = 1;x < gray.cols-1;x++)
    {
      int sumX = 0;
      int sumY = 0;
      for(int m_x = -1;m_x<2;m_x++)
      {
        for(int m_y = -1;m_y<2;m_y++)
        {
          sumX = gray.at<uchar>(y+m_y,x+m_x)*sobel_Mask_X[m_x+1][m_y+1];
          sumY = gray.at<uchar>(y+m_y,x+m_x)*sobel_Mask_Y[m_x+1][m_y+1];
        }   // 소벨 마스크 적용
      }
      return_img.at<uchar>(y,x) = abs(sumX) + abs(sumY);

      // 적용된 마스크를 토대로 theta 값 적용
      double theta;
      if(return_img.at<uchar>(y,x) == 0)
      {
        if(sumY == 0)
          theta = 0;
        else
          theta = 90;

      }
      else
      {
        theta = atan2((float)sumY,(float)sumX)*180/M_PI;
      }
      // theta 구하기..

      // theta 값 토대로 0 , 45, 90, 135  direction 구하기...
      if((theta > -22.5 && theta < 22.5) || theta > 157.5 || theta < -157.5)
        direct.at<uchar>(y,x) = 0;
      else if((theta >= 22.5 && theta < 67.5) || (theta >= -157.5 && theta < -112.5))
        direct.at<uchar>(y,x) = 45;
      else if((theta >= 67.5 && theta <= 112.5) || (theta >= -112.5 && theta < -67.5))
        direct.at<uchar>(y,x) = 90;
      else
        direct.at<uchar>(y,x) = 135;
    }
  }
  // theta 값 계산 및 대입..
  Mat img_canny = Mat(gray.rows,gray.cols,CV_8UC1); // Local maximum 구하고 저장하기 위한 데이터..
  img_canny.setTo(Scalar(0)); // 굳이 for loop를 안써도 값을 초기화 할 수 있는 함수 발견..

  for(int y = 1; y < gray.rows-1;y++)
  {
    for(int x = 1; x< gray.cols-1;x++)
    {
      switch(direct.at<uchar>(y,x))
      {
        case 0 :
          if(return_img.at<uchar>(y,x) > return_img.at<uchar>(y-1,x) && return_img.at<uchar>(y,x) > return_img.at<uchar>(y+1,x))
          {
            img_canny.at<uchar>(y,x) = 255;
          }
          break;
        case 45 :
          if(return_img.at<uchar>(y,x) > return_img.at<uchar>(y-1,x+1) && return_img.at<uchar>(y,x) > return_img.at<uchar>(y+1,x-1))
          {
            img_canny.at<uchar>(y,x) = 255;
          }
          break;
        case 90 :
          if(return_img.at<uchar>(y,x) > return_img.at<uchar>(y,x+1) && return_img.at<uchar>(y,x) > return_img.at<uchar>(y,x-1))
          {
            img_canny.at<uchar>(y,x) = 255;
          }
          break;
        case 135 :
          if(return_img.at<uchar>(y,x) > return_img.at<uchar>(y-1,x-1) && return_img.at<uchar>(y,x) > return_img.at<uchar>(y+1,x+1))
          {
            img_canny.at<uchar>(y,x) = 255;
          }
          break;

      }   // end of switch
    }
  } //end of for loop

  Mat img_result = Mat(gray.rows,gray.cols,CV_8UC1);  // 지금까지 했던 과정을 토대로 canny 알고리즘 적용할 img
  img_result.setTo(Scalar(0));

  for(int y = 1; y<gray.rows-1 ; y++)
  {
    for(int x=1;x<gray.cols-1;x++)
    {
      if(img_canny.at<uchar>(y,x))
      {
        if(return_img.at<uchar>(y,x) > HIGH)
        {
          img_result.at<uchar>(y,x) = 255;
        }
        else if(return_img.at<uchar>(y,x) > LOW)
        {
          bool check =true;
          switch (direct.at<uchar>(y,x)) {
            case 0: // 90 도 검사
              if(return_img.at<uchar>(y,x-1) > HIGH || return_img.at<uchar>(y,x+1) > HIGH)
              {
                img_result.at<uchar>(y,x) =255;
              }
              break;
            case 45: // 135도 검사
              if(return_img.at<uchar>(y-1,x-1) > HIGH || return_img.at<uchar>(y+1,x+1) > HIGH)
              {
                img_result.at<uchar>(y,x) =255;
              }
              break;
            case 90: // 0도 검사
              if(return_img.at<uchar>(y-1,x) > HIGH || return_img.at<uchar>(y+1,x) > HIGH)
              {
                img_result.at<uchar>(y,x) =255;
              }
              break;
            case 135: // 45 검사
              if(return_img.at<uchar>(y+1,x-1) > HIGH || return_img.at<uchar>(y-1,x+1) > HIGH)
              {
                img_result.at<uchar>(y,x) =255;
              }
              break;
          }
        }
      }
    }
  }


  return img_result;
}

int main()
{
  Mat origin = imread("road.jpg");
  Mat gray,gray2,gaussian,canny_img,gaussian2,using_canny_function;

  int high,row;

  cout << "Set the values of treshold(High, Row)" << endl;
  cin >> high >> row;
  cvtColor(origin,gray,COLOR_BGR2GRAY);
  cvtColor(origin,gray2,COLOR_BGR2GRAY);
  // 1 단계 가우시안 블러를 이용한 잡음 제거..
  GaussianBlur(gray,gaussian, Size(5, 5), 0);
  GaussianBlur(gray2,gaussian2, Size(5, 5), 0);

  Canny(gaussian2,using_canny_function,row,high);


  canny_img = make_Canny(gaussian,gray,high,row);

  imshow("origin",origin);
  imshow("Gaussian Image",gaussian2);
  imshow("Canny Image(using my function)",canny_img);
  imshow("Canny Image(using library function)",using_canny_function);
  waitKey(0);

}
