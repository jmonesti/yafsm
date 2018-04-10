#include <stdio.h>

#include "MyFSM.hpp"

class CWrapperArgs {
public:
    unsigned int v;
};

class CWrapper {
public:
    void print( const char * s, CWrapperArgs * a ) {
	::printf( "%s %d\n", s, a->v );
    };
};

void CBoing::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Boing", args );
};

void CPing2Pong::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Ping2Pong", args );
};

void CPong2Ping::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Pong2Ping", args );
};

void CEnteringPing::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Entering Ping", args );
};

void CLeavingPing::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Leaving Ping", args );
};

void CEnteringPong::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Entering Pong", args );
};

void CLeavingPong::trigger( CWrapper * instance, CWrapperArgs * args )
{
    instance->print( "Leaving Pong", args );
};

int main( int argc, char * argv[] )
{
    CWrapper		i;
    CWrapperArgs	v;
    MyFsm		fsm( &i );

    for ( v.v = 0; v.v < 10; ++v.v ) {
	fsm.fire( MyFsm::I_BOING, &v );
	::printf( "-\n" );
    }

    return 0;
};
