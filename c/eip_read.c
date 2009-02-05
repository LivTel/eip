/* eip_read.c
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/c/eip_read.c,v 1.3 2009-02-05 11:36:18 cjm Exp $
*/
/**
 * Routines for reading ints,floats,booleans.
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
#include "eip_general.h"
#include "eip_session.h"
#include "eip_session_private.h"
#include "eip_read.h"

/* internal variables */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: eip_read.c,v 1.3 2009-02-05 11:36:18 cjm Exp $";

/* -------------------------------------------------------------
** external functions 
** ------------------------------------------------------------- */
/**
 * Read an integer value from the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to read, of the form 'N7:0'.
 * @param value The address of an integer to store the PLC memory location contents.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Read_Integer(EIP_Handle_T *handle,char *tag_name,int *value)
{
	DHP_Header dhp={0,0,0,0};
	PLC_Read *plc_data;
	LGX_Read *lgx_data;

	if(handle == NULL)
	{
		EIP_Error_Number = 100;
		sprintf(EIP_Error_String,"EIP_Read_Integer : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 101;
		sprintf(EIP_Error_String,"EIP_Read_Integer : tag_name was NULL.");
		return FALSE;
	}
	if(value == NULL)
	{
		EIP_Error_Number = 102;
		sprintf(EIP_Error_String,"EIP_Read_Integer : value was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 112;
		sprintf(EIP_Error_String,"EIP_Read_Integer : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 113;
		sprintf(EIP_Error_String,"EIP_Read_Integer : The handle's Connection was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Integer (%p,%s) Started.",handle,tag_name);
#endif /* LOGGING */
	switch(handle->Plc.PlcType)
	{
		case PLC5:
		case SLC500:
			dhp.Dest_adress=handle->Plc.Node;
			if(handle->Plc.NetWork) /* DHP */
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,&dhp,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			else
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,NULL,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			if(plc_data == NULL)
			{
				EIP_Error_Number = 103;
				sprintf(EIP_Error_String,"EIP_Read_Integer : plc_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch (plc_data->type)
			{
				case PLC_BIT:
				case PLC_INTEGER:
					(*value) = PCCC_GetValueAsInteger(plc_data,0);/* Boolean eh ? */
					if(cip_errno != 0)
					{
						free(plc_data);
						EIP_Error_Number = 104;
						sprintf(EIP_Error_String,
							"EIP_Read_Integer : PCCC_GetValue failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case PLC_FLOATING:
				default:
					free(plc_data);
					EIP_Error_Number = 105;
					sprintf(EIP_Error_String,"EIP_Read_Integer : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(plc_data);
			break;
		case LGX:
			lgx_data = ReadLgxData(handle->Session,handle->Connection,tag_name,1);
			if(lgx_data == NULL)
			{
				EIP_Error_Number = 106;
				sprintf(EIP_Error_String,"EIP_Read_Integer : lgx_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch(lgx_data->type)
			{
				case LGX_BOOL:
					(*value) = GetLGXValueAsInteger(lgx_data,0);
					if(cip_errno != 0)
					{
						free(lgx_data);
						EIP_Error_Number = 107;
						sprintf(EIP_Error_String,
							"EIP_Read_Integer : GetLGXValueAsInteger failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
				case LGX_BITARRAY:
					free(lgx_data);
					EIP_Error_Number = 108;
					sprintf(EIP_Error_String,
						"EIP_Read_Integer : LGX_BITARRAY not implemented.");
					return FALSE;
				case LGX_SINT:
				case LGX_INT:
				case LGX_DINT:
					(*value) = GetLGXValueAsInteger(lgx_data,0);
					if(cip_errno != 0)
					{
						free(lgx_data);
						EIP_Error_Number = 109;
						sprintf(EIP_Error_String,
							"EIP_Read_Integer : GetLGXValueAsInteger failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case LGX_REAL:
					free(lgx_data);
					EIP_Error_Number = 110;
					sprintf(EIP_Error_String,"EIP_Read_Integer : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(lgx_data);
			break;
		default:
			EIP_Error_Number = 111;
			sprintf(EIP_Error_String,"EIP_Read_Integer :Unknown PLC type %d.",handle->Plc.PlcType);
			return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Integer : handle %p : %s Returned value %d.",handle,tag_name,
		       (*value));
#endif /* LOGGING */
	return TRUE;
}

/**
 * Read an float value from the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to read, of the form 'F8:0'.
 * @param value The address of a float to store the PLC memory location contents.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Read_Float(EIP_Handle_T *handle,char *tag_name,float *value)
{
	DHP_Header dhp={0,0,0,0};
	PLC_Read *plc_data;
	LGX_Read *lgx_data;

	if(handle == NULL)
	{
		EIP_Error_Number = 114;
		sprintf(EIP_Error_String,"EIP_Read_Float : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 115;
		sprintf(EIP_Error_String,"EIP_Read_Float : tag_name was NULL.");
		return FALSE;
	}
	if(value == NULL)
	{
		EIP_Error_Number = 116;
		sprintf(EIP_Error_String,"EIP_Read_Float : value was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 117;
		sprintf(EIP_Error_String,"EIP_Read_Float : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 118;
		sprintf(EIP_Error_String,"EIP_Read_Float : The handle's Connection was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Float (%p,%s) Started.",handle,tag_name);
#endif /* LOGGING */
	switch(handle->Plc.PlcType)
	{
		case PLC5:
		case SLC500:
			dhp.Dest_adress=handle->Plc.Node;
			if(handle->Plc.NetWork) /* DHP */
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,&dhp,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			else
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,NULL,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			if(plc_data == NULL)
			{
				EIP_Error_Number = 119;
				sprintf(EIP_Error_String,"EIP_Read_Float : plc_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch (plc_data->type)
			{
				case PLC_FLOATING:
					(*value) = PCCC_GetValueAsFloat(plc_data,0);
					if(cip_errno != 0)
					{
						free(plc_data);
						EIP_Error_Number = 120;
						sprintf(EIP_Error_String,
							"EIP_Read_Float : PCCC_GetValue failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case PLC_BIT:
				case PLC_INTEGER:
				default:
					free(plc_data);
					EIP_Error_Number = 121;
					sprintf(EIP_Error_String,"EIP_Read_Float : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(plc_data);
			break;
		case LGX:
			lgx_data = ReadLgxData(handle->Session,handle->Connection,tag_name,1);
			if(lgx_data == NULL)
			{
				EIP_Error_Number = 122;
				sprintf(EIP_Error_String,"EIP_Read_Float : lgx_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch(lgx_data->type)
			{
				case LGX_REAL:
					(*value) = GetLGXValueAsFloat(lgx_data,0);
					if(cip_errno != 0)
					{
						free(lgx_data);
						EIP_Error_Number = 123;
						sprintf(EIP_Error_String,
							"EIP_Read_Float : GetLGXValueAsFloat failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case LGX_BITARRAY:
					free(lgx_data);
					EIP_Error_Number = 124;
					sprintf(EIP_Error_String,
						"EIP_Read_Float : LGX_BITARRAY not implemented.");
					return FALSE;
				case LGX_BOOL:
				case LGX_SINT:
				case LGX_INT:
				case LGX_DINT:
					free(lgx_data);
					EIP_Error_Number = 125;
					sprintf(EIP_Error_String,"EIP_Read_Float : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(lgx_data);
			break;
		default:
			EIP_Error_Number = 126;
			sprintf(EIP_Error_String,"EIP_Read_Float :Unknown PLC type %d.",handle->Plc.PlcType);
			return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Float : handle %p : %s Returned value %f.",handle,tag_name,(*value));
#endif /* LOGGING */
	return TRUE;
}

/**
 * Read an boolean value from the PLC. A session must have previously been opened using
 * EIP_Session_Handle_Create / EIP_Session_Open.
 * @param handle The handle to communicate over.
 * @param tag_name The memory location of the tag to read, of the form 'N7:0/1'.
 * @param value The address of an boolean to store the PLC memory location contents.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Read_Boolean(EIP_Handle_T *handle,char *tag_name,int *value)
{
	DHP_Header dhp={0,0,0,0};
	PLC_Read *plc_data;
	LGX_Read *lgx_data;

	if(handle == NULL)
	{
		EIP_Error_Number = 127;
		sprintf(EIP_Error_String,"EIP_Read_Boolean : handle was NULL.");
		return FALSE;
	}
	if(tag_name == NULL)
	{
		EIP_Error_Number = 128;
		sprintf(EIP_Error_String,"EIP_Read_Boolean : tag_name was NULL.");
		return FALSE;
	}
	if(value == NULL)
	{
		EIP_Error_Number = 129;
		sprintf(EIP_Error_String,"EIP_Read_Boolean : value was NULL.");
		return FALSE;
	}
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 139;
		sprintf(EIP_Error_String,"EIP_Read_Boolean : The handle's Session was NULL.");
		return FALSE;
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 130;
		sprintf(EIP_Error_String,"EIP_Read_Boolean : The handle's Connection was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Boolean (%p,%s) Started.",handle,tag_name);
#endif /* LOGGING */
	switch(handle->Plc.PlcType)
	{
		case PLC5:
		case SLC500:
			dhp.Dest_adress=handle->Plc.Node;
			if(handle->Plc.NetWork) /* DHP */
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,&dhp,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			else
			{
				plc_data = ReadPLCData(handle->Session,handle->Connection,NULL,NULL,0,
						       handle->Plc.PlcType,handle->Tns++,tag_name,1);
			}
			if(plc_data == NULL)
			{
				EIP_Error_Number = 131;
				sprintf(EIP_Error_String,"EIP_Read_Boolean : plc_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch (plc_data->type)
			{
				case PLC_BIT:
					(*value) = PCCC_GetValueAsBoolean(plc_data,0);
					if(cip_errno != 0)
					{
						free(plc_data);
						EIP_Error_Number = 132;
						sprintf(EIP_Error_String,
							"EIP_Read_Boolean : PCCC_GetValue failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case PLC_INTEGER:
					(*value) = PCCC_GetValueAsBoolean(plc_data,0);
					if(cip_errno != 0)
					{
						free(plc_data);
						EIP_Error_Number = 140;
						sprintf(EIP_Error_String,
							"EIP_Read_Boolean : PCCC_GetValue failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					break;
				case PLC_FLOATING:
				default:
					free(plc_data);
					EIP_Error_Number = 133;
					sprintf(EIP_Error_String,"EIP_Read_Boolean : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(plc_data);
			break;
		case LGX:
			lgx_data = ReadLgxData(handle->Session,handle->Connection,tag_name,1);
			if(lgx_data == NULL)
			{
				EIP_Error_Number = 134;
				sprintf(EIP_Error_String,"EIP_Read_Boolean : lgx_data was NULL(%s,%d,%s).",
					tag_name,cip_errno,cip_err_msg);
				return FALSE;
			}
			switch(lgx_data->type)
			{
				case LGX_BOOL:
					(*value) = GetLGXValueAsInteger(lgx_data,0);
					if(cip_errno != 0)
					{
						free(lgx_data);
						EIP_Error_Number = 135;
						sprintf(EIP_Error_String,
							"EIP_Read_Boolean : GetLGXValueAsInteger failed (%s,%d,%s).",
							tag_name,cip_errno,cip_err_msg);
						return FALSE;
					}
					/* if value is non-zero, set to TRUE */
					if((*value) != 0)
						(*value) = TRUE;
				case LGX_BITARRAY:
					free(lgx_data);
					EIP_Error_Number = 136;
					sprintf(EIP_Error_String,
						"EIP_Read_Boolean : LGX_BITARRAY not implemented.");
					return FALSE;
				case LGX_SINT:
				case LGX_INT:
				case LGX_DINT:
				case LGX_REAL:
					free(lgx_data);
					EIP_Error_Number = 137;
					sprintf(EIP_Error_String,"EIP_Read_Boolean : data had Unknown/Wrong type %d.",
						plc_data->type);
					return FALSE;
			}
			free(lgx_data);
			break;
		default:
			EIP_Error_Number = 138;
			sprintf(EIP_Error_String,"EIP_Read_Boolean :Unknown PLC type %d.",handle->Plc.PlcType);
			return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERBOSE,"EIP_Read_Boolean : handle %p : %s Returned value %d.",handle,tag_name,
		       (*value));
#endif /* LOGGING */
	return TRUE;
}
/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2008/12/15 12:02:52  cjm
** Added handle logging.
**
** Revision 1.1  2008/10/15 13:48:23  cjm
** Initial revision
**
*/
