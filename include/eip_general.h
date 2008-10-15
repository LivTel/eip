/* eip_general.h
** $Header: /home/cjm/cvs/eip/include/eip_general.h,v 1.1 2008-10-15 13:48:28 cjm Exp $
*/

#ifndef EIP_GENERAL_H
#define EIP_GENERAL_H

/* hash defines */
/**
 * TRUE is the value usually returned from routines to indicate success.
 */
#ifndef TRUE
#define TRUE 1
#endif
/**
 * FALSE is the value usually returned from routines to indicate failure.
 */
#ifndef FALSE
#define FALSE 0
#endif

/**
 * How long the error string is.
 */
#define EIP_ERROR_LENGTH (1024)

/* These constants should be the same as those in ngat.net.plc.EIPLibrary.java */
/**
 * Value to pass into logging calls, used for session software logging.
 * The eip library is allocated bits 16..23.
 * This constant should be the same as that defined in ngat.net.plc.EIPLibrary.java.
 * @see #EIP_Log
 */
#define EIP_LOG_BIT_SESSION	(1<<16)
/**
 * Value to pass into logging calls, used for read logging.
 * The eip library is allocated bits 16..23.
 * This constant should be the same as that defined in ngat.net.plc.EIPLibrary.java.
 * @see #EIP_Log
 */
#define EIP_LOG_BIT_READ	(1<<17)
/**
 * Value to pass into logging calls, used for read logging.
 * The eip library is allocated bits 16..23.
 * This constant should be the same as that defined in ngat.net.plc.EIPLibrary.java.
 * @see #EIP_Log
 */
#define EIP_LOG_BIT_WRITE	(1<<18)
/**
 * Value to pass into logging calls, used for address logging.
 * The eip library is allocated bits 16..23.
 * This constant should be the same as that defined in ngat.net.plc.EIPLibrary.java.
 * @see #EIP_Log
 */
#define EIP_LOG_BIT_ADDRESS	(1<<19)

/**
 * Macro to check whether the parameter is either TRUE or FALSE.
 */
#define EIP_IS_BOOLEAN(value)	(((value) == TRUE)||((value) == FALSE))

/* external functions */
extern void EIP_General_Error(void);
extern void EIP_General_Error_To_String(char *error_string);
extern int EIP_Get_Error_Number(void);
extern void EIP_Get_Current_Time_String(char *time_string,int string_length);
extern void EIP_Log_Format(int level,char *format,...);
extern void EIP_Log(int level,char *string);
extern void EIP_Set_Log_Handler_Function(void (*log_fn)(int level,char *string));
extern void EIP_Set_Log_Filter_Function(int (*filter_fn)(int level,char *string));
extern void EIP_Log_Handler_Stdout(int level,char *string);
extern void EIP_Set_Log_Filter_Level(int level);
extern int EIP_Log_Filter_Level_Absolute(int level,char *string);
extern int EIP_Log_Filter_Level_Bitwise(int level,char *string);
extern char *EIP_Replace_String(char *string,char *find_string,char *replace_string);

/* external variables */
extern int EIP_Error_Number;
extern char EIP_Error_String[];

/*
** $Log: not supported by cvs2svn $
*/
#endif