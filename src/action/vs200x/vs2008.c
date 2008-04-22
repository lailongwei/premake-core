/**
 * \file   vs2008.c
 * \brief  Visual Studio 2008 project file generation action.
 * \author Copyright (c) 2002-2008 Jason Perkins and the Premake project
 */

#include <stdlib.h>
#include "premake.h"
#include "action/action.h"
#include "vs200x_solution.h"


/** The VS2008 solution writing process, for session_enumerate_objects() */
static SessionSolutionCallback SolutionCallbacks[] = 
{
	vs2002_solution_create,
	vs2008_solution_signature,
	vs2002_solution_projects,
	vs2005_solution_platforms,
	vs2005_solution_project_platforms,
	vs2005_solution_properties,
	NULL
};

/** The VS2008 project writing process, for session_enumerate_objects() */
static SessionProjectCallback ProjectCallbacks[] =
{
	NULL
};


/**
 * The Visual Studio 2008 action handler.
 * \param   sess   The active session object.
 * \returns OKAY if successful.
 */
int vs2008_action(Session sess)
{
	stream_writeline(Console, "Generating project files for Visual Studio 2008...");
	return session_enumerate_objects(sess, SolutionCallbacks, ProjectCallbacks);
}
