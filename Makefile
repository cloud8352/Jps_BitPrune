
cppFlags=

main:AStar Jps
	g++ ${cppFlags} -o FindPath main.cpp AStar.o jps.o

AStar:
	g++ ${cppFlags} -c AStar.cpp

Jps:
	g++ ${cppFlags} -c jps.cpp

clean:
	rm -rf *.o FindPath.exe FindPath