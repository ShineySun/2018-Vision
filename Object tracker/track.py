# 2019.12. 17 KMU ESD PROJECT
# Object tracking using pid control
from collections import deque
import numpy as np
import RPi.GPIO as GPIO
import argparse
import cv2
import time

# pid controller class
from pidcal import PidCal

current_x_motor = 7.5
current_y_motor = 8.5

# PID CONTROLLER
def pid_controller(p_x,pid_x,p_y,pid_y):
    global current_x_motor
    global current_y_motor

    if pid_x > 0.104 :
        if current_x_motor < 11.5:
            current_x_motor += 0.025
            p_x.ChangeDutyCycle(current_x_motor)
            print("case1")
            time.sleep(0.002)
    elif pid_x < -0.104:
        if current_x_motor > 3:
            current_x_motor -= 0.025
            p_x.ChangeDutyCycle(current_x_motor)
            print("case2")
            time.sleep(0.002)

    if pid_y > 0.104 :
        if current_y_motor > 3:
            current_y_motor -= 0.025
            p_y.ChangeDutyCycle(current_y_motor)
            print("case3")
            time.sleep(0.002)
    elif pid_y < -0.104:
        if current_y_motor < 11.5:
            current_y_motor += 0.025
            p_y.ChangeDutyCycle(current_y_motor)
            print("case4")
            time.sleep(0.002)


    #else:
    #    if current_x_motor > 8.25:
    #        current_x_motor -= 0.005
    #        p_x.ChangeDutyCycle(current_x_motor)
    #        time.sleep()
    #    elif current_x_motor < 6.25:
    #        current_x_motor += 0.005
    #        p_x.ChangeDutyCycle(current_x_motor)
    #        time.sleep()
    print("x : ", current_x_motor)
    print("y : ", current_y_motor)



# SET GPIO & PIN
pin_x = 18
pin_y = 25

pidcal_x = PidCal()
pidcal_y = PidCal()

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin_x, GPIO.OUT)
GPIO.setup(pin_y, GPIO.OUT)

p_x = GPIO.PWM(pin_x,50)
p_y = GPIO.PWM(pin_y,50)

p_x.start(7.25)
p_y.start(7.25)

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
    help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,
    help="max buffer size")
args = vars(ap.parse_args())

# define the lower and upper boundaries of the "green"
# ball in the HSV color space, then initialize the
# list of tracked points
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
pts = deque(maxlen=args["buffer"])

# WEBCAM RECIEVE
vs = cv2.VideoCapture(-1)

while True:
    # grab the current frame
    ret, frame = vs.read()

    # handle the frame from VideoCapture or VideoStream
    #frame = frame[1] if args.get("video", False) else frame

    # if we are viewing a video and we did not grab a frame,
    # then we have reached the end of the video
    if frame is None:
        break


    # 특정색을 뽑기 위한 색공간 변환
    # Resize 를 하여 라즈베리파에서의 연산량을 줄여 실시간성을 추구함
    frame = cv2.resize(frame, dsize = (500,500),interpolation=cv2.INTER_LINEAR)
    # 가우시안 블러
    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    # HSV 이미지 생성
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # inRange 함수를 이용한 특정 색상 추출
    mask = cv2.inRange(hsv, greenLower, greenUpper)
    #cv2.imshow("HSV",hsv)
    # 노이즈 제거용 침식 팽창 연산
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    # 방향성을 파악하여 군집의 개수를 탐색
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)
    cnts = cnts[1]
    center = None

    # only proceed if at least one contour was found
    if len(cnts) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

        # only proceed if the radius meets a minimum size
        if radius > 10:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv2.circle(frame, (int(x), int(y)), int(radius),
                (0, 255, 255), 2)
            cv2.circle(frame, center, 5, (0, 0, 255), -1)

    # update the points queue
    pts.appendleft(center)

    # loop over the set of tracked points
    for i in range(1, len(pts)):
        # if either of the tracked points are None, ignore
        # them
        if pts[i - 1] is None or pts[i] is None:
            continue

        # 단순 도식화용
        thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
        cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)

    # center point 존재시 검출된 객체의 center 값으로 pid 계산 및 컨트롤
    if center != None:
        pid_x = pidcal_x.pid_control(center[0])
        pid_y = pidcal_y.pid_control(center[1])
        print("pid_X : ", pid_x)
        print("pid_Y : ", pid_y)

        pid_controller(p_x,pid_x,p_y,pid_y)

        p_x.ChangeDutyCycle(current_x_motor)
        p_y.ChangeDutyCycle(current_y_motor)

        time.sleep(0.002)
        #print("pid_Y : ", pid_y)
    # show the frame to our screen
    cv2.imshow("Frame", frame)
    #print(frame.shape[0],frame.shape[1])
    key = cv2.waitKey(1) & 0xFF

    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

vs.release()

# close all windows
cv2.destroyAllWindows()
