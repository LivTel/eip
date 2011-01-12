/* eip_address.c
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/c/eip_address.c,v 1.3 2011-01-12 14:07:55 cjm Exp $
*/
/**
 * Routines for parsing PLC addresses of the form N7:0/1 into it's constituent parts.
 * @author Chris Mottram
 * @version $Revision: 1.3 $
 */
/**
 * This hash define is needed before including source files give us POSIX.4/IEEE1003.1b-1993 prototypes.
 */
#define _POSIX_SOURCE 1
/**
 * This hash define is needed before including source files give us POSIX.4/IEEE1003.1b-1993 prototypes.
 */
#define _POSIX_C_SOURCE 199309L

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> /* strncasecmp */
#include "log_udp.h"
#include "eip_address.h"
#include "eip_general.h"

/* internal variables */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: eip_address.c,v 1.3 2011-01-12 14:07:55 cjm Exp $";

/* -------------------------------------------------------------
** external functions 
** ------------------------------------------------------------- */
/**
 * Routine to take a string representation of a PLC address and turn it into an instance of EIP_PLC_Memory_Address_T.
 * @param class The class parameter for logging.
 * @param source The source parameter for logging.
 * @param straddress The string version of the address.
 * @param address The address (memory location) of an instance of EIP_PLC_Memory_Address_T to fill in with
 *                 the parsed PLC address.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_PLC_Memory_Address_T
 */
int EIP_Address_Parse(char *class,char *source,char *straddress,EIP_PLC_Memory_Address_T *address)
{
	int x,l;

	if(straddress == NULL)
	{
		EIP_Error_Number = 300;
		sprintf(EIP_Error_String,"EIP_Address_Parse:String address was NULL.");
		return FALSE;
	}
#if LOGGING > 5
	EIP_Log_Format(class,source,LOG_VERBOSITY_VERY_VERBOSE,"EIP_Address_Parse(address=%s): Started.",straddress);
#endif /* LOGGING */
	if(address == NULL)
	{
		EIP_Error_Number = 301;
		sprintf(EIP_Error_String,"EIP_Address_Parse:Address was NULL.");
		return FALSE;
	}
	memset(address,0,sizeof(*address));
	address->Size=0;
	address->File_Number=0;
	address->File_Type=0;
	address->Element_Number=0;
	address->Sub_Element_Number=0;
	for (x=0;x<strlen(straddress);x++)
	{
		switch (straddress[x])
		{
			case 'O':
				address->File_Letter = straddress[x];
				address->File_Type = 0x8b;
				address->File_Number = 0;
				address->Size = 2;
				break;
			case 'I':
				address->File_Letter = straddress[x];
				address->File_Type = 0x8c;
				address->File_Number = 1;
				address->Size = 2;
				break;
			case 'S':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x84;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 2;
				break;
			case 'B':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x85;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 2;
				break;
			case 'T':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x86;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 2;
				break;
			case 'C':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x87;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 2;
				break;
			case 'R':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x88;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 2;
				break;
			case 'N':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x89;
				address->File_Number = atoi(&straddress[x]);
				address->Size=2;
				break;
			case 'F':
				address->File_Letter = straddress[x];
				x++;
				address->File_Type = 0x8a;
				address->File_Number = atoi(&straddress[x]);
				address->Size = 4;
				break;
			case ':':
				address->Element_Number = atoi(&straddress[++x]);
				break;
			case '.':
			case '/':
				x++;
				if (isdigit(straddress[x]))
				{
					address->Sub_Element_Number = atoi(&straddress[x]);
				}
				l=strlen(straddress) - x;
				if (strncasecmp (&straddress[x],"acc",l) == 0 )
					address->Sub_Element_Number = 2;
				if (strncasecmp (&straddress[x],"pre",l) == 0 )
					address->Sub_Element_Number = 1;
				if (strncasecmp (&straddress[x],"len",l) == 0 )
					address->Sub_Element_Number = 1;
				if (strncasecmp (&straddress[x],"pos",l) == 0 )
					address->Sub_Element_Number = 2;
				if (strncasecmp (&straddress[x],"en",l) == 0 )
					address->Sub_Element_Number = 13;
				if (strncasecmp (&straddress[x],"tt",l) == 0 )
					address->Sub_Element_Number = 14;
				if (strncasecmp (&straddress[x],"dn",l) == 0 )
					address->Sub_Element_Number = 15;				
				x = strlen(straddress)-1;
		}/* end switch */
	}/* end for */
#if LOGGING > 5
	EIP_Log_Format(class,source,LOG_VERBOSITY_VERY_VERBOSE,
		       "EIP_Address_Parse: %s = Size = %d,File_Letter = %c,File_Number = %d,"
		       "File_Type = %#x, Element_Number = %d,Sub_Element_Number = %d.",straddress,address->Size,
		       address->File_Letter,address->File_Number,address->File_Type,address->Element_Number,
		       address->Sub_Element_Number);
#endif /* LOGGING */
#if LOGGING > 5
	EIP_Log(class,source,LOG_VERBOSITY_VERY_VERBOSE,"EIP_Address_Parse: Finished.");
#endif /* LOGGING */
	return TRUE;
}

/**
 * Routine to take a EIP_PLC_Memory_Address_T structure and return the string representation of a PLC address.
 * @param class The class parameter for logging.
 * @param source The source parameter for logging.
 * @param address An instance of EIP_PLC_Memory_Address_T.
 * @param straddress A string (of at least 16 bytes in length) to create a string representation of the address.
 * @param include_sub_element A boolean. If TRUE and the address.Sub_Element_Number is non-zero, add it to string
 *        otherwise don't.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_PLC_Memory_Address_T
 */
int EIP_Address_Create(char *class,char *source,EIP_PLC_Memory_Address_T address,char *straddress,
		       int include_sub_element)
{
#if LOGGING > 5
	EIP_Log_Format(class,source,LOG_VERBOSITY_VERY_VERBOSE,"EIP_Address_Create Started.");
#endif /* LOGGING */
#if LOGGING > 5
	EIP_Log_Format(class,source,LOG_VERBOSITY_VERY_VERBOSE,
		       "EIP_Address_Create: Size = %d,File_Letter = %c,File_Number = %d,"
		       "File_Type = %#x, Element_Number = %d,Sub_Element_Number = %d.",address.Size,
		       address.File_Letter,address.File_Number,address.File_Type,address.Element_Number,
		       address.Sub_Element_Number);
#endif /* LOGGING */
	if(straddress == NULL)
	{
		EIP_Error_Number = 302;
		sprintf(EIP_Error_String,"EIP_Address_Create:String address was NULL.");
		return FALSE;
	}
	if(EIP_IS_BOOLEAN(include_sub_element) == FALSE)
	{
		EIP_Error_Number = 303;
		sprintf(EIP_Error_String,"EIP_Address_Create:include_sub_element was not a boolean(%d).",
			include_sub_element);
		return FALSE;
	}
	/* initialise straddress */
	straddress[0] = '\0';
	/* straddress = "N\0" */
	sprintf(straddress+strlen(straddress),"%c",address.File_Letter);
	/* straddress = "N7:1\0" */
	sprintf(straddress+strlen(straddress),"%d:%d",address.File_Number,address.Element_Number);
	if(include_sub_element&&(address.Sub_Element_Number > 0))
	{
		/* straddress = "N7:1/3\0" */
		sprintf(straddress+strlen(straddress),"/%d",address.Sub_Element_Number);
	}
#if LOGGING > 5
	EIP_Log_Format(class,source,LOG_VERBOSITY_VERY_VERBOSE,"EIP_Address_Create: Address String = '%s'.",
		       straddress);
#endif /* LOGGING */
#if LOGGING > 5
	EIP_Log(class,source,LOG_VERBOSITY_VERY_VERBOSE,"EIP_Address_Create: Finished.");
#endif /* LOGGING */
	return TRUE;
}

/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2009/02/05 11:36:18  cjm
** Swapped Bitwise for Absolute logging levels.
**
** Revision 1.1  2008/10/15 13:48:23  cjm
** Initial revision
**
*/
