/**
 * \file   path.c
 * \brief  Path handling.
 * \author Copyright (c) 2002-2008 Jason Perkins and the Premake project
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "premake.h"
#include "base/path.h"
#include "platform/platform.h"
#include "base/buffers.h"
#include "base/cstr.h"
#include "base/dir.h"


/**
 * Create an absolute path from a relative one.
 * \param   path      The relative path to convert.
 * \returns An absolute version of the relative path.
 */
char* path_absolute(const char* path)
{
	char* source;
	char* result;

	assert(path);

	/* normalize the target path */
	source = path_translate(path, "/");
	if (strlen(source) == 0)
		strcpy(source, ".");

	/* If the directory is already absolute I don't have to do anything */
	if (path_is_absolute(source))
		return source;

	/* start from the current location */
	result = dir_get_current();

	/* split up the supplied relative path and tackle it bit by bit */
	while (source)
	{
		char* end = strchr(source, '/');
		if (end)
			*end = 0;

		if (cstr_eq(source, ".."))
		{
			char* up = strrchr(result, '/');
			if (up)
				*up = 0;
		}
		else if (!cstr_eq(source, "."))
		{
			strcat(result, "/");
			strcat(result, source);
		}

		source = end ? end + 1 : NULL;
	}

	return result;
}


/**
 * Assemble a complete file path from its component parts.
 * \param   dir       The directory portion of the path.
 * \param   filename  The file name portion of the path.
 * \param   ext       The extension portion of the path.
 * \returns The assembled file path.
 */
char* path_assemble(const char* dir, const char* filename, const char* ext)
{
	char* buffer;

	assert(dir);
	assert(filename);
	assert(ext);
	
	buffer = path_join(dir, filename);
	if (ext)
	{
		strcat(buffer, ext);
	}
	return buffer;
}


/**
 * Retrieve the directory portion of a path.
 * \param   path  The path to split.
 * \returns The directory portion of the path. Returns an empty string ("") if
 *          the path does not contain any directory information.
 */
char* path_directory(const char* path)
{
	char* ptr;
	char* buffer;
	
	assert(path);

	buffer = buffers_next();
	strcpy(buffer, path);
	
	/* look for the last path separator */
	ptr = strrchr(buffer, '/');
	if (ptr)
		*ptr = '\0';
	else
		*buffer = '\0';

	return buffer;
}


/**
 * Determine is a path is absolute (rooted at base of filesystem).
 * \param   path      The path to check.
 * \returns True if the path is absolute.
 */
int path_is_absolute(const char* path)
{
	assert(path);

	if (path[0] == '/' || path[0] == '\\')
		return 1;
	if (path[1] == ':')
		return 1;
	return 0;
}


/**
 * Join two paths togethers.
 * \param   leading   The leading path.
 * \param   trailing  The trailing path.
 * \returns A unified path.
 * \note If the trailing path is absolute, that will be the return value.
 *       A join is only performed if the trailing path is relative.
 */
char* path_join(const char* leading, const char* trailing)
{
	char* buffer = buffers_next();
	
	if (!trailing)
	{
		strcpy(buffer, leading);
		return buffer;
	}

	if (!leading || path_is_absolute(trailing))
	{
		strcpy(buffer, trailing);
		return buffer;
	}

	if (leading)
	{
		strcat(buffer, leading);
	}

	if (strlen(buffer) > 0 && !cstr_ends_with(buffer, "/"))
	{
		strcat(buffer, "/");
	}
		
	strcat(buffer, trailing);
	return buffer;
}


/**
 * Replace all path separator characters in a path.
 * \param   path    The path to translate.
 * \param   sep     The desired separator, or NULL for the platform's native separator.
 * \returns The translated path.
 */
char* path_translate(const char* path, const char* sep)
{
	char* ptr;
	char* buffer;

	assert(path);

	buffer = buffers_next();
	if (sep == NULL)
	{
#if defined(PLATFORM_WINDOWS)
		sep = "\\";
#else
		sep = "/";
#endif
	}

	strcpy(buffer, path);
	for (ptr = buffer; *ptr; ++ptr)
	{
		if (*ptr == '/' || *ptr == '\\')
			*ptr = *sep;
	}

	return buffer;
}