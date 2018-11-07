
#include <highgui.h>
#include <cv.h>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

/*
  이미지를 이진화 하는 함수
  이미지 레퍼런스를 가져와 이진화
*/
void setBinaryImg(Mat &img)
{
    for(int y = 0; y<img.rows;y++)
    {
      for(int x = 0; x< img.cols;x++)
      {
        if(img.at<uchar>(y,x) < 110)  // 임계값 100 이 현재 동영상에서 이상적, 그림자 지나갈때 차선도 인식하고 노이즈도 무시..
        {
          img.at<uchar>(y,x) = 0;
        }
        else
        {
          img.at<uchar>(y,x) = 255;
        }
      }
    }
    return;
}

int main()
{
  // read the video
  VideoCapture video1("left.avi");
  // exception check
  if(!video1.isOpened())
  {
    printf("Failed to open the video\n");
  }
  //set video frame
  video1.set(CAP_PROP_FRAME_WIDTH,320);
  video1.set(CAP_PROP_FRAME_HEIGHT,240);
  // cvNamedWindow("camera");

  Mat output;
  Mat gray,canny,line1,gray2;
  Mat result;
  Rect rect(130,370,180,109);  // interest 영역을 하드 코딩으로 정해놓음
  /*
  궁금한 점 .. roi 영역을 하드코딩이 아닌 자동으로 할당 할 수 있나??
  */


  while(true)
  {
    video1 >> output;

    cvtColor(output,gray,COLOR_BGR2GRAY); // gray scale 변환

    gray = output(rect);  // roi 설정
    //gray2 = output(rect); // roi 설정


    //cvtColor(gray,gray2,COLOR_BGR2GRAY);
    cvtColor(gray,line1,COLOR_BGR2GRAY);


    // 이진화
    threshold(gray,line1,100, 255, THRESH_BINARY); //library 사용
    setBinaryImg(line1);  // 내가 만든 함수 사용

    // canny 함수를 활용한 경계 값 추출
    Canny(line1,canny,70,150,3);

    //직선 성분을 검출합니다.
    vector<Vec4i> lines;
    HoughLinesP(canny, lines, 1, CV_PI/180,  30, 30, 3);  // houghlinep 함수가 좀 더 나은 결과..

    for( int i=0; i<lines.size(); i++ )
    {
        Vec4i L = lines[i];
        //cout << lines[i] << endl;
        line(gray, Point(L[0],L[1]), Point(L[2],L[3]),
             Scalar(0,0,255), 1, LINE_AA );
    }

    imshow("output",output);
    imshow("canny",canny);
    //imshow("left side",gray);
    //imshow("result",result);

    if(waitKey(30) == 1);
  }

  return 0;
}
