import cv2
import pyfirmata
import time
from cvzone.HandTrackingModule import HandDetector

# Initialize the hand detector
detector = HandDetector(detectionCon=0.8, maxHands=1)

# Capture video from the webcam
video = cv2.VideoCapture(0)

# Check if the camera is opened
if not video.isOpened():
    print("Error: Could not open video stream.")
    exit()

print("Video stream opened successfully.")

# Specify the COM port for the Arduino connection
comport = 'COM10'

# Initialize communication with the Arduino board
board = pyfirmata.Arduino(comport)

# Start an iterator thread to prevent overflow of the board's serial buffer
it = pyfirmata.util.Iterator(board)
it.start()

# Define the pins for the LEDs
led_1 = board.get_pin('d:8:o')   # Digital pin 8 as output
led_2 = board.get_pin('d:9:o')   # Digital pin 9 as output
led_3 = board.get_pin('d:10:o')  # Digital pin 10 as output
led_4 = board.get_pin('d:11:o')  # Digital pin 11 as output
led_5 = board.get_pin('d:13:o')  # Digital pin 12 as output

# Function to control the LEDs based on the input list 'fingerUp'
def led(fingerUp):
    # Turn off all LEDs initially to reset the state
    led_1.write(0)
    led_2.write(0)
    led_3.write(0)
    led_4.write(0)
    led_5.write(0)

    # Now set LEDs based on the fingerUp data
    if fingerUp == [0, 0, 0, 0, 0]:
        pass  # All LEDs remain off
    
    elif fingerUp == [0, 1, 0, 0, 0]:
        led_1.write(1)  # Turn on LED 1
    
    elif fingerUp == [0, 1, 1, 0, 0]:
        led_1.write(1)  # Turn on LED 1
        led_2.write(1)  # Turn on LED 2
    
    elif fingerUp == [0, 1, 1, 1, 0]:
        led_1.write(1)
        led_2.write(1)
        led_3.write(1)  # Turn on LEDs 1, 2, and 3
    
    elif fingerUp == [0, 1, 1, 1, 1]:
        led_1.write(1)
        led_2.write(1)
        led_3.write(1)
        led_4.write(1)  # Turn on LEDs 1, 2, 3, and 4
    
    elif fingerUp == [1, 1, 1, 1, 1]:
        led_1.write(1)
        led_2.write(1)
        led_3.write(1)
        led_4.write(1)
        led_5.write(1)  # Turn on all LEDs

while True:
    ret, frame = video.read()

    # Check if frame was captured successfully
    if not ret:
        print("Error: Failed to capture frame.")
        break

    # Flip the frame horizontally
    frame = cv2.flip(frame, 1)

    # Find hands in the frame
    hands, img = detector.findHands(frame)

    # Debugging: Check what `hands` returns
    if hands:
        print("Hands detected:", hands)  # Print detected hands for debugging

        # Access the first hand in the list
        myHand = hands[0]  # Get the first detected hand
        lmList = myHand['lmList']  # Access the landmarks

        # Check finger status and send data to Arduino
        fingerUp = detector.fingersUp(myHand)  # Pass the whole hand data instead of lmList
        led(fingerUp)  # Control LEDs based on finger status

        # Display finger count on the frame
        finger_count = sum(fingerUp)
        cv2.putText(frame, f'Finger count: {finger_count}', (20, 460), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 255, 255), 1, cv2.LINE_AA)
    else:
        print("No hands detected.")  # Print message if no hands are detected

    # Show the frame with a wait time for smoother display
    cv2.imshow("Hand Tracking", frame)

    # Break if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord("q"):  # Wait for 1ms for better performance
        break

# Release resources
video.release()
cv2.destroyAllWindows()
