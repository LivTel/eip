/* eip_session_private.h
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/include/eip_session_private.h,v 1.1 2023-03-21 17:15:11 cjm Exp $
*/
#ifndef EIP_SESSION_PRIVATE_H
#define EIP_SESSION_PRIVATE_H
#include <tuxeip/TuxEip.h>

/**
 * Maximum length of buffer holding path to PLC.
 */
#define MAXPATHSIZE 100
/**
 * Maximum length of PlcName buffer.
 */
#define MAXALIASSIZE 50

/**
 * PLC information.
 * <dl>
 * <dt>PlcName</dt> <dd>String containig name of PLC (of size MAXALIASSIZE).</dd>
 * <dt>PlcPath</dt> <dd></dd>
 * <dt>PlcType</dt> <dd></dd>
 * <dt>NetWork</dt> <dd></dd>
 * <dt>Node</dt> <dd></dd>
 * </dl>
 * @see #MAXALIASSIZE
 * @see #MAXPATHSIZE
 */
typedef struct _PLC
{
	char PlcName[MAXALIASSIZE];
	char PlcPath[MAXPATHSIZE];
	Plc_Type PlcType;
	int NetWork;
	int Node;
} PLC;

/**
 * Session Handle.
 * <dl>
 * <dt>Plc</dt> <dd>tuxeip PLC data structure.</dd>
 * <dt>Session</dt> <dd>tuxeip Session structure.</dd>
 * <dt>Connection</dt> <dd>tuxeip Connection structure.</dd>
 * <dt>Tns</dt> <dd>Transaction number.</dd>
 * </dl>
 */
struct EIP_Handle_Struct
{
	PLC Plc;
	Eip_Session *Session;
	Eip_Connection *Connection;
	int Tns;
};

#endif
/*
** $Log: not supported by cvs2svn $
*/
