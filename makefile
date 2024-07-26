all: program

program: Change.o EntityIO.o FileNotOpenException.o FileOpenFailedException.o \
          LifeCycleController.o Main.o PrintController.o Product.o ProductRelease.o \
          RecordNotFoundException.o Request.o Requester.o ScenarioController.o UI.o
	g++ -Wall -std=c++17 -o program Change.o EntityIO.o FileNotOpenException.o FileOpenFailedException.o \
          LifeCycleController.o Main.o PrintController.o Product.o ProductRelease.o \
          RecordNotFoundException.o Request.o Requester.o ScenarioController.o UI.o

Change.o: Change.cpp Change.h
	g++ -Wall -std=c++17 -c Change.cpp

EntityIO.o: EntityIO.cpp EntityIO.h
	g++ -Wall -std=c++17 -c EntityIO.cpp

FileNotOpenException.o: FileNotOpenException.cpp FileNotOpenException.h
	g++ -Wall -std=c++17 -c FileNotOpenException.cpp

FileOpenFailedException.o: FileOpenFailedException.cpp FileOpenFailedException.h
	g++ -Wall -std=c++17 -c FileOpenFailedException.cpp

LifeCycleController.o: LifeCycleController.cpp LifeCycleController.h
	g++ -Wall -std=c++17 -c LifeCycleController.cpp

Main.o: Main.cpp
	g++ -Wall -std=c++17 -c Main.cpp

PrintController.o: PrintController.cpp PrintController.h
	g++ -Wall -std=c++17 -c PrintController.cpp

Product.o: Product.cpp Product.h
	g++ -Wall -std=c++17 -c Product.cpp

ProductRelease.o: ProductRelease.cpp ProductRelease.h
	g++ -Wall -std=c++17 -c ProductRelease.cpp

RecordNotFoundException.o: RecordNotFoundException.cpp RecordNotFoundException.h
	g++ -Wall -std=c++17 -c RecordNotFoundException.cpp

Request.o: Request.cpp Request.h
	g++ -Wall -std=c++17 -c Request.cpp

Requester.o: Requester.cpp Requester.h
	g++ -Wall -std=c++17 -c Requester.cpp

ScenarioController.o: ScenarioController.cpp ScenarioController.h
	g++ -Wall -std=c++17 -c ScenarioController.cpp

UI.o: UI.cpp UI.h
	g++ -Wall -std=c++17 -c UI.cpp

clean:
	rm -f program *.o
