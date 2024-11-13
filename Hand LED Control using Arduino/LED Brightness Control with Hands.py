import numpy as np
import cv2
import pyfirmata
import time
from cvzone.HandTrackingModule import HandDetector

# Initialize Arduino and LED pin
comport = 'COM10'  # Change this to your COM port
board = pyfirmata.Arduino(comport)
it = pyfirmata.util.Iterator(board)
it.start()

led_pin = board.get_pin('d:9:p')  # Use PWM on pin 9 for brightness control

# Hand detector initialization
detector = HandDetector(detectionCon=0.8, maxHands=1)

# Capture video from webcam
video = cv2.VideoCapture(0)

if not video.isOpened():
    print("Error: Could not open video stream.")
    exit()

while True:
    ret, frame = video.read()
    if not ret:
        print("Error: Failed to capture frame.")
        break

    frame = cv2.flip(frame, 1)
    hands, img = detector.findHands(frame)

    if hands:
        myHand = hands[0]
        lmList = myHand['lmList']

        if lmList:
            # Get the positions of the thumb and index finger
            thumb_pos = lmList[4]  # Thumb tip (landmark 4)
            index_pos = lmList[8]  # Index finger tip (landmark 8)

            # Calculate the distance between thumb and index finger
            distance = int(np.linalg.norm(np.array(thumb_pos) - np.array(index_pos)))
            print(f"Distance between fingers: {distance}")  # Debug: Print distance

            # Normalize the distance to control brightness
            min_distance = 50  # Minimum distance for low brightness
            max_distance = 200  # Maximum distance for high brightness

            # Clamp the distance
            if distance < min_distance:
                brightness = 0.0  # Dim (close fingers)
            elif distance > max_distance:
                brightness = 1.0  # Bright (far apart fingers)
            else:
                # Normalize distance to [0.0, 1.0]
                brightness = (distance - min_distance) / (max_distance - min_distance)

            # Set LED brightness
            led_pin.write(brightness)
            print(f"Brightness set to: {brightness}")  # Debug: Print brightness level

    # Show the frame
    cv2.imshow("Hand Tracking", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# Release resources
video.release()
cv2.destroyAllWindows()
