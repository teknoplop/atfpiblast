#pragma once

#include <ostream>

#include "Blaster.hpp"
#include "Gal.hpp"

class Jedec
{
public:
	static void Format( const std::shared_ptr< Gal >& gal, 
		const std::shared_ptr< Gal::FuseArray >& fuses,
		std::ostream& ostr );

	static int Parse( const std::string& jedecText, std::shared_ptr< Gal::FuseArray >& fuses );

	// TODO: identify GAL type from the JEDEC file?
	static std::shared_ptr< Gal::FuseArray > Load( const std::string& filename, const std::shared_ptr< Gal >& gal );

protected: 
	static unsigned short CheckSum(int n, const Gal::FuseArray& fusemap);

};