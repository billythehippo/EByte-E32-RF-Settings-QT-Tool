# EByte-E32-RF-Settings-QT-Tool

This is a QT GUI tool to set up the Ebyte E32 radio modems.
It was made like Original Windows tool
but Crossplatform and English only (at this moment).

# QuickStart

Connect your modem to port with reset M1 and M0 (see Datasheet for E32 modems);
Type the name of this port in the Line and press "Open Port" button.
If you see the message "Port is open!" and the button "Get Param" is enabled - just press it!
If you don't see the message box "Parameters are GOT!" press it again (its a bug!).
When parameters are got you may change them and write them with "Set Param" button.

Also you may save and load the modem configurations.
For example: if you read from modem and save to file default config C0 00 00 1A 17 44,
the config string in the file will look like C000001A1744 - just text without spaces.

# BUILD and INSTALLATION

Classic:
1. qmake
2. make
3. (optionnaly) (sudo) make install

Another way is to open with QT Creator and build.

# TODO

1. Fix the bug with "Get Param" button function
2. Change the manual port name input to selection.

# INFO

This software is FREE and comes AS IS.
The author is not responsible for the use of it and for any bugs.
But author will be grateful for any help to fix the bugs and to make it better! :)

Also here is an Linux executable file compiled with QT 5.15 under Manjaro Linux.
