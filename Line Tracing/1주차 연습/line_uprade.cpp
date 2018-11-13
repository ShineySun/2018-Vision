#include <highgui.h>
#include <cv.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const int LeftLine = 120;
const int RightLine = 320;

void cutting_Img(Mat &img)
{
  for(int y = 0; y<img.rows; y++)
  {
    for(int x = 0; x<img.cols;x++)
    {
      if(y < img.rows/2 || x<LeftLine ||  x>RightLine || (3.5*x+3*y) < 1800){
        img.at<uchar>(y,x) = 0;
      }
      //img.at<uchar>(y,x) = 0;
    }
  }
}
int main()
{
  // 동영상 읽기
  VideoCapture lines("left.avi");

  // 동영상 읽기 예외 처리
  if(!lines.isOpened())
  {
    cout << "Can't open the video" << endl;
  }

  // 비디오 프레임 설정
  //lines.set(CAP_PROP_FRAME_WIDTH,320);
  //lies.set(CAP_PROP_FRAME_WIDTH,240);

  Mat inputImg;
  Mat grayImg,binaryImg,cannyImg,lineImg;
  int n = 0;
  // 비디오 출력
  while(true)
  {

    // 동영상을 Mat 객체로 읽기
    lines >> inputImg;
    //cout << inputImg.cols << ' ' << inputImg.rows << endl;

    // 이진화를 위한 gray scale 변환
    cvtColor(inputImg,binaryImg,COLOR_BGR2GRAY);

    cutting_Img(binaryImg);
    // 스무딩
    // GaussianBlur(binaryImg,binaryImg,Size(7,7),0);

    // 영상 이진화
    threshold(binaryImg,binaryImg,100,255,THRESH_BINARY);

    // Canny mask 를 이용한 경계 검출
    Canny(binaryImg,cannyImg,50,150,5);  // input,output,threshold1,threshold2,

    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(cannyImg, lines, 1, CV_PI/180, 100, 0, 0 ); // runs the actual detection

    int count = lines.size();
    float sum1X = 0;
    float sum1Y = 0;
    float sum2X = 0;
    float sum2Y = 0;
    Point pt1,pt2;
    // // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
       //cout << lines.size() << endl;
       float rho = lines[i][0], theta = lines[i][1];
       double a = cos(theta), b = sin(theta);
       double x0 = a*rho, y0 = b*rho;
       pt1.x = cvRound(x0 + 1000*(-b));
       pt1.y = cvRound(y0 + 1000*(a));
       pt2.x = cvRound(x0 - 1000*(-b));
       pt2.y = cvRound(y0 - 1000*(a));
       // line( inputImg, pt1, pt2, Scalar(0,255,0), 2, CV_AA);
       sum1X += pt1.x;
       sum1Y += pt1.y;
       sum2X += pt2.x;
       sum2Y += pt2.y;
    }
    // 평균값으로 대체.. polyfit 써보고 싶다..
    pt1.x = sum1X/count;
    pt1.y = sum1Y/count;
    pt2.x = sum2X/count;
    pt2.y = sum2Y/count;

    // avg 값으로 line 그리기
    line( inputImg, pt1, pt2, Scalar(0,255,0), 2, CV_AA);

    // 이미지 출력
    imshow("Input",inputImg);
    imshow("Binary",binaryImg);
    imshow("Canny",cannyImg);

    if(waitKey(30) == 1);
  }
  return 0;
}
