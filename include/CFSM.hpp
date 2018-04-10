#ifndef _CFSM_HPP_
#define _CFSM_HPP_

/* -------------------------------------------------------------------
 * Includes                                                          *
 * -------------------------------------------------------------------
*/

#include <stdlib.h>
#include <stdarg.h>

#include <assert.h>

#include <list>

#include "PortableTypes.h"

/* -------------------------------------------------------------------
 * Implementation                                                    *
 * -------------------------------------------------------------------
*/

namespace yafsm {

template< typename T, typename A > class CAction {

public:

    // Constructors, destructor
    CAction( )
    {
    };

    CAction( const CAction & action )
    {
    };

    virtual ~CAction( )
    {
    };

    // More
    virtual void trigger( T * instance, A * args )
    {
    };

};

template< typename T, typename A > class CActions {

    std::list< CAction< T, A > * >	m_actions;

public:

    // Constructors, destructor
    CActions( )
    {
    };

#if 0
    CActions( const CActions & actions )
    {
	*this = actions;
    };
#endif

    CActions( uint32 n, ... )
    {
	va_list vl;

	va_start( vl, n );

	for ( uint32 i = 0; i < n; ++i ) {
	    m_actions.push_back( ( CAction< T, A > * ) va_arg( vl, void * ) );
	}

	va_end( vl );
    };

#if 0
    CActions & operator=( const CActions & actions )
    {
	for ( std::list< CAction * >::const_iterator i = actions.m_actions.begin( ); i != actions.m_actions.end( ); ++i ) {
	    m_actions.push_back( new CAction( *(*i) ) );
	}
    };
#endif

    ~CActions( )
    {
	while ( ! m_actions.empty( ) ) {
	    delete m_actions.front( );
	    m_actions.pop_front( );
	}
    };

    // More
    void trigger( T * instance, A * args )
    {
	for ( typename std::list< CAction< T, A > * >::iterator i = m_actions.begin( ); i != m_actions.end( ); ++i ) {
	    (*i)->trigger( instance, args );
	}
    };

};

template< typename T, typename A > class CEvent {

    CActions< T, A > *			m_actions;

public:

    // Constructors, destructor
    CEvent( ) :
	m_actions( NULL )
    {
    };

    ~CEvent( )
    {
	delete m_actions;
    };

    // More
    void set_event( CActions< T, A > * a )
    {
	m_actions = a;
    };

    // More
    void trigger( T * instance, A * args )
    {
	if ( m_actions != NULL ) {
	    m_actions->trigger( instance, args );
	}
    };

};

template< typename T, typename A > class CState {

    CActions< T, A > *			m_enter;
    CActions< T, A > *			m_leave;

public:

    // Constructors, destructor
    CState( ) :
	m_enter( NULL ),
	m_leave( NULL )
    {
    };

    ~CState( )
    {
	delete m_enter;
	delete m_leave;
    };

    // More
    void set_enter( CActions< T, A > * e )
    {
	if ( m_enter != NULL ) {
	    delete m_enter;
	}
	m_enter = e;
    };

    void set_leave( CActions< T, A > * l )
    {
	if ( m_leave != NULL ) {
	    delete m_leave;
	}
	m_leave = l;
    };

    // More
    void enter( T * instance, A * args )
    {
	if ( m_enter != NULL ) {
	    m_enter->trigger( instance, args );
	}
    };

    void leave( T * instance, A * args )
    {
	if ( m_leave != NULL ) {
	    m_leave->trigger( instance, args );
	}
    };

};

template< typename T, typename A > class CTransition {

    uint32				m_state;
    CActions< T, A > *			m_transit;

public:

    // Constructors, destructor
    CTransition( ) :
	m_transit( NULL )
    {
    };

    ~CTransition( )
    {
	delete m_transit;
    };

    // More
    void set_transit( uint32 s, CActions< T, A > * t )
    {
	m_state = s;

	if ( m_transit != NULL ) {
	    delete m_transit;
	}
	m_transit = t;
    };

    // More
    uint32 transit( T * instance, A * args )
    {
	if ( m_transit != NULL ) {
	    m_transit->trigger( instance, args );
	}

	return m_state;
    };

};

template< uint32 NSTATES, uint32 NINPUTS, typename T, typename A > class CStateMachine {

protected:

    // states table
    CState< T, A >			m_states[ NSTATES ];

    // events table
    CEvent< T, A >			m_events[ NINPUTS ];

    // transition table
    CTransition< T, A >			m_transitions[ NSTATES ][ NINPUTS ];

    // current state
    uint32				m_current;

    // initial state
    uint32				m_initial;

public:

    // Constructors, destructor
    CStateMachine( uint32 init, uint32 dflt ) : m_current( init ), m_initial( init )
    {
	trans_reset( dflt );
    };

    virtual ~CStateMachine( ) { };

    // Specify events
    bool event( uint32 i, CActions< T, A > * a )
    {
	if ( i < NINPUTS ) {
	    m_events[ i ].set_event( a );
	}
	else {
	    assert( 0 );
	}

	return ( i < NINPUTS );
    };

    // Specify states
    bool state( uint32 s, CActions< T, A > * e, CActions< T, A > * l )
    {
	if ( s < NSTATES ) {
	    m_states[ s ].set_enter( e );
	    m_states[ s ].set_leave( l );
	}
	else {
	    assert( 0 );
	}

	return ( s < NSTATES );
    };

    // Reset transitions
    bool trans_reset( uint32 dflt )
    {
	if ( dflt < NSTATES ) {
	    for ( uint32 s = 0; s < NSTATES; s++ ) {
		for ( uint32 i = 0; i < NINPUTS; i++ ) {
		    m_transitions[ s ][ i ].set_transit( dflt, NULL );
		}
	    }
	}
	else {
	    assert( 0 );
	}

	return ( dflt < NSTATES );
    };

    // Specify transitions
    bool trans(
	    uint32		s1,
	    uint32		inp,
	    uint32		s2
	    )
    {
	if ( ( s1 < NSTATES ) && ( inp < NINPUTS ) && ( s2 < NSTATES ) ) {
	    m_transitions[ s1 ][ inp ].set_transit( s2, NULL );
	}
	else {
	    assert( 0 );
	}

	return ( ( s1 < NSTATES ) && ( inp < NINPUTS ) && ( s2 < NSTATES ) );
    };

    bool trans(
	    uint32		s1,
	    uint32		inp,
	    uint32		s2,
	    CActions< T, A > *	a
	    )
    {
	if ( ( s1 < NSTATES ) && ( inp < NINPUTS ) && ( s2 < NSTATES ) ) {
	    m_transitions[ s1 ][ inp ].set_transit( s2, a );
	}
	else {
	    assert( 0 );
	}

	return ( ( s1 < NSTATES ) && ( inp < NINPUTS ) && ( s2 < NSTATES ) );
    };

    // Examine the machine
    uint32 nstates( ) const
    {
	return NSTATES;
    };

    uint32 state( ) const
    {
	return m_current;
    };

    uint32 initial_state( ) const
    {
	return m_initial;
    };

    uint32 target( uint32 state, uint32 inp ) const
    {
	assert( state < NSTATES );
	assert( inp < NINPUTS );

#if 0
	return transitions[state][inp].s2;
#else
#endif
    };

    // Change the state of the machine
    uint32 fire( uint32 inp, T * instance, A * args )
    {
	if ( ( m_current < NSTATES ) && ( inp < NINPUTS ) ) {
	    m_events[ inp ].trigger( instance, args );
	    m_states[ m_current ].leave( instance, args );
	    m_current = m_transitions[ m_current ][ inp ].transit( instance, args );
	    m_states[ m_current ].enter( instance, args );
	}
	else {
	    assert( 0 );
	}

	assert( m_current < NSTATES );

	return m_current;
    };

    void go( uint32 state )
    {
	assert( state < NSTATES );

	m_current = state;
    };

    void reset( )
    {
	m_current = m_initial;
    };

    void abort( );

};

};

#endif
