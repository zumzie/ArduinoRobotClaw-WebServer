Work in progress...

# Description:

Uses Arudino ESP32 to create a web server which you're able to control servos from the web server. The servo controls are html sliders.
Also the arduino code implements a django local ran server to utilize your webcam which also uses opencv.


Using ngrok to tunnel multiple host. The arduino code needs to be modified for the img src link. 
It needs to be the domain for the web cam server. If it remains local, it will not appear for people who visit.



# What's next:

- Plan to fix and style the html page with css.
- Need to figure out how to speed up requests. takes abotu 1.5 seconds per servo request.


# Reference code:
- Web server servo code by BARRAGAN and modified by Scott Fitzgerald, Rui Santos https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/

- Django opencv https://www.youtube.com/watch?v=xz9MvyKGYio&t
