# C++ Projekt Emma

## Prerequisites

You need Visual Studio 2013 or 2015 including Visual C++ tools for Windows
desktop, Qt5, OpenCV 2.4.13 or 3.10 (other versions may work).

Further, after cloning this repository, run (just once in the Gruppe-13 root
directory)

```shell
git submodule init
git submodule update --depth 1
# depth 1 is optional, but prevents from downloading the whole history
```

to get the [inc/wiiuse](https://github.com/rpavlik/wiiuse) and the
[inc/inih](https://github.com/benhoyt/inih) submodules. Then
also run CMake (make sure to enable adding it to the `PATH` when installing!)
for the directory [inc/wiiuse](/inc/wiiuse) with a (not yet
existing) `inc/wiiuse/build` directory as build destination. Then **configure**,
**uncheck all the examples** (esp. the example_sdl, because that requires some
dependencies that are otherwise not needed) and **generate**. Then open the
generated project with Visual Studio and build it. It will give you an error in
[os_win.c#L123](https://github.com/rpavlik/wiiuse/blob/master/src/os_win.c#L123),
so as described in the known
[issue 86](https://github.com/rpavlik/wiiuse/issues/86), replace
`WM_PLUS_PRODUCT_ID` by `WM_PRODUCT_ID_TR`.

Define the OpenCV enviroment variables `OPENCV_VER` and `OPENCV_DIR` according
to [the manual method described here](https://msdn.microsoft.com/en-us/library/dn188695.aspx)
(because the provided script doesn't seem to work for OpenCV 3.10) and add
OpenCV to the `PATH` variable. You need to re-login to your Windows user session
before running qmake again.

You also need the [Kinect for Windows SDK v2.0]
(https://www.microsoft.com/en-us/download/details.aspx?id=44561).

Runtime v1.8 runs on Windows 7, but USB 3.0 support is incomplete and **not
useable for Kinect Sensor v2.0** anyway ((source)
[https://social.msdn.microsoft.com/Forums/en-US/45830eac-f7ed-41fe-8621-8a7bcb84cebb/use-kinect-v2-with-windows-7?forum=kinectv2sdk]
)

[libfreenect2](https://github.com/OpenKinect/libfreenect2) of the project
[OpenKinect](http://www.openkinect.org/) runs on Windows 7, Ubuntu, Mac
OS, ..., but unfortunately turned out to be **not really a feasible
alternative** because it doesn’t provide out-of-the-box solutions for human
body recognition, but only [interfaces for raw camera and depth data]
(https://openkinect.org/wiki/High_Level))

- examples can be found at [the codeplex repository]
    (https://kinectforwindows.codeplex.com/), one of which is
    [the coloringbook]
    (https://github.com/Microsoft/Windows-appsample-coloringbook)
- [developer documentation](https://msdn.microsoft.com/library/dn799271.aspx)
- requires Visual Studio 2012 or 2013, currently trying with
    [2015 Community Edition](https://www.visualstudio.com/downloads/) anyway
    - VS uses 14 GB disk space and installation takes veeeeery long!
    - Visual Studio 2015 Community Edition couldn't be installed on
        Windows 10 **to go** on 2016-11-18 using Julia's USB key drive at her
        laptop with Windows 7
- ~~freenect2: use any C++ IDE, documentation can be found at the
    [OpenKinect Doxygen site](https://openkinect.github.io/libfreenect2/) and
    examples [in the examples directory of its release]
    (https://github.com/OpenKinect/libfreenect2/tree/master/examples)~~ (we’ll
    use Microsoft Kinect SDK)
- Faking it without the actual camera
    - ~~[Fakenect](https://openkinect.org/wiki/Fakenect) from OpenKinect,
        provides some sample outputs for download~~ (we’ll use Microsoft Kinect SDK)
    - [the Kinect SDK record/play-back feature]
        (https://msdn.microsoft.com/en-us/library/dn785516.aspx)

## Qt Way

### the Quick Qt Way

1. Open the file [EMMA.pro](/EMMA/EMMA/EMMA.pro) with the Qt Creator.
2. Configure it for your system by following the dialogue that comes up the first time you open that project file. That’s it!

### the semi-traditional way (we probably go this way!)

1. Visual Studio öffnen
    (**no** need to create a project or solution)
2. Qt VS Tools -> Open Qt Project File (.pro)...
3. Open the file [EMMA.pro](/EMMA/EMMA/EMMA.pro) et voilà it creates a project with all the includes and libs!
    (you do need to manually copy the dll to the build directory or the user needs to install it in a `path` of the environment)

## Manual way (unsupported)

1. Visual Studio öffnen
    
    Das Projekt wird in Ordner der Repositorium gebaut!
    
    
	a) Datei ->  Neues Projekt -> Qt5 Projects -> Qt Application -> Name eingeben "EMMA" (ohne zeichnen)  -> Weiter-> Weiter- > Basis Classe : QWidget -> Beenden
