#pragma once

#include <ostream>

#include "Blaster.hpp"

class Jedec
{
public:
	static void Format( const std::shared_ptr< Blaster >& gal, 
		const std::shared_ptr< Blaster::FuseArray >& fuses,
		std::ostream& ostr );

	static int Parse( const std::string& jedecText, std::shared_ptr< Blaster::FuseArray >& fuses );

	// TODO: identify GAL type from the JEDEC file?
	static std::shared_ptr< Blaster::FuseArray > Load( const std::string& filename, const std::shared_ptr< Blaster >& gal );

protected: 
	static unsigned short CheckSum(int n, const Blaster::FuseArray& fusemap);

};