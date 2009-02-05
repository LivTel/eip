/* eip_write.c
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/c/eip_write.c,v 1.3 2009-02-05 11:36:18 cjm Exp $
*/
/**
 * Routines for writing ints,floats,booleans.
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

#include <errno.h>   /* Error number definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <tuxeip/TuxEip.h>
#include "log_udp.h"
#include "eip_address.h"
#include "eip_general.h"
#include "eip_read.h"
#include "eip_session.h"
#include "eip_session_private.h"
#include "eip_write.h"

/* internal variables */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: eip_write.c,v 1.3 2009-02-05 11:36:18 cjm Exp $";

/* -------------------------------------------------------------
** external functions 
** ------------------------------------------------------------- */
/**
 * Write an integer value to the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to write to, of the form 'N7:0'.
 * @param value The integer value to store in PLC memory location contents.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Write_Integer(EIP_Handle_T *handle,char *tag_name,int value)
{
	DHP_Header dhp={0,0,0,0};
	int retval;

	if(handle == NULL)
	{
		EIP_Error_Number = 200;
		sprintf(EIP_Error_String,"EIP_Write_Integer : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 201;
		sprintf(EIP_Error_String,"EIP_Write_Integer : tag_name was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 202;
		sprintf(EIP_Error_String,"EIP_Write_Integer : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 203;
		sprintf(EIP_Error_String,"EIP_Write_Integer : The handle's Connection was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Integer (%p,%s,%d) Started.",handle,tag_name,value);
#endif /* LOGGING */
	switch(handle->Plc.PlcType)
	{
		case PLC5:
		case SLC500:
			dhp.Dest_adress=handle->Plc.Node;
			if(handle->Plc.NetWork) /* DHP */
			{
				retval = WritePLCData(handle->Session,handle->Connection,&dhp,NULL,0,
						      handle->Plc.PlcType,handle->Tns++,tag_name,PLC_INTEGER,&value,1);
			}
			else
			{
				retval = WritePLCData(handle->Session,handle->Connection,NULL,NULL,0,
						      handle->Plc.PlcType,handle->Tns++,tag_name,PLC_INTEGER,&value,1);
			}
			if(retval <0)
			{
				EIP_Error_Number = 204;
				sprintf(EIP_Error_String,"EIP_Write_Integer : write(%s,%d) Failed (%d,%s).",
					tag_name,value,cip_errno,cip_err_msg);
				return FALSE;
			}
			break;
		case LGX:
			/* LGX_INT or LGX_SINT/LGX_DINT ? */
			if(WriteLgxData(handle->Session,handle->Connection,tag_name,LGX_INT,&value,1)<=0)
			{
				EIP_Error_Number = 205;
				sprintf(EIP_Error_String,"EIP_Write_Integer : write(%s,%d) Failed (%d,%s).",
					tag_name,value,cip_errno,cip_err_msg);
				return FALSE;
			}
			break;
		default:
			EIP_Error_Number = 206;
			sprintf(EIP_Error_String,"EIP_Write_Integer :Unknown PLC type %d.",handle->Plc.PlcType);
			return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Integer : handle %p : %s set to %d.",handle,tag_name,value);
#endif /* LOGGING */
	return TRUE;
}

/**
 * Write a float value to the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to write to, of the form 'F8:0'.
 * @param value The float value to store in PLC memory location contents.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Write_Float(EIP_Handle_T *handle,char *tag_name,float value)
{
	DHP_Header dhp={0,0,0,0};
	int retval;

	if(handle == NULL)
	{
		EIP_Error_Number = 207;
		sprintf(EIP_Error_String,"EIP_Write_Float : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 208;
		sprintf(EIP_Error_String,"EIP_Write_Float : tag_name was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 209;
		sprintf(EIP_Error_String,"EIP_Write_Float : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 210;
		sprintf(EIP_Error_String,"EIP_Write_Float : The handle's Connection was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Float (%p,%s,%f) Started.",handle,tag_name,value);
#endif /* LOGGING */
	switch(handle->Plc.PlcType)
	{
		case PLC5:
		case SLC500:
			dhp.Dest_adress=handle->Plc.Node;
			if(handle->Plc.NetWork) /* DHP */
			{
				retval = WritePLCData(handle->Session,handle->Connection,&dhp,NULL,0,
						      handle->Plc.PlcType,handle->Tns++,tag_name,PLC_FLOATING,&value,1);
			}
			else
			{
				retval = WritePLCData(handle->Session,handle->Connection,NULL,NULL,0,
						      handle->Plc.PlcType,handle->Tns++,tag_name,PLC_FLOATING,&value,1);
			}
			if(retval <0)
			{
				EIP_Error_Number = 211;
				sprintf(EIP_Error_String,"EIP_Write_Float : write(%s,%f) Failed (%d,%s).",
					tag_name,value,cip_errno,cip_err_msg);
				return FALSE;
			}
			break;
		case LGX:
			/* LGX_INT or LGX_SINT/LGX_DINT ? */
			if(WriteLgxData(handle->Session,handle->Connection,tag_name,LGX_REAL,&value,1)<=0)
			{
				EIP_Error_Number = 212;
				sprintf(EIP_Error_String,"EIP_Write_Float : write(%s,%f) Failed (%d,%s).",
					tag_name,value,cip_errno,cip_err_msg);
				return FALSE;
			}
			break;
		default:
			EIP_Error_Number = 213;
			sprintf(EIP_Error_String,"EIP_Write_Float :Unknown PLC type %d.",handle->Plc.PlcType);
			return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Float : handle %p : %s set to %f.",handle,tag_name,value);
#endif /* LOGGING */
	return TRUE;
}

/**
 * Write a boolean value to the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to write to, of the form 'N7:0/1'.
 * @param value The boolean value to store in PLC memory location contents, must be TRUE (1) or FALSE (0).
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 * @see eip_general.html#EIP_GENERAL_IS_BOOLEAN
 * @see eip_address.html#EIP_PLC_Memory_Address_Struct_T
 * @see eip_address.html#EIP_Address_Parse
 * @see eip_address.html#EIP_Address_Create
 */
int EIP_Write_Boolean(EIP_Handle_T *handle,char *tag_name,int value)
{
	EIP_PLC_Memory_Address_T memory_address;
	char word_address_buff[32];
	int word_value;

	if(handle == NULL)
	{
		EIP_Error_Number = 214;
		sprintf(EIP_Error_String,"EIP_Write_Boolean : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 215;
		sprintf(EIP_Error_String,"EIP_Write_Boolean : tag_name was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 216;
		sprintf(EIP_Error_String,"EIP_Write_Boolean : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 217;
		sprintf(EIP_Error_String,"EIP_Write_Boolean : The handle's Connection was NULL.");
		return FALSE;
	}
	if(EIP_IS_BOOLEAN(value) == FALSE)
	{
		EIP_Error_Number = 218;
		sprintf(EIP_Error_String,"EIP_Write_Boolean : The value was not a valid boolean (%d).",value);
		return FALSE;
	}		
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Boolean (%p,%s,%d) Started.",handle,tag_name,value);
#endif /* LOGGING */
	/* decipher the input address into word and bit components */
	if(!EIP_Address_Parse(tag_name,&memory_address))
		return FALSE;
	if(!EIP_Address_Create(memory_address,word_address_buff,FALSE))
		return FALSE;
	/* read the current word */
	if(!EIP_Read_Integer(handle,word_address_buff,&word_value))
		return FALSE;
	/* set/clear the relevant bit in the word */
	if(value == TRUE)
	{
		word_value |= (1<<memory_address.Sub_Element_Number);
	}
	else
	{
		word_value &= ~(1<<memory_address.Sub_Element_Number);
	}
	/* write the word back to the plc */
	if(!EIP_Write_Integer(handle,word_address_buff,word_value))
		return FALSE;
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Write_Boolean : handle %p : %s set to %d.",handle,tag_name,value);
#endif /* LOGGING */
	return TRUE;
}

/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2008/12/15 12:02:50  cjm
** Added handle logging.
**
** Revision 1.1  2008/10/15 13:48:23  cjm
** Initial revision
**
*/
