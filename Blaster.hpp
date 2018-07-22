#pragma once

#include <memory>
#include <vector>
#include <array>

#include "Gal.hpp"

class Blaster
{
public: 

  Blaster( std::shared_ptr< Gal >& gal );

  std::shared_ptr< Gal::FuseArray > ReadGAL();
  //void WritePES( const std::vector< unsigned char >& pes );
  void WriteGal ( const Gal::FuseArray& fuses );
  void SendBit( int bit );
  bool TestProperGAL();
  void ReadPES( Gal::PesArray& pes );
private:

  void TurnOn();
  void TurnOff();
  bool ReceiveBit();
  void StrobeRow( int row );
  void Strobe( int msec );
  void SendAddress( int n, int row );
  
  std::shared_ptr< Gal > _gal;

};

