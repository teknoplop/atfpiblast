#pragma once

#include <ostream>

#include "Blaster.hpp"

class Jedec
{
public:
	static void Format( const Blaster& gal, 
		const Blaster::FuseArray& fuses,
		const Blaster::PesArray& pes,
		std::ostream& ostr );

	static int Parse( const std::string& jedecText, std::shared_ptr< Blaster::FuseArray >& fuses );

	// TODO: identify GAL type from the JEDEC file?
	static std::shared_ptr< Blaster::FuseArray > Load( const std::string& filename, const Blaster& gal );

protected: 
	static unsigned short CheckSum( const Blaster::FuseArray& fusemap, int n );

};