/* eip_address.h
** $Header: /home/cjm/cvs/eip/include/eip_address.h,v 1.1 2008-10-15 13:48:28 cjm Exp $
*/

#ifndef EIP_ADDRESS_H
#define EIP_ADDRESS_H
/* type definitions */

/**
 * DF1 Protocol type definition.
 */
typedef unsigned char byte;	/* create byte type */

/**
 * DF1 Protocol Three Address Fields structure.
 * <dl>
 * <dt>Size</dt> <dd>Size of the PLC address in bytes.</dd>
 * <dt>File_Number</dt> <dd>The number, i.e. 7 (as a 1 byte integer) in N7:1.</dd>
 * <dt>File_Letter</dt> <dd>The letter, i.e. 'N' (as a character) in N7:1.</dd>
 * <dt>File_Type</dt> <dd>The PLC File Type (based on File_Letter). 'O'=0x8b, 'I'=0x8c, 'N'=0x89, 'F'=0x8a etc.</dd>
 * <dt>Element_Number</dt> <dd>The file element number i.e. 1 (as a 1 byte integer) in N7:1.</dd>
 * <dt>Sub_Element_Number</dt> <dd>The file sub-element number as a 1 byte integer. i.e. 0 in N7:1, 3 in N7:1/3</dd>
 * </dl>
 * @see #byte
 */
typedef struct EIP_PLC_Memory_Address_Struct
{
	byte Size;
	byte File_Letter;
	byte File_Number;
	byte File_Type;
	byte Element_Number;
	byte Sub_Element_Number;
} EIP_PLC_Memory_Address_T;

/* external functions */
extern int EIP_Address_Parse(char *straddress,EIP_PLC_Memory_Address_T *address);
extern int EIP_Address_Create(EIP_PLC_Memory_Address_T address,char *straddress,int include_sub_element);

/*
** $Log: not supported by cvs2svn $
*/
#endif
