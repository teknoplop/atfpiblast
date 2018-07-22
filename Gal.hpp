#pragma once

#include <memory>
#include <vector>
#include <array>

class Gal
{
public:
    virtual ~Gal() { }

    // Types
    // typedef std::vector< unsigned char > FuseArray; // TODO: make array of GAL type, need to template this class
	// typedef std::array< unsigned char, 12 > PesArray;
  	using FuseArray = std::vector< unsigned char >;
  	using PesArray  = std::array< unsigned char, 12 >;


    // Gal Info
	virtual unsigned char id0() const = 0;
	virtual unsigned char id1() const = 0;

	virtual int fuses() const = 0;
	virtual int pins() const  = 0;
	virtual int rows() const  = 0;
	virtual int bits() const  = 0;
	virtual int uesrow() const  = 0;
	virtual int uesfuse() const  = 0;
	virtual int uesbytes() const = 0;
	virtual int eraserow() const = 0;
	virtual int eraseallrow() const = 0;
	virtual int pesrow() const = 0;
	virtual int pesbytes() const = 0;
	virtual int cfgrow() const = 0;
	virtual const std::vector< int >& cfg() const = 0;
	virtual int cfgbits() const = 0;

	// PES info
    virtual int progtime() const = 0;
    virtual int erasetime() const = 0;
    virtual int vpp() const = 0;

	// Gal programming
	virtual void SetSTB( int state ) = 0;
	virtual void SetVPP( int state ) = 0;
    virtual void SetVCC( int state ) = 0;
    virtual void SetPV( int state )  = 0;

    virtual void SetRow( int row )   = 0;
    virtual void SetSDIN( int state ) = 0;
    virtual void SetSCLK( int state ) = 0;

    virtual bool GetSDOUT() = 0;

    virtual bool VerifyPesType( PesArray& pes ) = 0;
    virtual bool ParsePes( const PesArray& pes ) = 0;

    virtual std::shared_ptr< Gal::FuseArray > CreateFuseArray() = 0;

};
