//----------------------------------------------------------------------------
/*! \file
 *  \brief  pool for logger objects
 *  \author Michael Eichenlaub
 *  \date   27.10.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARAPPL_LOGGERPOOL_H
#define GUARD_LIBBASARAPPL_LOGGERPOOL_H

//----------------------------------------------------------------------------

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <log4cplus/logger.h>
#ifdef __GNUC__
#pragma GCC diagnostic push
#endif

//----------------------------------------------------------------------------
namespace basar		{
namespace appl	{

//---------------------------------------------------------------------------
//! class for initialized logger objects\n final class
class LoggerPool
{
public:	
	static log4cplus::Logger & eventManager();	//!< logger for event manager
};

//----------------------------------------------------------------------------
}	// namespace appl
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
