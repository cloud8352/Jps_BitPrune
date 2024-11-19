main:AStar Jps
	g++ -o FindPath main.cpp astar.o jps.o

AStar:
	g++ -c astar.cpp

Jps:
	g++ -c jps.cpp

clean:
	rm -rf *.o FindPath.exe FindPath