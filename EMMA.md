# C++ Projekt Emma

## Prerequisites

You need Visual Studio 2013 or 2015 including Visual C++ tools for Windows
desktop, Qt5, OpenCV 2.4.13 or 3.10 (other versions may work).

Further, after cloning this repository, run (in the Gruppe-13 root directory)

```shell
git submodule init
git submodule update --depth 1
# depth 1 is optional, but prevents from downloading the whole history
```

to get the [rpavlik/wiiuse](https://github.com/rpavlik/wiiuse) repository. Then
also run CMake for the directory [inc/wiiuse](/inc/wiiuse) with a (not yet
existing) `inc/wiiuse/build` directory as build destination. Then **configure**,
**uncheck all the examples** (esp. the example_sdl, because that requires some
dependencies that are otherwise not needed) and **generate**. Then open the
generated project with Visual Studio and build it. It will give you an error in
[os_win.c#L123](https://github.com/rpavlik/wiiuse/blob/master/src/os_win.c#L123),
so as described in the known
[issue 86](https://github.com/rpavlik/wiiuse/issues/86), replace
`WM_PLUS_PRODUCT_ID` by `WM_PRODUCT_ID_TR`.

## Traditional way

1. Visual Studio öffnen
    
    Das Projekt wird in Ordner der Repositorium gebaut!
    
    
	a) Datei ->  Neues Projekt -> Qt5 Projects -> Qt Application -> Name eingeben "EMMA" (ohne zeichnen)  -> Weiter-> Weiter- > Basis Classe : QWidget -> Beenden 

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