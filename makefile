all:clean program run

del:
	rm -f *.dat

program:
	g++ -w -std=c++17 Backup.cpp Change.cpp EntityIO.h FileNotOpenException.cpp FileOpenFailedException.cpp \
	LifeCycleController.cpp Main.cpp PrintController.cpp Product.cpp ProductRelease.cpp \
	RecordNotFoundException.cpp Request.cpp Requester.cpp ScenarioController.cpp UI.cpp -o program && ./program

run:
	./program

clean:
	rm -f program *.o
