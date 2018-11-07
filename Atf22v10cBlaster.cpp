#include <iostream>

#include "Atf22v10cBlaster.hpp"



Atf22v10cBlaster::Atf22v10cBlaster()
{ 
}

Atf22v10cBlaster::~Atf22v10cBlaster()
{
}

void 
Atf22v10cBlaster::ReadFuses( FuseArray& fuses )
{
	// read fuse rows
    for( int row = 0; row < rows(); row++ )
    {
		StrobeRow( row ) ;
		for( int bit = 0; bit < bits(); bit++ )
		{
			fuses[ ( rows() * bit ) + row ]=ReceiveBit();
		}       	
    	Delay(1);
    }

	// read UES
	StrobeRow( uesrow() );
	DiscardBits(68);
	for( int bit=0; bit < (uesbytes() * 8 ); bit++ )
	{
		fuses[ uesfuse() + bit ] = ReceiveBit();
	}

    Delay(1);

    // read CFG
    SetRow( cfgrow() );
    Strobe(1);

    for( int bit = 0; bit < cfgbits(); bit++ )
    {
        fuses[ cfg()[ bit ] ] = ReceiveBit();
    }
}


static std::vector< int > cfg22V10 =
{
    5809,5808,
    5811,5810,
    5813,5812,
    5815,5814,
    5817,5816,
    5819,5818,
    5821,5820,
    5823,5822,
    5825,5824,
    5827,5826
};

const std::vector< int >& 
Atf22v10cBlaster::cfg() const
{
	return cfg22V10;
}

static std::string galName = "ATF22V10C";

const std::string& 
Atf22v10cBlaster::name() const
{

    return galName;
}


int
Atf22v10cBlaster::cfgbits() const
{
	return cfg22V10.size();
}


bool 
Atf22v10cBlaster::VerifyPesType( PesArray& pes )
{
    if ( pes[7] == 'F' && pes[6] == '2' && pes[5] == '2' && pes[4]=='V' && pes[3]=='1' && pes[2]=='0')
    {
       if (pes[1]=='B')
       {
           //type = ATF22V10B;
            return false;
       }
       else
       {
           return true;
       }
    }
    return false;
}

bool 
Atf22v10cBlaster::ParsePes( const PesArray& pes )
{
    return true;
}

 void 
 Atf22v10cBlaster::WriteFuses( const FuseArray& fuses )
 {

 	// disable power-down feature (JEDEC bit #5892)


	SetRow( 0 ); // RA0-5 low
	for( int row = 0; row < rows(); row++)
	{
		std::cout << "row: " << row << std::endl;
		for( int bit = 0; bit < bits(); bit++ )
		{
			SendBit( fuses[ ( rows() * bit ) + row ] );
		}
		SendAddress( 6, row );  // send address 6 bits
		SetPV( 1 );
		Strobe( progtime() );
		SetPV( 0 );
	}

	// write UES
	SendBits( 68, 1 );
	for( int bit = 0; bit < 64; bit++ )
	{
		SendBit( fuses[ uesfuse() + bit ] );
	}
	SendAddress( 6, uesrow() );
	SetPV( 1 );
	Strobe( progtime() );
	SetPV( 0 );

	// write CFG
	SetRow( cfgrow() );
	for( int bit = 0; bit < 19; bit++ )
	{
		SendBit( fuses[ cfg()[ bit ] ] ); // clock out bits 0-62
	}
	SetSDIN( fuses[ cfg()[19] ] ); // send bit 63
	SetPV( 1 );
	Strobe( progtime() );
	SetPV( 0 );

	SetRow( 0 );
	SendAddress( 6, 59 );
	SetPV( 1 );
	Strobe( progtime() );
	SetPV( 0 );
	
	return;

 }


