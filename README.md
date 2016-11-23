# Entwicklung-eines-mobilen-markerlosen-Bewegungsanalysesystems

> :mega: Please help to fill in/correct

## For developers

### Kinect Sensor v2.0 (for XBox One, with Windows adapter)

- Hardware Requirements:
    - USB 3.0, preferably Intel or NEC host controller ([source](https://github.com/OpenKinect/libfreenect2))
- Required drivers
    - [Kinect for Windows Runtime v2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44559)
      - runs on Windows 8, 8.1, 10 (you might want to consider using Windows to go with the RWTH licene from Microsoft Imagine, but consider the required disk space for Visual Studio!)
    - Runtime v1.8 runs on Windows 7, but USB 3.0 support is incomplete and **not useable for Kinect Sensor v2.0** anyway ((source)[https://social.msdn.microsoft.com/Forums/en-US/45830eac-f7ed-41fe-8621-8a7bcb84cebb/use-kinect-v2-with-windows-7?forum=kinectv2sdk])
    - alternative: (runs on Windows 7, Ubuntu, Mac OS, ...) [libfreenect2](https://github.com/OpenKinect/libfreenect2) of the project [OpenKinect](http://www.openkinect.org/)
- IDE
    - [Kinect for Windows SDK v2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)
      - examples can be found at [the codeplex repository](https://kinectforwindows.codeplex.com/), one of which is [the coloringbook](https://github.com/Microsoft/Windows-appsample-coloringbook)
      - [developer documentation](https://msdn.microsoft.com/library/dn799271.aspx)
      - requires Visual Studio 2012 or 2013, currently trying with [2015 Community Edition](https://www.visualstudio.com/downloads/) anyway
        - VS uses 14 GB disk space and installation takes veeeeery long!
        - Visual Studio 2015 Community Edition couldn't be installed on Windows 10 **to go** on 2016-11-18 using Julia's USB key drive at her laptop with Windows 7
    - freenect2: use any C++ IDE, documentation can be found at the [OpenKinect Doxygen site](https://openkinect.github.io/libfreenect2/) and examples [in the examples directory of its release](https://github.com/OpenKinect/libfreenect2/tree/master/examples)
- Faking it without the actual camera
    - [Fakenect](https://openkinect.org/wiki/Fakenect) from OpenKinect, provides some sample outputs for download
    - [the Kinect SDK record/play-back feature](https://msdn.microsoft.com/en-us/library/dn785516.aspx)

## Wii Balance Board

- Required drivers
    - probably best and free and open: [xwiimote](https://dvdhrm.github.io/xwiimote/)
    - for other systems on [wiibrew](http://wiibrew.org/wiki/Wiimote/Library)

### AnyBody

Multi-body human biomechanics simulation tool; Some researchers at the HIA are interested in using in the future.

http://www.anybodytech.com/

One alternative open tool is [OpenSim](http://simtk-confluence.stanford.edu:8080/display/OpenSim/User%27s+Guide)
  
## Organization

- Project doc:
    - https://www.fh-aachen.de/fileadmin/people/fb09_terstegge/SWE_16_17/Gruppe_13.pdf
- Place:
    - [HIA](http://www.hia.rwth-aachen.de/) 2.Stock Zimmer 2.19 
