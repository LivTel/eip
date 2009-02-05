/* eip_session.c
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/c/eip_session.c,v 1.3 2009-02-05 11:36:18 cjm Exp $
*/
/**
 * Session handling routine.
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

/**
 * Timeout connection.
 */
#define MAX_SAMPLE 1000 

/* internal variables */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: eip_session.c,v 1.3 2009-02-05 11:36:18 cjm Exp $";

/* internal functions */
static int GetSerial(void);

/* -------------------------------------------------------------
** external functions 
** ------------------------------------------------------------- */
/**
 * Routine to allocate memeory for the interface handle.
 * @param handle The address of a pointer to allocate the handle.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Session_Handle_Create(EIP_Handle_T **handle)
{
#if LOGGING > 1
	EIP_Log(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Handle_Create Started.");
#endif /* LOGGING */
	if(handle == NULL)
	{
		EIP_Error_Number = 1;
		sprintf(EIP_Error_String,"EIP_Session_Handle_Create: handle was NULL.");
		return FALSE;
	}
	/* allocate handle */
	(*handle) = (EIP_Handle_T *)malloc(sizeof(EIP_Handle_T));
	if((*handle) == NULL)
	{
		EIP_Error_Number = 2;
		sprintf(EIP_Error_String,"EIP_Session_Handle_Create: Failed to allocate handle.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Handle_Create Finished with new handle %p.",(*handle));
#endif /* LOGGING */
	return TRUE;
}

/**
 * Routine to open a session to a Micrologix 1100 PLC.
 * @param hostname The IP address of the PLC.
 * @param backplane The backplane of the PLC. 1 for Micrologix1100.
 * @param slot The slot containing the PLC. 1 for Micrologix1100.
 * @param plc_type The type of PLC we are talking to. 
 * @param handle A pointer to a previously created handle. Filled in as the Session is opened/registered.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 * @see #EIP_SESSION_PLC_TYPE
 * @see #MAX_SAMPLE
 */
int EIP_Session_Open(char *hostname,int backplane,int slot,enum EIP_SESSION_PLC_TYPE plc_type,EIP_Handle_T *handle)
{
	int retval,path_size;
	BYTE path[2];

	if(hostname == NULL)
	{
		EIP_Error_Number = 5;
		sprintf(EIP_Error_String,"EIP_Session_Open: hostname was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Open(hostname=%s,backplane=%d,slot=%d,handle=%p) Started.",
		       hostname,backplane,slot,handle);
#endif /* LOGGING */
	if(handle == NULL)
	{
		EIP_Error_Number = 6;
		sprintf(EIP_Error_String,"EIP_Session_Open: handle was NULL.");
		return FALSE;
	}
	if((strlen(hostname)+5) > MAXPATHSIZE)
	{
		EIP_Error_Number = 7;
		sprintf(EIP_Error_String,"EIP_Session_Open: hostname was too long(%d).",strlen(hostname));
		return FALSE;
	}
	/* create PLC path */
	sprintf(handle->Plc.PlcPath,"%s,%d,%d",hostname,backplane,slot);
	if(plc_type == PLC_TYPE_MICROLOGIX1100)
	{
		/* Micrologix 1100 */
		handle->Plc.PlcType = SLC500;
		handle->Plc.NetWork = 0;
		handle->Plc.Node = 0;
		/* create path : 2 elements, backplane and slot */
		path_size = 2;
		path[0] = (BYTE)backplane;
		path[1] = (BYTE)slot;
	}
	else
	{
		EIP_Error_Number = 8;
		sprintf(EIP_Error_String,"EIP_Session_Open: Unknown PLC type(%d).",plc_type);
		return FALSE;
	}
	/* initialise session/connection */
	handle->Session = NULL;
	handle->Connection = NULL;
	handle->Tns = 1;
	/* see readtag.c : Connect */
	handle->Session=OpenSession(hostname);
	if(handle->Session == NULL)
	{
		EIP_Error_Number = 9;
		sprintf(EIP_Error_String,"EIP_Session_Open: Opening Session failed(%s,%d,%s).",hostname,
			cip_errno,cip_err_msg);
		return FALSE;
	}
	retval = RegisterSession(handle->Session);
	if (retval <0)
	{
		EIP_Error_Number = 10;
		sprintf(EIP_Error_String,"EIP_Session_Open: Registering Session failed(%s,%d,%s).",hostname,
			cip_errno,cip_err_msg);
		CloseSession(handle->Session);
		return FALSE;
	}
	if (handle->Plc.NetWork)
	{
		handle->Connection=ConnectPLCOverDHP(handle->Session,handle->Plc.PlcType,(int)(handle->Session),
						     GetSerial(),MAX_SAMPLE,handle->Plc.NetWork,path,path_size);
	}
	else
	{
		handle->Connection=ConnectPLCOverCNET(handle->Session,handle->Plc.PlcType,(int)(handle->Session),
						      GetSerial(),MAX_SAMPLE,path,path_size);
	}
	if(handle->Connection == NULL)
	{
		EIP_Error_Number = 11;
		sprintf(EIP_Error_String,"EIP_Session_Open: ConnectPLC failed(%s,%d,%s).",hostname,
			cip_errno,cip_err_msg);
		/* unregister session */
		UnRegisterSession(handle->Session);
		/* close session */
		CloseSession(handle->Session);
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Open Finished.");
#endif /* LOGGING */
	return TRUE;
}

/**
 * Close a previously opened session.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Session_Close(EIP_Handle_T *handle)
{
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Close(handle=%p) Started.",handle);
#endif /* LOGGING */
	/* close connection */
	if(handle->Connection != NULL)
		Forward_Close(handle->Connection);
	handle->Connection = NULL;
	/* unregister session */
	UnRegisterSession(handle->Session);
	/* close session */
	CloseSession(handle->Session);
	handle->Session = NULL;
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Close(handle=%p) Finished.",handle);
#endif /* LOGGING */
	return TRUE;
}

/**
 * Routine to destroy the specified handle.
 * @return The routine returns TRUE on success and FALSE on failure.
 * @see #EIP_Handle_T
 */
int EIP_Session_Handle_Destroy(EIP_Handle_T **handle)
{
	/* check parameters */
	if(handle == NULL)
	{
		EIP_Error_Number = 3;
		sprintf(EIP_Error_String,"EIP_Session_Handle_Destroy : handle was NULL.");
		return FALSE;
	}
#if LOGGING > 1
	EIP_Log_Format(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Handle_Destroy(handle=%p) Started.",(*handle));
#endif /* LOGGING */
	if((*handle) == NULL)
	{
		EIP_Error_Number = 4;
		sprintf(EIP_Error_String,"EIP_Session_Handle_Destroy : handle pointer was NULL.");
		return FALSE;
	}
	/* free alocated handle */
	free((*handle));
	(*handle) = NULL;
#if LOGGING > 1
	EIP_Log(LOG_VERBOSITY_VERY_VERBOSE,"EIP_Session_Handle_Destroy Finished.");
#endif /* LOGGING */
	return TRUE;
}

/* -------------------------------------------------------------
** internal functions 
** ------------------------------------------------------------- */
/**
 * Get a unique ID. Returns getpid.
 * @return Some sort of unique integer.
 */
static int GetSerial(void)
{
	return(getpid());
}
/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2008/12/15 12:02:54  cjm
** Added handle logging.
**
** Revision 1.1  2008/10/15 13:48:23  cjm
** Initial revision
**
*/
