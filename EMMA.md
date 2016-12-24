# C++ Projekt Emma

## Traditional way

1. Visual Studio öffnen
    
    Das Projekt wird in Ordner der Repositorium gebaut!
    
    
	a) Datei ->  Neues Projekt -> Qt5 Projects -> Qt Application -> Name eingeben "EMMA" (ohne zeichnen)  -> Weiter-> Weiter- > Basis Classe : QWidget -> Beenden 

## Qt Way

### the Quick Qt Way

1. Open the file [EMMA.pro](/EMMA/EMMA/EMMA.pro) with the Qt Creator.
2. Configure it for your system by following the dialogue that comes up the first time you open that project file. That’s it!

### the semi-traditional way

1. Visual Studio öffnen
    (**no** need to create a project or solution)
2. Qt VS Tools -> Open Qt Project File (.pro)...
3. Open the file [EMMA.pro](/EMMA/EMMA/EMMA.pro) et voilà it creates a project with all the includes and libs!
    (you do need to manually copy the dll to the build directory or the user needs to install it in a `path` of the environment)