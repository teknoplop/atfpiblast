#pragma once

#include <memory>
#include <vector>
#include <array>

#include "GalIo.hpp"
#include "GalInfo.hpp"

class Blaster
: public GalInfo
, public GalIo
{
public: 

  // Types
  // typedef std::vector< unsigned char > FuseArray; // TODO: make array of GAL type, need to template this class
  // typedef std::array< unsigned char, 12 > PesArray;
  using FuseArray = std::vector< unsigned char >;
  using PesArray  = std::array< unsigned char, 12 >;

  Blaster();
  virtual ~Blaster() {}

  std::shared_ptr< FuseArray > ReadGAL();

  //void WritePES( const std::vector< unsigned char >& pes );
  void WriteGal ( const FuseArray& fuses );
  void SendBit( int bit );
  void SendBits( int n, int bit );
  bool TestProperGAL();
  void ReadPES( PesArray& pes );

  std::shared_ptr< Blaster::FuseArray > CreateFuseArray() const
  {
    return std::shared_ptr< Blaster::FuseArray > ( new FuseArray( fuses() ) );
  }

protected:
  void TurnOn();
  void TurnOff();
  bool ReceiveBit();
  void StrobeRow( int row );
  void Strobe( int msec );
  void SendAddress( int n, int row );
  void DiscardBits(int n);
  void Delay( int msec );

  virtual void ReadFuses( FuseArray& fuses ) = 0;
  virtual bool VerifyPesType( PesArray& pes ) = 0;
  virtual bool ParsePes( const PesArray& pes ) = 0;  
  virtual void WriteFuses( const FuseArray& fuses ) = 0;  
};

