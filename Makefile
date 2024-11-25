
cppFlags=

main:AStar BitPruneJps
	g++ ${cppFlags} -o FindPath main.cpp AStar.o BitPruneJps.o

AStar:
	g++ ${cppFlags} -c AStar.cpp

BitPruneJps:
	g++ ${cppFlags} -c BitPruneJps.cpp

clean:
	rm -rf *.o FindPath.exe FindPath