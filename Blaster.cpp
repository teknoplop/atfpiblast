#include "Blaster.hpp"

#include <chrono>
#include <thread>
#include <algorithm>
#include <wiringPi.h>

Blaster::Blaster( std::shared_ptr< Gal >& g ) 
: _gal( g )
{

}

static void 
Delay(int msec)
{
//    long start=timeGetTime();
//    while(timeGetTime()<start+msec) ;
	std::this_thread::sleep_for(std::chrono::milliseconds( msec ));
}

void 
Blaster::TurnOn()
{

    _gal->SetVPP(0);    // VPP off
    _gal->SetPV(0);     // P/V- low
    _gal->SetRow(0x3F); // RA0-5 high
    _gal->SetSDIN(1);   // SDIN high
    _gal->SetSCLK(1);   // SCLK high
    _gal->SetSTB(1);    // STB high
    _gal->SetVCC(1);    // turn on VCC (if controlled)
    //    Delay(100);
    delayMicroseconds( 100 ); 
    _gal->SetSCLK(0);   // SCLK low
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

	_gal->SetVPP( vpp );
	*/
}

void 
Blaster::TurnOff()
{
    Delay(100);
    _gal->SetPV(0);    // P/V- low
    _gal->SetRow(0x3F);// RA0-5 high
    _gal->SetSDIN(1);  // SDIN high
    _gal->SetVPP(0);   // Vpp off (+12V)
    _gal->SetPV(1);    // P/V- high
    Delay(2);
    _gal->SetVCC(0);   // turn off VCC (if controlled)
}

bool 
Blaster::ReceiveBit( void )
{
    bool bit;
    bit=_gal->GetSDOUT();
    _gal->SetSCLK(1);
    delayMicroseconds( 200 );
    _gal->SetSCLK(0);
    delayMicroseconds( 200 );
    return bit;
}


void 
Blaster::StrobeRow( int row )
{
#ifdef NOT_FOR_NOW
  // case ATF16V8B:

  _gal->SetRow(row);         // set RA0-5 to row number
  Strobe(2);           // pulse /STB for 2ms
#endif

  // case ATF22V10C:
  _gal->SetRow(0);    // set RA0-5 low
  SendAddress(6,row); // send row number (6 bits)
  _gal->SetSTB(0);
  delayMicroseconds( 100 );
  _gal->SetSTB(1);    // pulse /STB
  _gal->SetSDIN(0);   // SDIN low

}

/* clock bit out to SDIN */
void
Blaster::SendBit( int bit )
{
    _gal->SetSDIN(bit);
    _gal->SetSCLK(1);
    delayMicroseconds( 100 );
    _gal->SetSCLK(0);
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
  _gal->SetSDIN( row & 32 );       // SDIN = row number bit 0
}

std::shared_ptr< Gal::FuseArray > 
Blaster::ReadGAL()
{
    std::shared_ptr< Gal::FuseArray > fusesPtr;

    int row,bit;

    TurnOn();
    _gal->SetVPP( 1 );



#ifdef NOT_FOR_NOW
    switch(gal)
    {
    case GAL16V8:
    case GAL20V8:
    // read fuse rows
    for(row=0;row<galinfo[gal].rows;row++)
    {
       StrobeRow(row);
       for(bit=0;bit<galinfo[gal].bits;bit++)
       {
      fuses[galinfo[gal].rows*bit+row]=ReceiveBit();
       }
    }
    // read UES
    StrobeRow(galinfo[gal].uesrow);
    for(bit=0;bit<64;bit++)
    {
       fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
    }
    // read CFG
    StrobeRow(galinfo[gal].cfgrow);
    for(bit=0;bit<82;bit++)
    {
       switch(pes[2])
       {
       case 0x00:
           fuses[cfg16V8[bit]]=ReceiveBit();
           break;
       case 0x1A:
           fuses[cfg16V8AB[bit]]=ReceiveBit();
           break;
       case 0x20:
           fuses[cfg20V8[bit]]=ReceiveBit();
           break;
       case 0x3A:
           fuses[cfg20V8AB[bit]]=ReceiveBit();
           break;
       }
    }
    case ATF16V8B:
#endif
    	fusesPtr.reset( new Gal::FuseArray( *max_element( 
    			std::begin( _gal->cfg() ), 
    			std::end( _gal->cfg() ) ) ) );

    	Gal::FuseArray& fuses = *fusesPtr;
        // read fuse rows ATF16V8
        for ( int row = 0; row < _gal->rows(); row++ )
        {
           StrobeRow( row );
           for( int bit = 0; bit < _gal->bits(); bit++ )
           {
               fuses[ ( _gal->rows() * bit ) + row ] = ReceiveBit();
           }
        }

        // read UES ATF16V8
        StrobeRow( _gal->uesrow() );
        for( int bit = 0; bit < 64; bit++ )
        {
            fuses[ _gal->uesfuse() + bit ] = ReceiveBit();
        }

        // read CFG ATF16V8
        StrobeRow( _gal->cfgrow() );

        for( int bit = 0; bit < 82; bit++ )
        {
            fuses[ _gal->cfg()[ bit ] ] = ReceiveBit();
        }

#ifdef NOT_FOR_NOW

        break;
    case GAL22V10:
    case ATF22V10B:
    case ATF22V10C:
        // read fuse rows
        for(row=0;row<galinfo[gal].rows;row++)
        {
           StrobeRow(row);
           for(bit=0;bit<galinfo[gal].bits;bit++)
           {
               fuses[galinfo[gal].rows*bit+row]=ReceiveBit();
           }
        Delay(1);
        }
        // read UES
        StrobeRow(galinfo[gal].uesrow);
        if (gal==GAL22V10)
        {
            for(bit=0;bit<64;bit++)
            {
                fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
            }
        }
        else // ATF22V10x
        {
            DiscardBits(68);
            for(bit=0;bit<64;bit++)
            {
                fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
            }
        }
        Delay(1);
        // read CFG
        SetRow(galinfo[gal].cfgrow);
        Strobe(1);
        for(bit=0;bit<galinfo[gal].cfgbits;bit++)
        {
            fuses[galinfo[gal].cfg[bit]]=ReceiveBit();
        }
    }
#endif

    TurnOff();

    return fusesPtr;
}

void 
Blaster::Strobe( int msec )
{
    delayMicroseconds( 2000 );
    _gal->SetSTB(0);
    //Delay(msec);
    delayMicroseconds( 2000 );
    _gal->SetSTB(1);
    delayMicroseconds( 2000 );
    //Delay(2);
}

void 
Blaster::WriteGal( const Gal::FuseArray& fuses )
{
    TurnOn();
    _gal->SetVPP( 1 );

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

            _gal->SetPV(1);

            // write fuse array
            for( int row=0; row < _gal->rows(); row++ )
            {
                _gal->SetRow( row );
                for( int bit = 0; bit < _gal->bits(); bit++ )
                {
                    SendBit( fuses[ ( _gal->rows() * bit ) + row ] );
                }
                Strobe( _gal->progtime() );
            }

            // write UES
            _gal->SetRow( _gal->uesrow() );
            for( int bit = 0; bit < 64; bit++ )
            {
                SendBit( fuses[ _gal->uesfuse() + bit ] );
            }
            Strobe( _gal->progtime() );

            // write CFG
            _gal->SetRow( _gal->cfgrow() );
            for( int bit = 0; bit < 82; bit++ )
            {
                SendBit( fuses[ _gal->cfg()[ bit ] ] );
            }
            Strobe( _gal->progtime() );
            
            _gal->SetPV(0);

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
Blaster::ReadPES( Gal::PesArray& pes )
{
    TurnOn();

    StrobeRow( _gal->pesrow() );
    delayMicroseconds( 500 );    
    for ( int byte = 0; byte < _gal->pesbytes(); byte++ )
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
	Gal::PesArray pes;

    ReadPES( pes );
    if ( _gal->VerifyPesType( pes ) )
    {
    	return _gal->ParsePes( pes );
    }

    return false;
}




