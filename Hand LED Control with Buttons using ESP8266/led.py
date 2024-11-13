import numpy as np
import cv2
import serial
from cvzone.HandTrackingModule import HandDetector

# Initialize serial communication with ESP8266
esp = serial.Serial('COM7', 9600)  # Adjust COM port as necessary

# Hand detector initialization
detector = HandDetector(detectionCon=0.8, maxHands=1)

# Capture video from webcam
video = cv2.VideoCapture(0)

if not video.isOpened():
    print("Error: Could not open video stream.")
    exit()

# Define positions and sizes for "On" and "Off" buttons for each LED
led1_on_position = (100, 100)   # X, Y coordinates for LED 1 ON button
led1_off_position = (100, 200)  # X, Y coordinates for LED 1 OFF button
led2_on_position = (300, 100)   # X, Y coordinates for LED 2 ON button
led2_off_position = (300, 200)  # X, Y coordinates for LED 2 OFF button
button_size = (150, 50)         # Width and Height of the buttons

# Function to detect if the index finger is touching a button
def is_finger_touching(finger_pos, button_pos, size):
    x, y = button_pos
    width, height = size
    return x < finger_pos[0] < x + width and y < finger_pos[1] < y + height

# Track previous touch status to avoid multiple commands sent on a single touch
previous_touch_1_on = False
previous_touch_1_off = False
previous_touch_2_on = False
previous_touch_2_off = False

while True:
    ret, frame = video.read()
    if not ret:
        print("Error: Failed to capture frame.")
        break

    frame = cv2.flip(frame, 1)
    hands, img = detector.findHands(frame)

    # Draw the buttons for LED 1 and LED 2 on the screen
    cv2.rectangle(frame, led1_on_position, (led1_on_position[0] + button_size[0], led1_on_position[1] + button_size[1]), (0, 255, 0), -1)
    cv2.putText(frame, "LED 1 ON", (led1_on_position[0] + 20, led1_on_position[1] + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    cv2.rectangle(frame, led1_off_position, (led1_off_position[0] + button_size[0], led1_off_position[1] + button_size[1]), (0, 0, 255), -1)
    cv2.putText(frame, "LED 1 OFF", (led1_off_position[0] + 20, led1_off_position[1] + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    cv2.rectangle(frame, led2_on_position, (led2_on_position[0] + button_size[0], led2_on_position[1] + button_size[1]), (0, 255, 0), -1)
    cv2.putText(frame, "LED 2 ON", (led2_on_position[0] + 20, led2_on_position[1] + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    cv2.rectangle(frame, led2_off_position, (led2_off_position[0] + button_size[0], led2_off_position[1] + button_size[1]), (0, 0, 255), -1)
    cv2.putText(frame, "LED 2 OFF", (led2_off_position[0] + 20, led2_off_position[1] + 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

    if hands:
        myHand = hands[0]
        lmList = myHand['lmList']

        if lmList:
            index_pos = lmList[8]  # Index finger tip position

            # Check if the index finger is touching the "ON" or "OFF" buttons for LED 1
            if is_finger_touching(index_pos, led1_on_position, button_size):
                if not previous_touch_1_on:
                    esp.write(b'1\n')  # Send command to turn LED 1 ON
                    print("Turning LED 1 ON")
                    previous_touch_1_on = True
            else:
                previous_touch_1_on = False

            if is_finger_touching(index_pos, led1_off_position, button_size):
                if not previous_touch_1_off:
                    esp.write(b'0\n')  # Send command to turn LED 1 OFF
                    print("Turning LED 1 OFF")
                    previous_touch_1_off = True
            else:
                previous_touch_1_off = False

            # Check if the index finger is touching the "ON" or "OFF" buttons for LED 2
            if is_finger_touching(index_pos, led2_on_position, button_size):
                if not previous_touch_2_on:
                    esp.write(b'2\n')  # Send command to turn LED 2 ON
                    print("Turning LED 2 ON")
                    previous_touch_2_on = True
            else:
                previous_touch_2_on = False

            if is_finger_touching(index_pos, led2_off_position, button_size):
                if not previous_touch_2_off:
                    esp.write(b'3\n')  # Send command to turn LED 2 OFF
                    print("Turning LED 2 OFF")
                    previous_touch_2_off = True
            else:
                previous_touch_2_off = False

    # Show the frame with the buttons and current state
    cv2.imshow("Hand Tracking", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources
video.release()
cv2.destroyAllWindows()
esp.close()
