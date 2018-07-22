
CPPFLAGS=-std=c++11 

atf16blastrpi: Blaster.o GpioAtf16.o atf16blastrpi.o Jedec.o
	g++ -lwiringPi -o $@ $^

Blaster.o : Blaster.cpp Blaster.hpp Gal.hpp
	g++ -c Blaster.cpp $(CPPFLAGS)

GpioAtf16.o : GpioAtf16.cpp GpioAtf16.hpp
	g++ -c GpioAtf16.cpp $(CPPFLAGS)

Jedec.o : Jedec.cpp Jedec.hpp Blaster.hpp
	g++ -c Jedec.cpp $(CPPFLAGS)

atf16blastrpi.o : atf16blastrpi.cpp Blaster.hpp GpioAtf16.hpp Jedec.hpp
	g++ -c atf16blastrpi.cpp $(CPPFLAGS)


