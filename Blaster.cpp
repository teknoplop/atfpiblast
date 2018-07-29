#include "Blaster.hpp"

#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
//#include <wiringPi.h>

void delayMicroseconds( int usec )
{
    std::this_thread::sleep_for(std::chrono::microseconds( usec ));
}

Blaster::Blaster()
{

}


void
Blaster::Delay(int msec)
{
//    long start=timeGetTime();
//    while(timeGetTime()<start+msec) ;
	std::this_thread::sleep_for(std::chrono::milliseconds( msec ));
}

void 
Blaster::DiscardBits(int n)
{
    while(n-- >0) ReceiveBit();
}

void 
Blaster::TurnOn()
{

    SetVPP(0);    // VPP off
    SetPV(0);     // P/V- low
    SetRow(0x3F); // RA0-5 high
    SetSDIN(1);   // SDIN high
    SetSCLK(1);   // SCLK high
    SetSTB(1);    // STB high
    SetVCC(1);    // turn on VCC (if controlled)
    //    Delay(100);
    delayMicroseconds( 100 ); 
    SetSCLK(0);   // SCLK low
/*
    int vpp;
    switch( _mode )
    {
    	case WRITEGAL:
    	case ERASEGAL:
    	case ERASEALL:
    	case BURNSECURITY:
    	case WRITEPES:
    	case VPPTEST:
    		vpp = 1; // VPP = programming voltage
    		break;

    	default:
    		vpp = 0; // VPP = +12V
    		break;
    }

	SetVPP( vpp );
	*/
}

void 
Blaster::TurnOff()
{
    Delay(100);
    SetPV(0);    // P/V- low
    SetRow(0x3F);// RA0-5 high
    SetSDIN(1);  // SDIN high
    SetVPP(0);   // Vpp off (+12V)
    SetPV(1);    // P/V- high
    Delay(2);
    SetVCC(0);   // turn off VCC (if controlled)
}

bool 
Blaster::ReceiveBit( void )
{
    bool bit;
    bit=GetSDOUT();
    SetSCLK(1);
    delayMicroseconds( 200 );
    SetSCLK(0);
    delayMicroseconds( 200 );
    return bit;
}


void 
Blaster::StrobeRow( int row )
{
#ifdef NOT_FOR_NOW
  // case ATF16V8B:

  SetRow(row);         // set RA0-5 to row number
  Strobe(2);           // pulse /STB for 2ms
#endif

  // case ATF22V10C:
  SetRow(0);    // set RA0-5 low
  SendAddress(6,row); // send row number (6 bits)
  SetSTB(0);
  delayMicroseconds( 100 );
  SetSTB(1);    // pulse /STB
  SetSDIN(0);   // SDIN low

}

/* clock bit out to SDIN */
void
Blaster::SendBit( int bit )
{
    SetSDIN(bit);
    SetSCLK(1);
    delayMicroseconds( 100 );
    SetSCLK(0);
    delayMicroseconds( 100 );
}

void
Blaster::SendAddress( int n, int row )
{
  while(n-- >1)
  {
    SendBit( row & 32 );   // clock in row number bits 5-1
    row <<= 1;
  }
  SetSDIN( row & 32 );       // SDIN = row number bit 0
}

std::shared_ptr< Blaster::FuseArray > 
Blaster::ReadGAL()
{
    // TODO: create helper function
    std::shared_ptr< FuseArray > fusesPtr( new FuseArray( fuses() ) );

    TurnOn();
    SetVPP( 1 );
    ReadFuses( *fusesPtr ) ;
    TurnOff();

    return fusesPtr;
}

void 
Blaster::Strobe( int msec )
{
    delayMicroseconds( 2000 );
    SetSTB(0);
    delayMicroseconds( 2000 );
    SetSTB(1);
    delayMicroseconds( 2000 );
}

void 
Blaster::WriteGal( const FuseArray& fuses )
{
    TurnOn();
    SetVPP( 1 );

#ifdef NOT_FOR_NOW

    {
        switch(gal)
        {
        case GAL16V8:
        case GAL20V8:
            SetPV(1); 
            // write fuse array
            for(row=0;row<galinfo[gal].rows;row++)
            {
                SetRow(row);
                for(bit=0;bit<galinfo[gal].bits;bit++)
                {
                    SendBit(fuses[galinfo[gal].rows*bit+row]);
                }
                Strobe(progtime);
            }
            // write UES
            SetRow(galinfo[gal].uesrow);
            for(bit=0;bit<64;bit++)
            {
                SendBit(fuses[galinfo[gal].uesfuse+bit]);
            }
            Strobe(progtime);
            // write CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<82;bit++)
            {
                switch(pes[2])
                {
                case 0x00:
                    SendBit(fuses[cfg16V8[bit]]);
                    break;
                case 0x1A:
                    SendBit(fuses[cfg16V8AB[bit]]);
                    break;
                case 0x20:
                    SendBit(fuses[cfg20V8[bit]]);
                    break;
                case 0x3A:
                    SendBit(fuses[cfg20V8AB[bit]]);
                    break;
                }
            }
            Strobe(progtime);
            SetPV(0);
            break;
        case ATF16V8B:
#endif

            SetPV(1);

            // write fuse array
            for( int row=0; row < rows(); row++ )
            {
                SetRow( row );
                for( int bit = 0; bit < bits(); bit++ )
                {
                    SendBit( fuses[ ( rows() * bit ) + row ] );
                }
                Strobe( progtime() );
            }

            // write UES
            SetRow( uesrow() );
            for( int bit = 0; bit < 64; bit++ )
            {
                SendBit( fuses[ uesfuse() + bit ] );
            }
            Strobe( progtime() );

            // write CFG
            SetRow( cfgrow() );
            for( int bit = 0; bit < 82; bit++ )
            {
                SendBit( fuses[ cfg()[ bit ] ] );
            }
            Strobe( progtime() );
            
            SetPV(0);

#ifdef NOT_FOR_NOW            
            break;
        case GAL22V10:
        case ATF22V10B:
            // write fuse array
            SetRow(0);               // RA0-5 low
            for(row=0;row<galinfo[gal].rows;row++)
            {
                for(bit=0;bit<galinfo[gal].bits;bit++)
                {
                    SendBit(fuses[galinfo[gal].rows*bit+row]);
                }
                SendAddress(6,row);  // send address 6 bits
                SetPV(1);
                Strobe(progtime);
                SetPV(0);
            }
            // write UES
            if(gal==ATF22V10B)
            {
               SendBits(68,1);                              // fill first 68 bits
               for(bit=0;bit<64;bit++)
               {
                  SendBit(fuses[galinfo[gal].uesfuse+bit]); // send UES bits
               }
            }
            else
            {
               for(bit=0;bit<64;bit++)
               {
                  SendBit(fuses[galinfo[gal].uesfuse+bit]); // send UES bits
               }
               SendBits(68,0);                              // fill next 68 bits
            }
            SendAddress(6,galinfo[gal].uesrow);
            SetPV(1);
            Strobe(progtime);
            SetPV(0);
            // write CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<galinfo[gal].cfgbits;bit++)
            {
                SendBit(fuses[galinfo[gal].cfg[bit]]);
            }
            SetPV(1);
            Strobe(progtime);
            SetPV(0);
            break;
        case ATF22V10C:
            SetRow(0);      // RA0-5 low
            for(row=0;row<galinfo[gal].rows;row++)
            {
                for(bit=0;bit<galinfo[gal].bits;bit++)
                {
                    SendBit(fuses[galinfo[gal].rows*bit+row]);
                }
                SendAddress(6,row);  // send address 6 bits
                SetPV(1);
                Strobe(progtime);
                SetPV(0);
            }
            // write UES
            SendBits(68,1);
            for(bit=0;bit<64;bit++)
            {
                SendBit(fuses[galinfo[gal].uesfuse+bit]);
            }
            SendAddress(6,galinfo[gal].uesrow);
            SetPV(1);
            Strobe(progtime);
            SetPV(0);
            // write CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<19;bit++)
            {
                SendBit(fuses[galinfo[gal].cfg[bit]]); // clock out bits 0-62
            }
            SetSDIN(fuses[galinfo[gal].cfg[19]]); // send bit 63
            SetPV(1);
            Strobe(progtime);
            SetPV(0);
            // disable power-down feature (JEDEC bit #5892)
            SetRow(0);
            SendAddress(6,59);
            SetPV(1);
            Strobe(progtime);
            SetPV(0);
        }
#endif

    TurnOff();
}

void
Blaster::ReadPES( PesArray& pes )
{
    TurnOn();

    StrobeRow( pesrow() );
    delayMicroseconds( 500 );    
    for ( int byte = 0; byte < pesbytes(); byte++ )
    {
        pes[ byte ] = 0;
        for ( int bitmask = 0x1; bitmask <= 0x80; bitmask <<= 1 )
        {
            if ( ReceiveBit() )
           	{
            	pes[ byte ] |= bitmask;
            }
        }
    } 

    TurnOff();
}

bool 
Blaster::TestProperGAL()
{
	PesArray pes;

    ReadPES( pes );
    if ( VerifyPesType( pes ) )
    {
    	return ParsePes( pes );
    }

    return false;
}




