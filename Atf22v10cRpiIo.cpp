#include "Atf22v10cRpiIo.hpp"
 #include <iostream>

#ifdef __linux
#include <wiringPi.h>
#endif

// use Broadcom GPIO PIN numbers
const int SDIN   = 14;
const int STB    = 2;
const int SCLK   = 3;
const int SDOUT  = 17;
const int RA0    = 8;
const int RA1    = 15;
const int RA2    = 18;
const int RA3    = 23;
const int RA4    = 24;
const int RA5    = 25;

const int PV     = 4;
const int EDIT   = 22;
const int VCC    = 10;


Atf22v10cRpiIo::Atf22v10cRpiIo()
{

}

Atf22v10cRpiIo::~Atf22v10cRpiIo()
{

}

void
Atf22v10cRpiIo::Init()
{
#ifdef __linux
	std::cout << "init" << std::endl;
	wiringPiSetupGpio();



	pinMode( STB,    OUTPUT );
	pinMode( SDOUT,   INPUT );
	pinMode( SCLK,   OUTPUT );
    pinMode( SDIN,   OUTPUT );
	pinMode( PV,     OUTPUT );
	pinMode( EDIT,   OUTPUT );
	pinMode( VCC,    OUTPUT );
	pinMode( RA0,    OUTPUT );
	pinMode( RA1,    OUTPUT );
	pinMode( RA2,    OUTPUT );
	pinMode( RA3,    OUTPUT );
	pinMode( RA4,    OUTPUT );
	pinMode( RA5,    OUTPUT );
#endif
}


void
Atf22v10cRpiIo::SetSTB( int state )
{
#ifdef __linux
  digitalWrite( STB, state );
#endif
}


void 
Atf22v10cRpiIo::SetPV( int state )
{
#ifdef __linux
  digitalWrite( PV, state );
#endif
}

void 
Atf22v10cRpiIo::SetRow( int row )
{  
 #ifdef __linux
 digitalWrite( RA0, row & 0x01 );
  digitalWrite( RA1, row & 0x02 );
  digitalWrite( RA2, row & 0x04 );
  digitalWrite( RA3, row & 0x08 );
  digitalWrite( RA4, row & 0x10 );
  digitalWrite( RA5, row & 0x20 );
#endif
}

void 
Atf22v10cRpiIo::SetSDIN( int state )
{
#ifdef __linux
 digitalWrite( SDIN, state );
#endif
}

void 
Atf22v10cRpiIo::SetSCLK( int state )
{
#ifdef __linux
  digitalWrite( SCLK, state );
#endif
}

bool 
Atf22v10cRpiIo::GetSDOUT()
{
#ifdef __linux
  return digitalRead( SDOUT ) != 0;;
#else
  return false;
#endif
}


