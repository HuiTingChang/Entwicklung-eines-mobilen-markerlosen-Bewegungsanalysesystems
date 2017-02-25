# Minute of 16 February 2017 (Thu) "Checkerboard"

By Yushin Washio

Attendance: Inna Kh, Nuno Alves, Gilbert Nzondzhou, Yuliia Danylkina, Yushin Washio

## Activities

### Checkerboard

Yuliia and Inna have successfully determined the position of the
checkerboard fixed at the Balance Board and developed a way to transform
the position of the center of pressure from the Balance Board
coordinates into the Kinect body coordinate system.

### Pointer issues with wiiuse

Last time some issues arose when testing the Balance Board input
actually connecting with the device, caused by some restructuring
done while integrating it with the CurrentState class. This has
been solved by Yuliia on Tuesday, but Yushin noticed that the new
solution requires C++14, whilst the current project was setup for
C++11. Qt supports C++14 (just add `CONFIG += c++14`) if [at least
Qt 5.4]
(http://stackoverflow.com/questions/26127217/ddg#27458874).
Yushin tried an alternative solution compatible with C++11. After
testing it on Nuno’s machine this solution has been merged into
master.

## Integrating Balance Board capturing loop into the main application

Up to now, Balance Board data has been captured in a busy loop
without using a timer. Now Yuliia has added the `board_timer` to the
`Body_Widget` class and is planning to use it from
`BalanceBoardThread` analogously to the way `main_timer` is being
used by `CameraCapture`.

## StreamIO writeReadTwice test case

Yushin is developing `StreamIO` for continuously writing captured
data on the disk. It is working for writing just once, as reading
will produce the original `CurrentState` (though not all attributes
are integrated yet). However, he’s still working to fix the test case
of writing and reading two instances.

## Center of Mass

Nuno has found a [good resource]
(http://www.ele.uri.edu/faculty/vetter/BME207/anthropometric-data.pdf)
for human mass distribution and partial centers of gravity data needed
for calculating the center of mass and is going to add this into a new
header file `Anatomy.h` and use it to draw the center of mass on the
body canvas.

## Absence

* Hui-Ting is on vacation (announced, to be compensated by home office)
