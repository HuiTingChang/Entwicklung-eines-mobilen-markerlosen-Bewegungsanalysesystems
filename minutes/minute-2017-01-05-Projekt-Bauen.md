# Protokoll für 5. Januar 2016 (Do) "Projekt bauen"

Protokoll geschrieben von: Yushin Washio

Teilnehmer: Danylkina Yuliia, Hui-Ting Chang, Yushin Washio

## Tätigkeiten

Heute wird ein Projekt erzeugt, in das später die wii- und
kinect-Funktionen zusammengeführt werden sollen (als einzelne
Klassen?).

Das Projekt sollte im Git-Verzeichnis direkt zu bearbeiten sein,
damit man nicht ständig hin und her kopieren muss und das
Zusammenführen von Modifikationen des Teams erleichtert wird.

Damit hierbei die mühsamen Schritte der Abhängigkeitseinstellungen in
Visual Studio zur Neuerzeugung des Projektes oder zur Anpassung neuer
Abhängigkeiten nicht jedes Mal von jedem Teammitglied vorgenommen
werden müssen, könnte ein CMake- oder qmake-Rezept hilfreich sein,
mit dem man das Visual-Studio-Projekt mitsamt aller nötigen
Abhängigkeiten automatisch erzeugen kann.

* CMake: Es scheint, dass Nuno bereits für sein Body-Projekt eine
  CMakeList verwendet, aber es ist unklar, ob dies für das aktuelle
  Projekt angepasst ist oder nur vom Sample-Projekt von UNancyOwen
  kopiert wurde. Yuliia hat bereits Erfahrung mit dem Schreiben
  einer CMakeList am Betrieb gesammelt, aber hat noch einen gewissen
  Zweifel.
* qmake: Diese Methode ist nicht so leistungsstark wie CMake, um
  installierte Abhängigkeiten automatisch zu erkennen oder
  Einstellungen auf das zu kompilierende Plattform anzupassen,
  jedoch war es für Yushin einfacher, die Syntax zu verstehen, sodass
  er damit bereits für Nunos Body-Projekt eine qmake Projekt-Datei
  (`Body.pro`) erstellt hat und am Montag diese Woche auch mit Nuno
  auf Visual Studio 2013 (statt 2015 bei Yushin) getestet hat (es hat
  noch nicht ganz funktioniet, aber es schien machbar).

Um die Machbarkeit einzuschätzen, haben wir uns vorgenommen, uns für
eine Stunde jeweils CMakeLists (Yuliia) bzw. qmake (Yushin)
auszuprobieren, während Hui-Ting auf ihrem 64bit-System Nunos
Body-Projekt zum Kompilieren bringt.

In Laufe der Stunde hat Yushin die `Body.pro` so angepasst, dass es
auch für Visual Studio 2013 laufen könnte, und hat es mit Hui-Ting
auf ihrem Rechner ausprobiert. Dabei haben wir noch einige Fehler
behoben. Damit dies läuft, müssen Windows-Umgebungsvariablen

* `OPENCV_VER`
* `OPENCV_DIR`
* `KINECTSDK20_DIR`

richtig eingestellt werden. In Laufzeit ist auch die richtige
Einstellung des OPENCV-Eintrags in der `PATH`-Variable auch von
Bedeutung.

Nach einiger Zeit hat Yuliia, nach dem sie noch Bugs im Body-Code
behoben hat, erfolgreich Nunos Projekt mithilfe des qmake-Prozesses
auf Visual Studio 2013 in x86-Architektur gebaut.

Dann haben wir noch, wie Hui-Ting vorschlägt, möglichst alle
Abhängigkeiten lokal im Projektverzeichnis abzulegen, zunächst den
wiiuse-Repository in unsere Repository unter dem `inc`-Verzeichnis
als git submodule eingebunden. Das Bauen von wiiuse per CMake muss
allerdings von jedem Nutzer selbst gemacht werden, da die erzeugten
lib- oder dll-Dateien Plattformabhängig sind.

Anschließend haben wir das Projekt EMMA hinzugefügt, das vom
QtCreator als leere QWidget-Anwendung erzeugt wurde, und dort die
relevanten Abhängigkeiten in die .pro-Datei eingetragen. Dies hat bei
Yuliia bereits erfolgreich gebaut. Die vorhandenen Projekte sollen
dann als Nächstes in dieses Projekt vereint werden.

## Abwesenheiten

* Nuno und Inna waren abwesend, da keine Genehmigung von ihren
  Ausbildern erteilt wurde
* Gilbert war abwesend, da ein Gespräch mit einem Kunden bei seinem
  Betrieb anstand
