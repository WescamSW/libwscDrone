#!/usr/bin/python3
# import logging
from CppPyBebop_IF import PyBebop
# import libwscDrone
import threading
import time
import cv2
import numpy as np

# global drone

drone = PyBebop(3)

# def moveDroneThread():

takeoffThread = threading.Thread(target=drone.takeoff)

# def missionCmds

def imgProc():
  while (True):
    buff = cv2.cvtColor(drone.getBuff(), cv2.COLOR_RGB2BGR)
    cv2.imshow('buff',buff)
    # gray = cv2.cvtColor(buff, cv2.COLOR_BGR2GRAY)
    # ret, gb = cv2.threshold(gray,128,255,cv2.THRESH_BINARY)

    # gb = cv2.bitwise_not(gb)

    # contour,hier = cv2.findContours(gb,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)

    # for cnt in contour:
    #     cv2.drawContours(gb,[cnt],0,255,-1)
    # gray = cv2.bitwise_not(gb)

    # cv2.drawContours(gray,contour,-1,(0,255,0),3)

    # cv2.imshow('test', gray)
    keyRes = cv2.waitKey(1) & 0xFF

    if keyRes == ord('q'):
      break
    elif keyRes == ord('e'):
      print ("brb Dying")
      drone.die()
      takeoffThread.join()
      break
    elif keyRes == ord('t'):
      takeoffThread.start()
    elif keyRes == ord('w'):
      drone.moveRelativeMeters(0,0.5)
    elif keyRes == ord('s'):
      drone.moveRelativeMeters(0,-0.5)
    elif keyRes == ord('a'):
      drone.moveRelativeMeters(0.5,0)
    elif keyRes == ord('d'):
      drone.moveRelativeMeters(-0.5,0)
    elif keyRes == ord('o'):
      drone.land()


def main():
   print("starting")
  #  drone = libwscDrone.PyBebop(3)
   print ("battery level: " + str(drone.getBatteryLevel()))
   procThread = threading.Thread(target=imgProc)
   procThread.start()  
  #  t1 = threading.Thread(target=drone.takeoff)
  #  t2 = threading.Thread(target=drone.die)
  #  t3 = threading.Thread(target=thread1)

  #  print ("About to start takeoff")
  #  t1.start()
  #  t3.start()
  #  print ("Sleeping 5")
  #  time.sleep(8)
  #  print ("Time to die")
  #  t2.start()

   
  #  t1.join()
  #  t2.join()

   print("exited")

if __name__ == "__main__":
   main()