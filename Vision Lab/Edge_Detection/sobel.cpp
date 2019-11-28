#include <highgui.h>
#include <iostream>
#include <cv.h>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

const int sobel_Mask_X[3][3] = {1,2,1,0,0,0,-1,-2,-1};  // 소벨 수평 마스크
const int sobel_Mask_Y[3][3] = {-1,0,1,-2,0,2,-1,0,1};  // 소벨 수직 마스크

Mat make_Sobel(Mat gray,Mat return_img)
{
  for(int y = 1;y < gray.rows-1;y++)  // 0 ~ end 까지 하다가 세그멘테이션 오류 떠서.. 수정...
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
        }   // 마스크 적용
      }
      return_img.at<uchar>(y,x) = abs(sumX) + abs(sumY);

    }
  }
  return return_img;
}

int main()
{
  Mat origin = imread("road.jpg");
  Mat gray;
  Mat sobel_img = Mat(origin.rows,origin.cols,CV_8UC1);
  //Mat sobel_img2; // sobel 함수 사용
  //resize(origin,origin,Size(350,350),0,0,CV_INTER_LINEAR);
  // 이미지 사이즈가 너무 커서 줄임..
  //resize(sobel_img,sobel_img,Size(350,350),0,0,CV_INTER_LINEAR);
  cvtColor(origin,gray,COLOR_BGR2GRAY);
  // gray 생성..
  //Sobel(gray, sobel_img2, CV_32F, 1, 1);  // 소벨 함수 사용..

  sobel_img = make_Sobel(gray,sobel_img);



  imshow("origin",origin);
  imshow("gray",gray);
  imshow("sobel",sobel_img);
  //imshow("using lib",sobel_img2);
  waitKey(0);
  return 0;
}
