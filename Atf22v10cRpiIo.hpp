#pragma once

#include "Atf22v10cBlaster.hpp"

// Abstact the RPI GPIO to GalBlast i/f
// comments on GalBlast Parallel port pin mappings

// https://github.com/garlick/pi-parport



class Atf22v10cRpiIo
: public Atf22v10cBlaster
{
public:

	Atf22v10cRpiIo();
	virtual ~Atf22v10cRpiIo();

	// GalIo
    void SetSTB( int state ); // STROBE/P1/CTRL0
	void SetVPP( int state ) { } // N/C (FEED/P14/CTRL1?)
    void SetVCC( int state ) { } // N/C  (INIT/P16/CTRL3?)
    void SetPV( int state ); // SEL/P17/CTRL4

    void SetRow( int row );  // RA0-5?
    void SetSDIN( int state ); // P2/DAT0 ?
    void SetSCLK( int state ); // P9/DAT7 ?

    bool GetSDOUT(); // ACK/P10/DAT4
/*



	unsigned char id0() const { return 0x00; } // variant 1, variant 2 (eg. 16V8=0x00, 16V8A+=0x1A)
	unsigned char id1() const { return 0x00; }

	int fuses() const { return 2194; }
	int pins() const  { return 20; }
	int rows() const  { return 32; }
	int bits() const  { return 64; }
	int uesrow() const  { return 32; }
	int uesfuse() const  { return 2056; }
	int uesbytes() const  { return 8; }
	int eraserow() const  { return 63; }
	int eraseallrow() const  { return 54; }
	int pesrow() const  { return 58; }
	int pesbytes() const  { return 8; }
	int cfgrow() const  { return 60; }
	const std::vector< int >& cfg() const;
	int cfgbits() const;

    int progtime() const { return 10; }
    int erasetime() const { return 100; }
    int vpp() const { return 48; } // 12.0V


    bool VerifyPesType( PesArray& pes );
    bool ParsePes( const PesArray& pes );
 */
    //std::shared_ptr< B::FuseArray > CreateFuseArray();
    static void Init();

};
