import cv2 # opencv 사용
import numpy as np

def canny():
    edge=cv2.Canny(gray_img,70,210)
    return edge

def ROI(gray_img, vertices, color3=(255,255,255), color1=255): # ROI 셋팅
    mask = np.zeros_like(gray_img) # mask = img와 같은 크기의 빈 이미지
    if len(gray_img.shape) > 2: # Color 이미지(3채널)라면 :
        color = color3
    else: # 흑백 이미지(1채널)라면 :
        color = color1 # vertices에 정한 점들로 이뤄진 다각형부분(ROI 설정부분)을 color로 채움
    cv2.fillPoly(mask, vertices, color) # 이미지와 color로 채워진 ROI를 합침
    ROI_image = cv2.bitwise_and(gray_img, mask)
    return ROI_image

def draw_lines(gray_img, lines, color=[0, 0, 255], thickness=2): # 선 그리기
    for line in lines:
        for x1,y1,x2,y2 in line:
            cv2.line(gray_img, (x1, y1), (x2, y2), color, thickness)

def hough_lines(gray_img, rect, rho, theta, threshold, min_line_len, max_line_gap): # 허프 변환
    lines = cv2.HoughLinesP(gray_img, rho, theta, threshold, np.array([]), minLineLength=min_line_len, maxLineGap=max_line_gap)
    line_img = np.zeros((gray_img.shape[0], gray_img.shape[1], 3), dtype=np.uint8)
    draw_lines(rect, lines)
    return rect

def addImage(gray_img, origin_img, α=1, β=1., λ=0.): # 두 이미지 operlap 하기
    return cv2.addWeighted(origin_img, α, gray_img, β, λ)

cap = cv2.VideoCapture('left.avi') # 동영상 불러오기

# origin_img = cv2.imread('solidWhiteCurve.jpg', cv2.IMREAD_COLOR)
# gray_img = cv2.imread('solidWhiteCurve.jpg', cv2.IMREAD_GRAYSCALE)#이진화

while(cap.isOpened()):
    # image 변수에 동영상 캡쳐(사진 형태라 생각하면 쉬움)
    ret, image = cap.read() # ret : True(Frame is read correctly) or false

    #rectangle
    #rect = cv2.rectangle(gray_img,(135,300),(300,475), (0, 255, 0)
    # set ROI
    rect = image[340:475,135:300]

    # grayscale 변환(threshold 사용하기 위함)
    gray_img = cv2.cvtColor(rect,cv2.COLOR_RGB2GRAY)



    # vertices = np.array[[(135,300),(300,475)],dtype=np.int32]


    # 잡음 제거 하기 위한 이진화 연산
    # cv2.threshold(소스 이미지(grayscale 이어야만 됨..), 임계값(ex : 255//2) , MaxValue : 255 , Type : cv2.THRESH_BINARY )
    # output : ret(임계값인거 같음..) , thresh(이진화된 이미지)
    ret,thresh = cv2.threshold(gray_img,110,255,cv2.THRESH_BINARY)

    height, width = image.shape[:2] # 이미지 높이, 너비

    # vertices = np.array([[(50,height),(width/2-45, height/2+60), (width/2+45, height/2+60), (width-50,height)]], dtype=np.int32)
    # print(vertices)
    # cv2.GaussianBlur(소스 이미지, mask size, ??)
    cv2.GaussianBlur(thresh,(5,5),0) # Blur 효과

    #cv2.Canny(소스 이미지, 임계값(minval),임계값(maxval))
    canny_img = cv2.Canny(thresh,70,210)

    hough_img = hough_lines(canny_img, rect, 1, 1 * np.pi/180, 30, 10, 20) # 허프 변환
    #
    #
    #result = addImage(hough_img,rect) # 원본 이미지에 검출된 선 overlap



    # cv2.imshow('result',result)
    cv2.imshow('gray',gray_img)
    cv2.imshow('original',image)
    cv2.imshow('rect',rect)
    # cv2.imshow('roi',ROI_img)
    cv2.imshow('binarization',thresh)
    cv2.imshow('hough',hough_img)
    cv2.imshow('Canny',canny_img)

    # # gray_img = grayscale(image) # 흑백이미지로 변환

    # vertices = np.array([[(50,height),(width/2-45, height/2+60), (width/2+45, height/2+60), (width-50,height)]], dtype=np.int32)
    # print(vertices)
    # ROI_img = ROI(canny_img, vertices) # ROI 설정
    # hough_img = hough_lines(ROI_img, 1, 1 * np.pi/180, 30, 10, 20) # 허프 변환
    # result = addImage(hough_img,image) # 원본 이미지에 검출된 선 overlap
    # cv2.imshow('result',result) # 결과 이미지 출력
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break # Release
cap.release()
cv2.destroyAllWindows()
