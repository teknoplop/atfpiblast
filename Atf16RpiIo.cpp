#include "Atf22v10cRpiIo.hpp"

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




static std::vector< int > cfg16V8AB
{
    2048,2049,2050,2051,
    2193,
    2120,2121,2122,2123,
    2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143,2144,2145,2146,2147,2148,2149,2150,2151,2152,2153,2154,2155,2156,2157,2158,2159,2160,2161,2162,2163,2164,2165,2166,2167,2168,2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,2186,2187,2188,2189,2190,2191,
    2124,2125,2126,2127,
    2192,
    2052,2053,2054,2055
};

void
GpioAtf16::Init()
{
#ifdef __linux

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
	/*

	bool v = false;
	while ( true )
	{
	    digitalWrite( STROBE, HIGH );
	    v != v;
		
	    int status = digitalRead( SDIN );
	    std::cout << "Status: " << status << std::endl;
	    delay( 2000 );

	    digitalWrite( STROBE, LOW );
	    delay( 2000 );
	}
	*/
}


const std::vector< int >&
GpioAtf16::cfg() const
{
	return cfg16V8AB;
}

int
GpioAtf16::cfgbits() const
{
	return cfg16V8AB.size();
}

void
GpioAtf16::SetSTB( int state )
{
#ifdef __linux
  digitalWrite( STB, state );
}


void 
GpioAtf16::SetPV( int state )
{
#ifdef __linux
  digitalWrite( PV, state );
}

void 
GpioAtf16::SetRow( int row )
{  
 #ifdef __linux
 digitalWrite( RA0, row & 0x01 );
  digitalWrite( RA1, row & 0x02 );
  digitalWrite( RA2, row & 0x04 );
  digitalWrite( RA3, row & 0x08 );
  digitalWrite( RA4, row & 0x10 );
  digitalWrite( RA5, row & 0x20 );
}

void 
GpioAtf16::SetSDIN( int state )
{
 #ifdef __linux
 digitalWrite( SDIN, state );
}

void 
GpioAtf16::SetSCLK( int state )
{
#ifdef __linux
  digitalWrite( SCLK, state );
}

bool 
GpioAtf16::GetSDOUT()
{
#ifdef __linux
  return digitalRead( SDOUT ) != 0;;
}

bool 
GpioAtf16::VerifyPesType( PesArray& pes )
{
/*
    if ( pes[7] == 'F' && pes[6] == '2' && pes[5] == '2' && pes[4]=='V' && pes[3]=='1' && pes[2]=='0')
    {
       if (pes[1]=='B')
       {
           type = ATF22V10B;
       }
       else
       {
           type = ATF22V10C;
       }
    }
    else */
    return pes[6]=='F' && pes[5]=='1' && pes[4]=='6' && pes[3]=='V' && pes[2]=='8';

/*
    else if (pes[2]!=0x00 && pes[2]!=0xFF)
    {
       for(type=sizeof(galinfo)/sizeof(galinfo[0]);type;type--)
       {
           if(pes[2]==galinfo[type].id0||pes[2]==galinfo[type].id1) break;
       }
    }
*/
}

bool 
GpioAtf16::ParsePes( const PesArray& pes )
{
/*    int algo;

    switch (gal)
    {
    case ATF16V8B:
    case ATF22V10B:
    case ATF22V10C:

        progtime=10;
        erasetime=100;
        vpp=48;    // 12.0V

    break;
    default:
        algo=pes[1]&0x0F;
        if(algo==5)
        {
            erasetime=(25<<((pes[4]>>2)&7))/2;
            progtime=duration[((((unsigned)pes[5]<<8)|pes[4])>>5)&15];
            vpp=2*((pes[5]>>1)&31)+20;
        }
        else switch(gal)
        {
        case GAL16V8:
        case GAL20V8:
        erasetime=100;
            switch(algo)
            {
            case 0:
                vpp=63; // 15.75V
                progtime=100;
                break;
            case 1:
                vpp=63; // 15.75V
                progtime=80;
                break;
            case 2:
                vpp=66; // 16.5V
                progtime=10;
                break;
            case 3:
                vpp=pes[3]==NATIONAL?60:58; // 15.0V or 14.5V
                progtime=40;
                break;
            case 4:
                vpp=56; // 14V
                progtime=100;
                break;
            }
            break;
        default:
            erasetime=pes[3]==NATIONAL?50:100;
            switch(algo)
            {
            case 0:
                vpp=66; // 16.5V
                progtime=10;
                break;
            case 1:
                vpp=63; // 15.75V
                progtime=100;
                break;
            case 2:
                vpp=pes[3]==NATIONAL?60:58; // 15.0V or 14.5V
                progtime=40;
                break;
            case 3:
                vpp=56; // 14V
                progtime=100;
                break;
            }
        }
    }
    */
    return true;
}

/*
std::shared_ptr< Gal::FuseArray >
GpioAtf16::CreateFuseArray()
{
	return std::shared_ptr< Gal::FuseArray > ( new Gal::FuseArray( 2194 ) );
}
*/