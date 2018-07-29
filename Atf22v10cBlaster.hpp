#pragma once

#include <string>
#include "Blaster.hpp"

class Atf22v10cBlaster :
public Blaster 
{
public:
	Atf22v10cBlaster();
	virtual ~Atf22v10cBlaster();

	// GalInfo
    const std::string& name() const;

	unsigned char id0() const { return 0x00; } // variant 1, variant 2 (eg. 16V8=0x00, 16V8A+=0x1A)
	unsigned char id1() const { return 0x00; }

	int fuses() const       { return 5892; }
	int pins() const        { return 24; }
	int rows() const        { return 44; }
	int bits() const        { return 132; }
	int uesrow() const      { return 44; }
	int uesfuse() const     { return 5828; }
	int uesbytes() const    { return 8; }
	int eraserow() const    { return 61; }
	int eraseallrow() const { return 60; }
	int pesrow() const      { return 58; }
	int pesbytes() const    { return 10; }
	int cfgrow() const      { return 16; }

	const std::vector< int >& cfg() const;
	int cfgbits() const;

    int progtime() const { return 10; }
    int erasetime() const { return 100; }
    int vpp() const { return 48; } // 12.0V


    bool VerifyPesType( PesArray& pes );
    bool ParsePes( const PesArray& pes );
  	void ReadFuses( FuseArray& fuses );
   	void WriteFuses( const FuseArray& fuses );  
};