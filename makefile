file = raytracer
folder = a5
options = -Wall -I. -c -O3

all: $(file)

$(file): Eigen/ main.o IObot.o Scene.o Model.o Calculon.o Eye.o Camera.o Sphere.o Light.o Material.o Ray.o
	g++ -Wall -I. *.o -std=c++14 -o $(file)
	
IObot.o: IObot.cpp IObot.h
	g++ $(options) IObot.cpp -std=c++14
Scene.o: Scene.cpp Scene.h
	g++ $(options) Scene.cpp -std=c++14
Model.o: Model.cpp Model.h
	g++ $(options) Model.cpp -std=c++14
Calculon.o: Calculon.cpp Calculon.h
	g++ $(options) Calculon.cpp -std=c++14
Eye.o: Eye.cpp Eye.h
	g++ $(options) Eye.cpp -std=c++14
Camera.o: Camera.cpp Camera.h
	g++ $(options) Camera.cpp -std=c++14
Sphere.o: Sphere.cpp Sphere.h
	g++ $(options) Sphere.cpp -std=c++14
Light.o: Light.cpp Light.h
	g++ $(options) Light.cpp -std=c++14
Material.o: Material.cpp Material.h
	g++ $(options) Material.cpp -std=c++14
Ray.o: Ray.cpp Ray.h
	g++ $(options) Ray.cpp -std=c++14

main.o: main.cpp IObot.h 
	g++ $(options) main.cpp -std=c++14

touch:
	touch *.cpp *.h
tar: clean untar
	mkdir $(folder)
	cp -v *.cpp *.h makefile $(folder)/
	tar -cvf $(folder)/$(file).tar *.cpp *.h makefile README.txt Eigen/
	mkdir $(folder)/test/
	mv -v $(folder)/$(file).tar $(folder)/test/
	tar -xvf $(folder)/test/$(file).tar -C $(folder)/test/
untar:
	rm -vrf $(folder)
#copy:	
clean:
	rm -vf *.o $(file)





