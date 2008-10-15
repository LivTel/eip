/* eip_session.h
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/include/eip_session.h,v 1.1 2008-10-15 13:48:28 cjm Exp $
*/
#ifndef EIP_SESSION_H
#define EIP_SESSION_H
/**
 * Typedef for the session handle pointer, which is an instance of EIP_Handle_Struct.
 * @see #EIP_Handle_Struct
 */
typedef struct EIP_Handle_Struct EIP_Handle_T;

/**
 * What type of PLC we are talking to.
 * <ul>
 * <li>PLC_TYPE_NONE
 * <li>PLC_TYPE_MICROLOGIX1100
 * </ul>
 */
enum EIP_SESSION_PLC_TYPE
{
	PLC_TYPE_NONE,PLC_TYPE_MICROLOGIX1100
};

extern int EIP_Session_Handle_Create(EIP_Handle_T **handle);
extern int EIP_Session_Open(char *hostname,int backplane,int slot,enum EIP_SESSION_PLC_TYPE plc_type,
			    EIP_Handle_T *handle);
extern int EIP_Session_Close(EIP_Handle_T *handle);
extern int EIP_Session_Handle_Destroy(EIP_Handle_T **handle);

#endif
/*
** $Log: not supported by cvs2svn $
*/
