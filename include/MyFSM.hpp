#ifndef _MYFSM_HPP_
#define _MYFSM_HPP_

/* -------------------------------------------------------------------
 * Includes                                                          *
 * -------------------------------------------------------------------
*/

#include "PortableTypes.h"

#include "CFSM.hpp"

/* -------------------------------------------------------------------
 * Defines                                                           *
 * -------------------------------------------------------------------
*/

#define __NSTATES__		4
#define __NINPUTS__		3

/* -------------------------------------------------------------------
 * Implementation                                                    *
 * -------------------------------------------------------------------
*/

class CWrapper;
class CWrapperArgs;

class CBoing : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CPing2Pong : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CPong2Ping : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CEnteringPing : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CLeavingPing : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CEnteringPong : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class CLeavingPong : public yafsm::CAction< CWrapper, CWrapperArgs > {
    virtual void trigger( CWrapper * instance, CWrapperArgs * args );
};

class MyFsm : public yafsm::CStateMachine< __NSTATES__, __NINPUTS__, CWrapper, CWrapperArgs > {

public:

    // States - TAKE CARE of __NSTATES__ !!!
    enum {
	S_ERRONEOUS		= 0,

	S_PING			= 1,
	S_PONG			= 2
    };

    // Inputs - TAKE CARE of __NINPUTS__ !!!
    enum {
	I_BOING			= 0
    };

private:

    CWrapper *			m_instance;

public:
    // Constructors, destructor
    MyFsm( CWrapper * instance ) :
	yafsm::CStateMachine< __NSTATES__, __NINPUTS__, CWrapper, CWrapperArgs >( S_PING, S_ERRONEOUS ),
	m_instance( instance )
    {
        // -----------------------------------------------------------
	// transitions

	trans_reset( S_ERRONEOUS );

        // -----------------------------------------------------------
	// transitions

	trans( S_PING, I_BOING, S_PONG, new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CPing2Pong( ) ) );
	trans( S_PONG, I_BOING, S_PING, new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CPong2Ping( ) ) );

        // -----------------------------------------------------------
	// events

	event(
		I_BOING,
		new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CBoing( ) )
	);

        // -----------------------------------------------------------
	// states

	state(
		S_PING,
		new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CEnteringPing( ) ),
		new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CLeavingPing( ) )
	);
	state(
		S_PONG,
		new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CEnteringPong( ) ),
		new yafsm::CActions< CWrapper, CWrapperArgs >( 1, new CLeavingPong( ) )
	);
    };

    virtual ~MyFsm( ) { };

    uint32 fire( uint32 inp, CWrapperArgs * args )
    {
	return yafsm::CStateMachine< __NSTATES__, __NINPUTS__, CWrapper, CWrapperArgs >::fire( inp, m_instance, args );
    };

};

#endif
