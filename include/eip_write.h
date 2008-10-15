/* eip_write.h
** Ethernet over IP (CIP) PLC communication library.
** $Header: /home/cjm/cvs/eip/include/eip_write.h,v 1.1 2008-10-15 13:48:28 cjm Exp $
*/
#ifndef EIP_WRITE_H
#define EIP_WRITE_H

extern int EIP_Write_Integer(EIP_Handle_T *handle,char *tag_name,int value);
extern int EIP_Write_Float(EIP_Handle_T *handle,char *tag_name,float value);
extern int EIP_Write_Boolean(EIP_Handle_T *handle,char *tag_name,int value);

#endif
/*
** $Log: not supported by cvs2svn $
*/
