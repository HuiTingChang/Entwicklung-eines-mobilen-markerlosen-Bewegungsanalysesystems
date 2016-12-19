Was braucht man für wii board ? 

1. Bib wurde von hier geladen:
https://github.com/rpavlik/wiiuse
Für Kompelieren braucht man CMake programm

2. 
Linken ( Add verfahren) 

C/C++-> Allgemein -> 
C:\Users\yuliia\Documents\Visual Studio 2013\Projects\wiiuse\src

Linker-> Allgemein 
C:\Users\yuliia\Documents\Visual Studio 2013\Projects\wiiuse\wii\src\Debug
Linker->  Eingabe -> wiiuse_debug.lib



API : 
http://public.vrac.iastate.edu/vancegroup/docs/wiiuse/
https://public.vrac.iastate.edu/vancegroup/docs/wiiuse/db/db2/group__internal__wiiboard.html


Beispiel:
https://github.com/rpavlik/wiiuse/blob/master/example/example.c
https://github.com/lefevren/CryVR/blob/master/src/Flownodes/Wiimote/FlowWiimoteControlerDebug.cpp
https://public.vrac.iastate.edu/vancegroup/docs/wiiuse/d2/d31/wiiboard_8c_source.html


TEST CODE 


	 wiimote** wiimotes = wiiuse_init(2);    
	 int found = wiiuse_find(wiimotes, 2, 60);
       
	 if (!found) {
	 	printf("No wiimotes found.\n");
	 }
    
	 int connected = wiiuse_connect(wiimotes, 2);
	 if (connected) {
	 	printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	 }
	 else {
	 	printf("Failed to connect to any wiimote.\n");
	  
	 }
	  
	 wiimote* wm = wiimotes[0];



	 /* wii balance board */
	 struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
	 float total = wb->tl + wb->tr + wb->bl + wb->br;
	 float x = ((wb->tr + wb->br) / total) * 2 - 1;
	 float y = ((wb->tl + wb->tr) / total) * 2 - 1;
	 printf("Weight: %f kg @ (%f, %f)\n", total, x, y);
	 printf("Interpolated weight: TL:%f  TR:%f  BL:%f  BR:%f\n", wb->tl, wb->tr, wb->bl, wb->br);
	 printf("Raw: TL:%d  TR:%d  BL:%d  BR:%d\n", wb->rtl, wb->rtr, wb->rbl, wb->rbr);


