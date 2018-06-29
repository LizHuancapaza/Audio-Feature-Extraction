# """
#     author: Liz Huancapaza H.
# """
compile:
	g++ -std=c++11 src/Main.cpp -o ./bin/fe -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
run:
# resultados serao visualizados na pasta ./output/go_stop/
# 	python3 ./src/Spectrogram.py
	./bin/fe "./input/spectromams/go_stop/" "./output/go_stop/"
	python3 ./src/Classification.py ./output/go_stop/

