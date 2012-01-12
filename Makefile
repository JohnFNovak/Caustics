Caustics : Caustics.o
	g++ -O3 -m64 Caustics.o -L/usr/local/root-v5.24.00/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lfreetype -pthread -Wl,-rpath,/usr/local/root-v5.24.00/lib -lm -ldl  -o Caustics
Caustics.o : Caustics.C
	g++  -O3 -pipe -Wall -W -Woverloaded-virtual -D_REENTRANT -pthread -m64 -I/usr/local/root-v5.24.00/include -c Caustics.C
