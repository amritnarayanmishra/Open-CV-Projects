import cv2
import mediapipe as mp
import requests
import time

ESP_IP = "http://192.168.141.109"  # ← Put ESP32 IP Here

mpHands = mp.solutions.hands
hands = mpHands.Hands(max_num_hands=1)
mpDraw = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0)

tipIds = [4, 8, 12, 16, 20]

def send(cmd):
    try:
        requests.get(f"{ESP_IP}/{cmd}")
        time.sleep(0.1)
    except:
        print("ESP32 Not Connected ⚠️")

while True:
    success, img = cap.read()
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)

    if results.multi_hand_landmarks:
        hand = results.multi_hand_landmarks[0]
        lm = hand.landmark
        
        fingers = []

        # Thumb
        if lm[tipIds[0]].x < lm[tipIds[0] - 1].x:
            fingers.append(1)
        else:
            fingers.append(0)

        # other 4 fingers
        for id in range(1, 5):
            if lm[tipIds[id]].y < lm[tipIds[id] - 2].y:
                fingers.append(1)
            else:
                fingers.append(0)

        total = fingers.count(1)
        print("Fingers:", total)

        if total == 1: send("d4on")
        elif total == 2: send("d4off")
        elif total == 3: send("d2on")
        elif total == 4: send("d2off")
        elif total == 5: send("allon")
        elif total == 0: send("alloff")

        mpDraw.draw_landmarks(img, hand, mpHands.HAND_CONNECTIONS)

    cv2.imshow("Hand Gesture", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
