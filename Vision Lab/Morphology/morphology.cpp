// 3 x 3 의 마스크의 모양을 받아 모폴로지 연산을 시행하는 프로그램 작성
// 2018 7 25 (수)
// 처음 설계 시 마스크의 사이즈와 모양을 둘 다 받아 들여 코딩하려 하였으나
// 사이즈에 대한 제약 조건이 생각보다 복잡하여 3 x 3  마스크로 통일함...

#include <iostream>
#include <highgui.h>
#include <cv.h>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

Mat Dilation(Mat,int[][3]);
Mat Erosion(Mat,int[][3],const int);

int main()
{
  //int input_row,input_col;  // variables for input which are  row,col of mask size
  int temp;
  int count = 0;

  Mat origin = imread("Image1.pgm");  //create Mat object
  Mat gray;
  Mat dilation_img = Mat(origin.rows,origin.cols,CV_8UC1);
  Mat erosion_img = Mat(origin.rows, origin.cols, CV_8UC1);
  cvtColor(origin,gray,COLOR_BGR2GRAY);

  int mask[3][3];

  cout <<endl <<"Input the mask components" << endl;
  for(int  i =0;i<3;i++)
  {
    for(int j =0;j<3;j++)
    {
      cin >> temp;
      if(temp == 1) count++;
      mask[i][j] = temp;
    }
  }

  dilation_img = Dilation(gray,mask);
  erosion_img = Erosion(gray,mask,count);

  imshow("gray",gray);
  imshow("dilation",dilation_img);
  imshow("erosion",erosion_img);
  waitKey(0);


}

/*
Dilation 함수
original 이미지와 mask를 받아 새로운 이미지 객체를 생성하고 초기화 한 뒤
mask 모양 대로 팽창 연산 수행..
*/
Mat Dilation(Mat original,int mask[][3])
{
  Mat return_img = Mat(original.rows,original.cols,CV_8UC1);

  for(int y = 0; y<original.rows;y++)
  {
    for(int x= 0;x<original.cols;x++)
    {
      return_img.at<uchar>(y,x) = 0;
    }
  }  // return_img 초기화

  for(int y = 1; y < original.rows-1;y++)
  {
    for(int x = 1;x<original.cols-1;x++)
    {
      if(original.at<uchar>(y,x)==255)
      {
        for(int i = -1;i<2;i++)
        {
          for(int j = -1;j<2;j++)
          {
            if(mask[i+1][j+1] == 1)
              return_img.at<uchar>(y+j,x+i) = 255;
          }
        }
      }
    }
  }
  return return_img;
}

/*
Erosion 함수
original 이미지와 mask를 받아 새로운 이미지 객체를 생성하고 초기화 한 뒤
mask 모양 대로 침식 연산 수행..
mask check 시 카운트 하는 형식으로 체크..
비교할 count 변수는 사용자가 입력 할때 1의 개수를 미리 count해놓음
마스크 모양과 맵 객체의 각 점에서의 모양이 같으면 흰색 아닐시 백색
*/
Mat Erosion(Mat original , int mask[][3],const int count)
{
  Mat return_img = Mat(original.rows,original.cols,CV_8UC1);

  for(int y = 0; y<original.rows;y++)
  {
    for(int x= 0;x<original.cols;x++)
    {
      return_img.at<uchar>(y,x) = 0; // 0 은 검은색
    }
  }  // return_img 초기화

  for(int y = 1; y<original.rows-1;y++)
  {
    for(int x = 1;x<original.cols-1;x++)
    {
      if(original.at<uchar>(y,x) == 255)
      {
        int count_num = 0;
        for(int i = -1;i<2;i++)
        {
          for(int j = -1;j<2;j++)
          {
            if(mask[i+1][j+1] == 1 && original.at<uchar>(y+j,x+i) == 255)
              count_num++;
          }
        }
        if(count == count_num)
          return_img.at<uchar>(y,x) = 255;
        else
          return_img.at<uchar>(y,x) = 0;
      }
    }
  }
  return return_img;

}
