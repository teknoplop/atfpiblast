#include <iostream>
#include <fstream>
#include <iomanip>
#include <iterator>

#include "Atf22v10cRpiIo.hpp"
#include "Jedec.hpp"

// http://www.armory.com/~rstevew/Public/Pgmrs/GAL/algo.htm

int 
main( int argc, char** argv )
{
	std::cout << "Raspberry Pi ATF16V8B Blaster" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl << std::endl;

		try 
		{
			Atf22v10cRpiIo b;
			b.Init();





// READ
			if ( argc == 1 )
			{
				Blaster::PesArray pes;
				b.ReadPES( pes );
/*
				std::cout << "PES: ";

				for ( int i = 0; i < b.pesbytes(); ++i )
				{
				  std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)pes[ i ] << " ";
			    }
				std::cout << std::endl;

				std::cout << "     ";
				for ( int i = 0; i < b.pesbytes(); ++i )
				{
				  std::cout << " " << ( std::isprint( pes [ i ] ) ? static_cast< char >( pes[ i ] ) : '.' ) << " ";
				}
*/			
				//std::copy(pes.begin(), pes.end(), std::ostream_iterator<int>(std::cout));
				std::cout << std::endl << std::endl;
					auto fuses = b.ReadGAL();
					Jedec::Format( b, *fuses, pes,  std::cout ); // read fuse rows, UES, cfg.
					std::cout <<  std::flush;
				}

/* Verify
        if(!CheckJEDEC(wnd)) return TRUE;
        if(!TestProperGAL(wnd)) return TRUE;
        ReadGAL(wnd,backup);
        if(memcmp(backup,fusemap,galinfo[gal].fuses)==0)
*/

// ERASE
// if(!TestProperGAL(wnd)) return TRUE;
// if(pes[1]&0x80) if(Message(wnd,"GAL is a MASTER, still erase?",NULL,MB_ICONSTOP|MB_YESNO)!=IDYES) return TRUE;
//    b.EraseGAL();

/* Write PES

    if(!GetSetup(wnd)) return FALSE;
        if(DialogBox(hInstance,MAKEINTRESOURCE(2),wnd,ProgDlgProc))
        {
            WritePES(wnd,pes);
        }
*/

/* Write
        if(GetDlgItemText(wnd,102,galbuffer,sizeof(galbuffer))<1)
        {
            MessageBox(wnd,"Load a JEDEC fuse map first",progname,MB_ICONEXCLAMATION|MB_OK);
            return 0L;
        }
        if(!CheckJEDEC(wnd)) return TRUE;
        if(!TestProperGAL(wnd)) return TRUE;*/


		if ( argc > 1 )
		{
			if ( std::string( argv[1] ) == "--erase" )
			{
			        b.Erase();
			}
			else
			{

			/*	if ( !b.TestProperGAL() )
				{
					return -1;
				} */
		        

				//  "/Users/sam/JSMOTOR.jed"
				auto fuseMap = Jedec::Load( argv[ 1 ], b );

				int linechar = 0;
				for (auto const& c : *fuseMap )
				{
	    			std::cout << (char)( '0' + c ) << ' ';
	    			if ( ++linechar == 44 )
	    			{
	    				linechar = 0;
	    				std::cout << std::endl;
	    			}
	    		}

				std::cout << std::endl;


		        b.WriteGal( *fuseMap );
	    	}
	    }

 /*       if(security)
        {
            if(Message(wnd,"Programming the security fuse will prohibit the readout and verification of the GAL. Do you want to continue ?",progname,MB_ICONEXCLAMATION|MB_YESNO)!=IDYES) return TRUE;
            BurnSecurity(wnd,gal);
        }
*/



    	}
    	catch( const std::runtime_error& ex )
    	{
   			std::cout
   				<< "Runtime error: "
   				<< ex.what() 
   				<< std::endl;
    	}


        return 0;
}


