/* eip_test_address_parse.c
** $Header: /home/cjm/cvs/eip/test/eip_test_address_parse.c,v 1.2 2009-02-05 11:36:45 cjm Exp $
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "log_udp.h"
#include "eip_address.h"
#include "eip_general.h"

/**
 * This program tests parsing PLC addresses of the form 'N7:0/1'.
 * @author $Author: cjm $
 * @version $Revision: 1.2 $
 */

/**
 * Main program.
 * @param argc The number of arguments to the program.
 * @param argv An array of argument strings.
 * @return This function returns 0 if the program succeeds, and a positive boolean if it fails.
 */
int main(int argc, char *argv[])
{
	char *address = NULL;
	EIP_PLC_Memory_Address_T plc_memory_address;
	int retval;
	char buff[256];

	if(argc != 2)
	{
		fprintf(stderr,"eip_test_address_parse <address>.\n");
		fprintf(stderr,"<address> is a PLC address of the form 'N7:0/1'.\n");
		return 1;
	}
	address = argv[1];
	EIP_Set_Log_Handler_Function(EIP_Log_Handler_Stdout);
	EIP_Set_Log_Filter_Function(EIP_Log_Filter_Level_Absolute);
	EIP_Set_Log_Filter_Level(LOG_VERBOSITY_VERY_VERBOSE);
	if(!EIP_Address_Parse(address,&plc_memory_address))
	{
		EIP_General_Error();
		return 1;
	}
	fprintf(stdout,"EIP_Address_Parse returned Size = %d,File_Letter = %c,File_Number = %d,"
		"File_Type = %#x, Element_Number = %d,Sub_Element_Number = %d.\n",plc_memory_address.Size,
		plc_memory_address.File_Letter,plc_memory_address.File_Number,plc_memory_address.File_Type,
		plc_memory_address.Element_Number,plc_memory_address.Sub_Element_Number);
	if(!EIP_Address_Create(plc_memory_address,buff,TRUE))
	{
		EIP_General_Error();
		return 2;
	}
	fprintf(stdout,"EIP_Address_Create(include_sub_element=TRUE) returned '%s'.\n",buff);
	if(!EIP_Address_Create(plc_memory_address,buff,FALSE))
	{
		EIP_General_Error();
		return 2;
	}
	fprintf(stdout,"EIP_Address_Create(include_sub_element=FALSE) returned '%s'.\n",buff);
	return 0;
}
/*
** $Log: not supported by cvs2svn $
** Revision 1.1  2008/10/15 13:48:34  cjm
** Initial revision
**
*/
