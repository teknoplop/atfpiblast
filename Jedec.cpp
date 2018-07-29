#include "Jedec.hpp"

#include <fstream> // To use ifstream
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>

#include <stdio.h>

std::shared_ptr< Blaster::FuseArray >
Jedec::Load( const std::string& filename, const std::shared_ptr< Blaster >& gal )
{

  auto fuses = gal->CreateFuseArray();

  std::cout << "fuseMap size: " << fuses->size() << std::endl;

  std::ifstream ifs( filename );

  if ( !ifs ) 
  {
    throw std::runtime_error("Cannot open Jedec file");
  }

  std::string jedecText;
  jedecText.assign( ( std::istreambuf_iterator<char>( ifs ) ), std::istreambuf_iterator<char>() );

  std::cout << "Parsing Jedec length: " << jedecText.length() << std::endl;


  int position = Parse( jedecText, fuses );

  std::cout << "Position: " << position << std::endl;

  if ( position != jedecText.length() )
  {
    throw std::runtime_error("Jedec file not parsed to end");
  }

  return fuses;

}

void 
Jedec::Format( const Blaster& gal, const Blaster::FuseArray& fuses, const Blaster::PesArray& pes, std::ostream& ostr )
{
    int i,j,k,n;
    bool unused,start;
    time_t now;
    unsigned char ch;
    char buffer[16384];

    time( &now );
    n = sprintf( buffer, "JEDEC file for %s created on %s", gal.name().c_str(), asctime( localtime( &now ) ) ) - 1;
    n += sprintf( buffer + n, "\r\n*QP%d*QF%d*QV0*F0*G0*X0*\r\n", gal.pins(), gal.fuses()) ;
    for( i = k = 0; i < gal.bits(); i++ )
    {
        start = n;
        unused = true;
        n += sprintf( buffer + n, "L%04d ", k );
        for( j = 0; j < gal.rows(); j++ )
        {
            if ( fuses[ k ] ) 
            {
              unused = false;
            }

            buffer[ n++ ] = '0' + fuses[ k++ ];
        }
        n += sprintf( buffer+n, "*\r\n" );
        if( unused ) 
        {
          n = start;
        }
    }
    if( k < gal.uesfuse() )
    {
        start = n;
        unused = true;
        n += sprintf( buffer + n, "L%04d ", k );
        while( k < gal.uesfuse() )
        {
             if ( fuses[ k ] )
             {
               unused = false;
             }
             buffer[ n++ ] = '0' + fuses[ k++ ];
        }
        n += sprintf( buffer + n, "*\r\n");
        if ( unused ) 
        {
          n=start;
        }
    }
    start = n;
    unused = true;
    n += sprintf( buffer + n, "N UES" );
    for( j = 0; j < gal.uesbytes(); j++ )
    {
        ch = 0 ;
        for( i = 0; i < 8; i++ )
        {
            if( fuses[ k + 8 * j + i ] )
            {
                // TODO: do this function in the concrete class, or return endianess
                if (gal.name() == "ATF22V10C")
                {
                    ch |= 1 << ( 7 - i );  // big-endian
                }
                else
                {
                    ch |= 1 << i;     // little-endian
                }
            }
        }
        n += sprintf( buffer + n, " %02X", ch );
    }
    n += sprintf( buffer + n, "*\r\nL%04d ", k );
    for ( j = 0; j < 8 * gal.uesbytes(); j++ )
    {
        if ( fuses[ k ] )
        {
          unused = false;
        }
        buffer[ n++ ] = '0' + fuses[ k++ ];
    }
    n += sprintf( buffer + n, "*\r\n" );
    if ( unused )
    {
      n = start;
    }
    if( k < gal.fuses() )
    {
       start = n;
       unused = true;
       n += sprintf( buffer + n, "L%04d ", k );
       while( k < gal.fuses() )
       {
            if ( fuses[ k ] ) 
            {
              unused = false;
            }
            buffer[ n++ ] = '0' + fuses[ k++ ];
       }
       n += sprintf( buffer + n, "*\r\n" );
       if ( unused )
       {
         n = start;
       }
    }
    n += sprintf( buffer + n,"N PES" );
    for ( i = 0 ;i < gal.pesbytes(); i++ )
    {
        n += sprintf( buffer + n, " %02X", pes[ i ] );
    }
    n += sprintf( buffer + n, "*\r\nC%04X\r\n*", CheckSum( fuses, gal.fuses() ) );
    buffer[ n ] = '\0';

    ostr << buffer;

}


// TODO: exception instead of return n
int
Jedec::Parse( const std::string& ptr, std::shared_ptr< Blaster::FuseArray >& fusemapptr )
{
	int security = 0;
	unsigned short checksum;

    int i,n,type,checksumpos,address,pins;
    int state; // 0=outside JEDEC, 1=skipping comment or unknown, 2=read command

    unsigned short lastfuse;


    state=0;
    security=0;
    checksum=0;
    checksumpos=0;
    pins=0;
    lastfuse=0;

    Blaster::FuseArray& fusemap = *fusemapptr;

    for(n=0;ptr[n];n++)
    {
    	std::cout << ptr[n] << std::flush;

   if(ptr[n]=='*') state=2; // else ignored
   else switch(state)
   {
   case 2:
       if(!isspace(ptr[n])) switch(ptr[n])
       {
       case 'L':
          address=0;
          state=3;
          break;
       case 'F':
          state=5;
          break;
       case 'G':
          state=13;
          break;
       case 'Q':
          state=7;
          break;
       case 'C':
          checksumpos=n;
          state=14;
          break;
       default:
          state=1;
       }
       break;
   case 3:
       if(!isdigit(ptr[n])) return n;
       address=ptr[n]-'0';
       state=4;
       break;
   case 4:
       if(isspace(ptr[n]))
       {
           state=6;
       }
       else if(isdigit(ptr[n]))
       {
           address=10*address+(ptr[n]-'0');
       }
       else
       {
           return n;
       }
       break;
   case 5:
       if(isspace(ptr[n])) break; // ignored
       if(ptr[n]=='0'||ptr[n]=='1')
       {
       		std::fill( fusemap.begin(), fusemap.end(), ptr[n]-'0' );
       }
       else
       {
           return n;
       }
       state=1;
       break;
   case 6:
       if(isspace(ptr[n])) break; // ignored
       if(ptr[n]=='0'||ptr[n]=='1')
       {
           fusemap[address++]=ptr[n]-'0';
       }
       else
       {
           return n;
       }
       break;
   case 7:
       if(isspace(ptr[n])) break; // ignored
       if(ptr[n]=='P')
       {
           pins=0;
           state=8;
       }
       else if(ptr[n]=='F')
       {
           lastfuse=0;
           state=9;
       }
       else state=2;
       break;
   case 8:
       if(isspace(ptr[n])) break; // ignored
       if(!isdigit(ptr[n])) return n;
       pins=ptr[n]-'0';
            state=10;
       break;
   case 9:
       if(isspace(ptr[n])) break; // ignored
            if(!isdigit(ptr[n])) return n;
       lastfuse=ptr[n]-'0';
       state=11;
            break;
        case 10:
            if(isdigit(ptr[n]))
            {
                pins=10*pins+(ptr[n]-'0');
            }
            else if(isspace(ptr[n]))
            {
                state=12;
            }
            else return n;
            break;
   case 11:
            if(isdigit(ptr[n]))
            {
                lastfuse=10*lastfuse+(ptr[n]-'0');
            }
            else if(isspace(ptr[n]))
            {
                state=12;
            }
            else return n;
            break;
   case 12:
            if(!isspace(ptr[n])) return n;
       break;
   case 13:
       if(isspace(ptr[n])) break; // ignored
       if(ptr[n]=='0'||ptr[n]=='1')
       {
           security=ptr[n]-'0';
       }
       else
       {
           return n;
       }
       state=1;
       break;
   case 14:
       if(isspace(ptr[n])) break; // ignored
       if(isdigit(ptr[n]))
       {
           checksum=ptr[n]-'0';
       }
       else if(toupper(ptr[n])>='A'&&toupper(ptr[n])<='F')
       {
           checksum=toupper(ptr[n])-'A'+10;
       }
       else return n;
       state=15;
       break;
   case 15:
       if(isdigit(ptr[n]))
       {
           checksum=16*checksum+ptr[n]-'0';
       }
       else if(toupper(ptr[n])>='A'&&toupper(ptr[n])<='F')
       {
           checksum=16*checksum+toupper(ptr[n])-'A'+10;
       }
       else if(isspace(ptr[n]))
       {
           state=2;
       }
       else return n;
       break;
    }
    }

    std::cout << std::endl << std::endl;

    if(lastfuse||pins)
    {
    	std::cout << "lastfuse: " << lastfuse << std::endl; 


        if(checksum)
       	{
       		auto ck = CheckSum( fusemap, lastfuse );

	        if ( checksum!=ck)
	        {
				std::cout << "checksum: " 
					<< checksum 
					<< " != "
					<< ck
					<< std::endl;

	        	throw std::runtime_error( "Invalid checksum" );
	/*
	            if(Message(wnd,"Checksum given %04X calculated %04X",NULL,MB_OKCANCEL,checksum,CheckSum(lastfuse))==IDCANCEL)
	            {
	                return checksumpos;
	            }

	            */
	        }
        }

/* TODO: don't guess the type, but do check it matches...

        for(type=0,i=1;i<sizeof(galinfo)/sizeof(galinfo[0]);i++)
        {
            if((lastfuse==0||galinfo[i].fuses==lastfuse||galinfo[i].uesfuse==lastfuse&&galinfo[i].uesfuse+8*galinfo[i].uesbytes==galinfo[i].fuses)
            && (pins==0||galinfo[i].pins==pins||galinfo[i].pins==24&&pins==28))
            {
                if(gal==0)
                {
                     type=i;
                     break;
                }
                else if(!type)
                {
                     type=i;
                }
            }
        }
*/        
    }
    return n;
}



unsigned short 
Jedec::CheckSum( const Blaster::FuseArray& fusemap, int n )
{
    unsigned short c,e;
    long a;
    int i;

    c=e=0;
    a=0;
    for(i=0;i<n;i++)
    {
        e++;
        if(e==9)
    {
        e=1;
        a+=c;
        c=0;
    }
        c>>=1;
    if(fusemap[i]) c+=0x80;
    }
    return (unsigned short)((c>>(8-e))+a);
}

