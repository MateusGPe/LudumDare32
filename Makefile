ludumdare: main.o groundTileMap.o player.o enemy.o
	g++ -o ludumdare main.o groundTileMap.o player.o enemy.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
