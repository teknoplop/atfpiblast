#pragma once

class GalIo
{
public:
	virtual ~GalIo() {}

    virtual void SetSTB( int state ) = 0;
	virtual void SetVPP( int state ) = 0;
    virtual void SetVCC( int state ) = 0;
    virtual void SetPV( int state ) = 0;

    virtual void SetRow( int row ) = 0;
    virtual void SetSDIN( int state ) = 0;
    virtual void SetSCLK( int state ) = 0;

    virtual bool GetSDOUT() = 0;
};