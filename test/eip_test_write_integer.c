/* eip_test_write_integer.c
** $Header: /home/cjm/cvs/eip/test/eip_test_write_integer.c,v 1.1 2008-10-15 13:48:34 cjm Exp $
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "eip_general.h"
#include "eip_session.h"
#include "eip_write.h"
/**
 * This program tests writing an integer value from a PLC.
 * @author $Author: cjm $
 * @version $Revision: 1.1 $
 */

/* hash definitions */
/**
 * Default bit-wise log level.
 */
#define DEFAULT_LOG_LEVEL       (EIP_LOG_BIT_SESSION|EIP_LOG_BIT_WRITE)

/* internal variables */
/**
 * Revision control system identifier.
 */
static char rcsid[] = "$Id: eip_test_write_integer.c,v 1.1 2008-10-15 13:48:34 cjm Exp $";

/**
 * The hostname of the PLC.
 */
static char Hostname[256];
/**
 * The address of PLC to write the value to.
 */
static char PLC_Address[256];
/**
 * The integer value to write to the PLC.
 */
static int Value;

/* internal routines */
static int Parse_Arguments(int argc, char *argv[]);
static void Help(void);

/**
 * Main program.
 * @param argc The number of arguments to the program.
 * @param argv An array of argument strings.
 * @return This function returns 0 if the program succeeds, and a positive integer if it fails.
 * @see #Hostname
 * @see #PLC_Address
 * @see #Value
 */
int main(int argc, char *argv[])
{
	EIP_Handle_T *handle = NULL;

	fprintf(stdout,"Test Writing Integer values from PLC.\n");
	/* initialise logging */
	EIP_Set_Log_Handler_Function(EIP_Log_Handler_Stdout);
	EIP_Set_Log_Filter_Function(EIP_Log_Filter_Level_Bitwise);
	EIP_Set_Log_Filter_Level(DEFAULT_LOG_LEVEL);
	fprintf(stdout,"Parsing Arguments.\n");
	strcpy(Hostname,"");
	strcpy(PLC_Address,"");
	/* parse arguments */
	if(!Parse_Arguments(argc,argv))
		return 1;
	/* open interface */
	if(strlen(PLC_Address) == 0)
	{
		fprintf(stderr,"No PLC address specified.\n");
		return 2;
	}
	if(strlen(Hostname) == 0)
	{
		fprintf(stderr,"No hostname specified.\n");
		return 2;
	}
	/* open interface */
	if(!EIP_Session_Handle_Create(&handle))
	{
		EIP_General_Error();
		return 4;
	}
	/* open a session to a Micrologix 1100 backplane 1, slot 0 @ Hostname */
	if(!EIP_Session_Open(Hostname,1,0,PLC_TYPE_MICROLOGIX1100,handle))
	{
		EIP_General_Error();
		EIP_Session_Handle_Destroy(&handle);
		return 4;
	}
	/* write the integer */
	if(!EIP_Write_Integer(handle,PLC_Address,Value))
	{
		EIP_General_Error();
		EIP_Session_Handle_Destroy(&handle);
		return 4;
	}
	fprintf(stdout,"Wrote value %d to PLC Address %s.\n",Value,PLC_Address);
	/* close */
	if(!EIP_Session_Close(handle))
	{
		EIP_General_Error();
		EIP_Session_Handle_Destroy(&handle);
		return 4;
	}
	if(!EIP_Session_Handle_Destroy(&handle))
	{
		EIP_General_Error();
		return 4;
	}

	return 0;
}

/**
 * Routine to parse command line arguments.
 * @param argc The number of arguments sent to the program.
 * @param argv An array of argument strings.
 * @see #Help
 * @see #Hostname
 * @see #PLC_Address
 * @see #Value
 */
static int Parse_Arguments(int argc, char *argv[])
{
	int i,retval,ivalue;

	for(i=1;i<argc;i++)
	{
		if((strcmp(argv[i],"-address")==0)||(strcmp(argv[i],"-a")==0))
		{
			if((i+1)<argc)
			{
				strncpy(PLC_Address,argv[i+1],255);
				i++;
			}
			else
			{
				fprintf(stderr,"Test Write Integer:Parse_Arguments:"
					"Address requires an address.\n");
				return FALSE;
			}
		}
		else if((strcmp(argv[i],"-ip")==0)||(strcmp(argv[i],"-hostname")==0))
		{
			if((i+1)<argc)
			{
				strcpy(Hostname,argv[i+1]);
				i++;
			}
			else
			{
				fprintf(stderr,"Test Write Integer:Parse_Arguments:"
					"Hostname requires a name.\n");
				return FALSE;
			}
		}
		else if((strcmp(argv[i],"-log_level")==0)||(strcmp(argv[i],"-l")==0))
		{
			if((i+1)<argc)
			{
				retval = sscanf(argv[i+1],"%d",&ivalue);
				if(retval != 1)
				{
					fprintf(stderr,"Test Write Integer:Parse_Arguments:"
						"Illegal log level %s.\n",argv[i+1]);
					return FALSE;
				}
				EIP_Set_Log_Filter_Level(ivalue);
				i++;
			}
			else
			{
				fprintf(stderr,"Test Write Integer:Parse_Arguments:"
					"Log Level requires a number.\n");
				return FALSE;
			}
		}
		else if((strcmp(argv[i],"-value")==0)||(strcmp(argv[i],"-v")==0))
		{
			if((i+1)<argc)
			{
				retval = sscanf(argv[i+1],"%d",&Value);
				if(retval != 1)
				{
					fprintf(stderr,"Test Write Integer:Parse_Arguments:"
						"Illegal value %s.\n",argv[i+1]);
					return FALSE;
				}
				i++;
			}
			else
			{
				fprintf(stderr,"Test Write Integer:Parse_Arguments:"
					"-value requires a number.\n");
				return FALSE;
			}
		}
		else if(strcmp(argv[i],"-help")==0)
		{
			Help();
			exit(0);
		}
		else
		{
			fprintf(stderr,"Test Write Integer:Parse_Arguments:argument '%s' not recognized.\n",argv[i]);
			return FALSE;
		}			
	}
	return TRUE;
}

/**
 * Help routine.
 */
static void Help(void)
{
	fprintf(stdout,"Test Write Integer:Help.\n");
	fprintf(stdout,"Test Write Integer tries writing an integer value from a PLC.\n");
	fprintf(stdout,"eip_test_write_integer [-ip|-hostname <address>]\n");
	fprintf(stdout,"\t[-a[ddress] <string>][-v[alue] <integer>]\n");
	fprintf(stdout,"\t[-l[og_level] <number>][-help]\n");
	fprintf(stdout,"\n");
	fprintf(stdout,"\t-address specifies the integer PLC address to set, of the form N7:1.\n");
	fprintf(stdout,"\t-log_level specifies the logging. See eip_general.h for details.\n");
}

/*
** $Log: not supported by cvs2svn $
*/
