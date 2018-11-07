
CPPFLAGS=-std=c++11 -g

atf16blastrpi: Atf22v10cBlaster.o Blaster.o  atf16blastrpi.o Jedec.o Atf22v10cRpiIo.o
	g++ -g -lwiringPi -o $@ $^

Atf22v10cBlaster.o : Atf22v10cBlaster.cpp Atf22v10cBlaster.hpp Blaster.hpp
	g++ -c Atf22v10cBlaster.cpp $(CPPFLAGS)

Blaster.o : Blaster.cpp Blaster.hpp GalIo.hpp
	g++ -c Blaster.cpp $(CPPFLAGS)

Atf22v10cRpiIo.o : Atf22v10cRpiIo.cpp Atf22v10cRpiIo.hpp
	g++ -c Atf22v10cRpiIo.cpp $(CPPFLAGS)

Jedec.o : Jedec.cpp Jedec.hpp Blaster.hpp
	g++ -c Jedec.cpp $(CPPFLAGS)

atf16blastrpi.o : atf16blastrpi.cpp Blaster.hpp Atf22v10cRpiIo.hpp Jedec.hpp
	g++ -c atf16blastrpi.cpp $(CPPFLAGS)


