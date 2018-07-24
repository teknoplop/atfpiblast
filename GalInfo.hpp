#pragma once

class GalInfo
{
public:
    virtual ~GalInfo() { }

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
	virtual int progtime() const = 0;
};
