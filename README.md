# Entwicklung eines mobilen, markerlosen Bewegungsanalysesystems

[![Gitter](https://badges.gitter.im/gitterHQ/gitter.svg)](https://gitter.im/ghinyy/Lobby)

> :mega: Please help to fill in/correct

TODO briefly describe the project!

For the time being, refer to the [official project description]
(https://www.fh-aachen.de/fileadmin/people/fb09_terstegge/SWE_16_17/Gruppe_13.pdf)

## For developers

### Kinect Sensor v2.0 (for XBox One, with Windows adapter)

Get started by installing Visual Studio, Kinect SDK 2.0 (ships with Runtime 2.0) and following the [Kinect for Windows Programming Guide](https://msdn.microsoft.com/en-us/library/dn782037.aspx). You’re also advised to take a look at the Body Basics-D2D sample by going to [Project/Solution](https://msdn.microsoft.com/en-us/library/dn782040.aspx).

- Hardware Requirements:
    - USB 3.0, preferably Intel or NEC host controller ([source](https://github.com/OpenKinect/libfreenect2))
- Required drivers
    - **We’ll use this**: [Kinect for Windows Runtime v2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44559)
      - runs on Windows 8, 8.1, 10 (~~you might want to consider using Windows to go with the RWTH licene from Microsoft Imagine, but consider the required disk space for Visual Studio!~~ → didn’t work on Windows to go until now)
    - Runtime v1.8 runs on Windows 7, but USB 3.0 support is incomplete and **not useable for Kinect Sensor v2.0** anyway ((source)[https://social.msdn.microsoft.com/Forums/en-US/45830eac-f7ed-41fe-8621-8a7bcb84cebb/use-kinect-v2-with-windows-7?forum=kinectv2sdk])
    - not really a feasible alternative: ~~(runs on Windows 7, Ubuntu, Mac OS, ...) [libfreenect2](https://github.com/OpenKinect/libfreenect2) of the project [OpenKinect](http://www.openkinect.org/)~~ (because it doesn’t provide out-of-the-box solutions for human body recognition, but only [interfaces for raw camera and depth data](https://openkinect.org/wiki/High_Level))
- IDE
    - **We’ll use this**: [Kinect for Windows SDK v2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)
      - examples can be found at [the codeplex repository](https://kinectforwindows.codeplex.com/), one of which is [the coloringbook](https://github.com/Microsoft/Windows-appsample-coloringbook)
      - [developer documentation](https://msdn.microsoft.com/library/dn799271.aspx)
      - requires Visual Studio 2012 or 2013, currently trying with [2015 Community Edition](https://www.visualstudio.com/downloads/) anyway
        - VS uses 14 GB disk space and installation takes veeeeery long!
        - Visual Studio 2015 Community Edition couldn't be installed on Windows 10 **to go** on 2016-11-18 using Julia's USB key drive at her laptop with Windows 7
    - ~~freenect2: use any C++ IDE, documentation can be found at the [OpenKinect Doxygen site](https://openkinect.github.io/libfreenect2/) and examples [in the examples directory of its release](https://github.com/OpenKinect/libfreenect2/tree/master/examples)~~ (we’ll use Microsoft Kinect SDK)
- Faking it without the actual camera
    - ~~[Fakenect](https://openkinect.org/wiki/Fakenect) from OpenKinect, provides some sample outputs for download~~ (we’ll use Microsoft Kinect SDK)
    - [the Kinect SDK record/play-back feature](https://msdn.microsoft.com/en-us/library/dn785516.aspx)

## Wii Balance Board

- Required drivers
    - https://github.com/rpavlik/wiiuse (CMake required) 
    
### AnyBody

Multi-body human biomechanics simulation tool; Some researchers at the HIA are interested in using in the future.

http://www.anybodytech.com/

One alternative open tool is [OpenSim](http://simtk-confluence.stanford.edu:8080/display/OpenSim/User%27s+Guide)

## Software development tools

- Revision control
    - We’ll probably use a repository on the [FH GitLab](https://git.noc.fh-aachen.de/explore/projects). Learn more about GitLab [in its documentation](https://docs.gitlab.com/ce/README.html)
    - A good way to get started with the git workflow, no matter where your repository is hosted, is to follow [the simple guide](http://rogerdudler.github.io/git-guide/) for a quick starter and refer to the [full docs](https://git-scm.com/doc) for more specific cases
    - Those "*.md" files are [markdown](https://docs.gitlab.com/ce/user/markdown.html)

## Organization

- Communication:
    - [ghinny/Lobby](https://gitter.im/ghinyy/Lobby) at gitter (it can integrate with git repositories)
        - Find [documentatin here](https://gitter.zendesk.com/hc/en-us/categories/200024041-RTFM)
        - It supports some [GitHub-Flavored Markdown](https://guides.github.com/features/mastering-markdown/)
- Project doc:
    - https://www.fh-aachen.de/fileadmin/people/fb09_terstegge/SWE_16_17/Gruppe_13.pdf
- Place:
    - [HIA](http://www.hia.rwth-aachen.de/) 2nd floor [mediTEC Room 2.19](http://www.meditec.hia.rwth-aachen.de/en/chair/employees/) (contact supervisor Matías de la Fuente, AOR Dr.-Ing.)
