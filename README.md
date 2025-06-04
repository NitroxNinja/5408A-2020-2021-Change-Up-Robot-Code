# 5408A-2020-2021-Change-Up-Robot-Code
Team 5408A's Change Up Code 

This is a good idea!

![image](https://github.com/user-attachments/assets/6dcbbf7d-8923-402e-9389-6e9f2b84dc5f)

## Intro 

This code has been in development since 2019, when I first started to learn how to program. What motivated me to make this code, was the lack of an autonomous system for my Tower Takeover Robot (you can see the gallery in orbiterAll repository in Gallery.md), and most espicially the COVID-19 in 2020, which sort of forced me to be stuck at home learning a whole bunch of things. This represents a platform of how I started to program and how this led to a very successful robot even before all the fancy schmancy libraries that provided the answers for you. This features the usage of IMU and 2 lateral tracking wheels. To start, just download `finalDynamoProject.v5code` into your editor (like VEXcode PRO on Visual Studio Code) with all the correct files and header files, although I don't intend for you to take from this code and write your own, use it to inspire you perhaps. (I'm mostly writing this for historical purposes). 

I did do a little experimentation on a full 3 tracking wheel + IMU system, and I believe you could see some remnants of it in this code, but I may have deleted everything. 

## Driver-focused 

There is also very little drive macros and automated tasks. **This is a driver's robot. Not a neat robot.** Everything is raw control and skill. It's supposed to be a **viseral experience to drive one of my robots!** Even my drivetrain uses voltage control instead of PID (but this does come at some consequences such as low torque in both ends of the RPM limits), and features a autolocking drive (most people complain that this may damage the motors, but if you drive properly, it shouldn't). It uses arguably the most difficult to control setting, right hand single-stick arcade, and absolutely no power curves to the thing, just linear pure movement. 

Out of all the robots I've made, this robot had the worst handling. This was mostly due to the center of gravity of this robot being shifted too far forward, and the weight was perhaps a little too much for the 4 motor 2.57" omni+traction drive. But regardless it beat out my division and almost won the 2021 Texas State Championship had my partner not stalled their program. 

Pooper mode is perhaps one of the few tasks in which I included some automation, espicially in regards to line sensors. It essentially acts as a way for me to intake and hold balls in the big blue/red rubber band wheel, but if I want to discard that ball in the rear of my robot, I press a button and intake, and press back to do so, vice versa. 

## Some files

There are a couple files in my code that you may want to read about. 

- `main.cpp` is where the code is run. `int main()` whatever. It features the PID code that I used, with a whole lot of `resetDriveSensors = true;` and very little linkage of movements. 
- `main.h` represents where all the function names are stored, which can be accesses more throughly in the individual files such as `opControl.cpp` and `functions.cpp`
- `robot-config.cpp` and `robot-config.h` is where I define all the motors, sensors 
- Driver control functions are located in `opControl.cpp` and functions can be listed in main.h (which isn't on this repo, go check my other repos), and use a right hand single-stick arcade drive, which allows me (the driver) to do other tasks with the left hand. People in countries that drive RHD will find this workable, however most people may find this weird, so they can move it to their left hand. To be honest figuring out macros and other driver widgets is pretty easy,`if, else` and whatnot can be used. 
- Pre autonomous functions could be located in `functions.cpp` such as resetting the inertial sensor. 

If you are still learning how to code, I highly recommend watching some videos on control structures, boolean statements, variables, functions, and more basic C++ topics. I have no idea how I taught myself C++ so good luck!

## Autonomous Routine Methodology

To adjust the lateral and turning PID, simply go to the `kP`, `kI`, and `kD` and tune accordingly. You can always adjust these values on the go, I think you could make a function to do that or something. 

There is an autonomous button selector, that when you run this code, 8 buttons will pop up. You can possibly add more but I think 8 is enough. Put your autonomous code into the autonomous function where there are `if` statements with the corresponding function/button. You could make a function to do that but nah. 

If you go to the autonomous function of the code, you may notice that I turn "on" my drivePID, but in my driver code I turn "off" my drivePID. This will be important as you do want your robot to move accurately in autonomous, but you do not want your robot to move against your will (of your controller) in drive control. 

## Autonomous Programming Tips and Remarks

### i. How to use this code
'`dV`' and '`dTV`' are short for "desiredValue" and "desiredTurnValue" respectively. The reason why I named it like this was so that it would be easier for me to input instructions on the go. '`resetDriveSensors`' is a boolean variable, that once is set to true, will reset all the IMEs and IMUs so that you can achieve a new movement from that pose. I will warn you to avoid using `resetDriveSensors` when possible and simply learn how to link movements by connecting the lines of code together, and adjusting the `task::sleep();` values. You can also make a function for to do that or something. Let's take a look at an example of how to do movements: 

dd

### ii. Tips in developing autonomous routines (KISS, Keep it simple, stupid)
Make movements simple as possible. Make it fast. Make it reasonable. Why go in a curve when you just drive in a straight line? More crazier movements mean more crazier readings from your IMU and IMEs (aka this is called noise). There is a lack of kalman filtering on this system, and you're gonna have to be careful. 
But you may be asking why the hell would I do that? Well VEX is a very small field, and it doesn't have to dive deep into the real world sh-t of nonlinear systems, and special instrumentation devices. You should be able to go very far based on this code, and create more functions and make my code a WHOLE lot neater to help you simplify your programming. 

### iii. Include some comments 
I know CS teachers make a huge deal out of this, but to be honest take a chill pill. You don't always have to include comments on obivious things, just include comments where they may be complicated movements. In the code snippet above, I just included a lot to help you get a more intuitive sense on how our team programs. In reality, I just include one comment like "the robot is (should be) doing a right curve turn into the goal". Don't spend all your time on documentation.

### iv. Improve this code 
You get this. Don't just take this code at face value. There's so much work that can be done to this program which can make moving your robot better. I don't suggest using the same code for more than 4 years like how I did. I have an excuse to use the same code since our team didn't really invest into programming R&D, and most of the time it was me just building the robot and figuring out ideas rather than figuring out how to code. Try to make improvements to it every week or atleast monthly. 

You can also alternatively switch to another coding library like EZ-Template or LemLib, but I am different. 

## Conclusion 

This code was the start to most of the programming I do. I want to note that most of this robot was mostly mechanical developments and learning how to become a better driver throughout time. 
