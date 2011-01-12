/* ngat_eip_EIPPLC.c
** implementation of Java Class ngat.eip.EIPPLC native interfaces.
** $Header: /home/cjm/cvs/eip/c/ngat_eip_EIPPLC.c,v 1.3 2011-01-12 14:07:55 cjm Exp $
*/
/**
 * ngat_eip_EIPPLC.c is the 'glue' between libeip, 
 * the C library used to interface to a PLC over Ethernet/IP, and EIPPLC.java, 
 * a Java Class to drive the server. This file specifically
 * contains all the native C routines corresponding to native methods in Java.
 * @author Chris Mottram LJMU
 * @version $Revision: 1.3 $
 */
/**
 * This hash define is needed before including source files give us POSIX.4/IEEE1003.1b-1993 prototypes
 * for time.
 */
#define _POSIX_SOURCE 1
/**
 * This hash define is needed before including source files give us POSIX.4/IEEE1003.1b-1993 prototypes
 * for time.
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include <time.h>
#include <termios.h>
#include "eip_general.h"
#include "eip_read.h"
#include "eip_write.h"
#include "eip_session.h"

/* hash definitions */
/**
 * Hash define for the size of the array holding EIPPLC Instance (jobject) maps to EIP_Handle_T.
 * Set to 5.
 */
#define HANDLE_MAP_SIZE         (5)

/* internal structures */
/**
 * Structure holding mapping between EIPHandle Instances (jobject) to EIP_Handle_T.
 * <dl>
 * <dt>Java_Handle</dt> <dd>jobject reference for the Java instance.</dd>
 * <dt>C_Handle</dt> <dd>Pointer to the EIP_Handle_T for that Java instance.</dd>
 * </dl>
 */
struct Handle_Map_Struct
{
	jobject Java_Handle;
	EIP_Handle_T* C_Handle;
};


/* internal variables */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: ngat_eip_EIPPLC.c,v 1.3 2011-01-12 14:07:55 cjm Exp $";

/**
 * Copy of the java virtual machine pointer, used for logging back up to the Java layer from C.
 */
static JavaVM *java_vm = NULL;
/**
 * Cached global reference to the "ngat.eip.EIPPLC" logger, 
 * used to log back to the Java layer from C routines.
 */
static jobject logger = NULL;
/**
 * Cached reference to the "ngat.util.logging.Logger" class's 
 * log(int level,String clazz,String source,String message) method.
 * Used to log C layer log messages, in conjunction with the logger's object reference logger.
 * @see #logger
 */
static jmethodID log_method_id = NULL;
/**
 * Internal list of maps between EIPHandle jobject's (i.e. EIPHandle references), and
 * EIP_Handle_T handles (C/tuxeip handle's).
 * @see #Handle_Map_Struct
 * @see #HANDLE_MAP_SIZE
 */
static struct Handle_Map_Struct Handle_Map_List[HANDLE_MAP_SIZE] = 
{
	{NULL,NULL},
	{NULL,NULL},
	{NULL,NULL},
	{NULL,NULL},
	{NULL,NULL}
};

/* internal routines */
static void EIPPLC_Throw_Exception(JNIEnv *env,jobject obj,char *function_name);
static void EIPPLC_Throw_Exception_String(JNIEnv *env,jobject obj,char *function_name,char *error_string);
static void EIPPLC_Log_Handler(char *class,char *source,int level,char *string);
static int EIPPLC_Handle_Map_Add(JNIEnv *env,jobject instance,jobject j_handle,EIP_Handle_T* c_handle);
static int EIPPLC_Handle_Map_Delete(JNIEnv *env,jobject instance,jobject j_handle);
static int EIPPLC_Handle_Map_Find(JNIEnv *env,jobject instance,jobject j_handle,EIP_Handle_T** c_handle);

/* ------------------------------------------------------------------------------
** 		External routines
** ------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------
** 		EIPPLC C layer initialisation
** ------------------------------------------------------------------------------ */
/**
 * This routine gets called when the native library is loaded. We use this routine
 * to get a copy of the JavaVM pointer of the JVM we are running in. This is used to
 * get the correct per-thread JNIEnv context pointer in EIPPLC_Log_Handler.
 * @see #java_vm
 * @see #EIPPLC_Log_Handler
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	java_vm = vm;
	return JNI_VERSION_1_2;
}

/*
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    initialiseLoggerReference<br>
 * Signature: (Lngat/util/logging/Logger;)V<br>
 * Java Native Interface implementation of EIPPLC's initialiseLoggerReference.
 * This takes the supplied logger object reference and stores it in the logger variable as a global reference.
 * The log method ID is also retrieved and stored.
 * The libeip's log handler is set to the JNI routine EIPPLC_Log_Handler.
 * The libeip's log filter function is set bitwise.
 * @param l The EIPPLC's "ngat.eip.EIPPLC" logger.
 * @see #EIPPLC_Log_Handler
 * @see #logger
 * @see #log_method_id
 * @see eip_general.html#EIP_Log_Filter_Level_Absolute
 * @see eip_general.html#EIP_Set_Log_Handler_Function
 * @see eip_general.html#EIP_Set_Log_Filter_Function
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_initialiseLoggerReference(JNIEnv *env,jobject obj,jobject l)
{
	jclass cls = NULL;

/* save logger instance */
	logger = (*env)->NewGlobalRef(env,l);
/* get the ngat.util.logging.Logger class */
	cls = (*env)->FindClass(env,"ngat/util/logging/Logger");
	/* if the class is null, one of the following exceptions occured:
	** ClassFormatError,ClassCircularityError,NoClassDefFoundError,OutOfMemoryError */
	if(cls == NULL)
		return;
/* get relevant method id to call */
/* log(int level,java/lang/String clazz,java/lang/String source,java/lang/String message) */
	log_method_id = (*env)->GetMethodID(env,cls,"log",
					    "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if(log_method_id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
	/* Make the C layer log back to the Java logger, using EIPPLC_Log_Handler JNI routine.  */
	EIP_Set_Log_Handler_Function(EIPPLC_Log_Handler);
	/* Make the filtering absolute, as expected by the C layer */
	EIP_Set_Log_Filter_Function(EIP_Log_Filter_Level_Absolute);
}


/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    finaliseLoggerReference<br>
 * Signature: ()V<br>
 * This native method is called from EIPPLC's finaliser method. It removes the global reference to
 * logger.
 * @see #logger
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_finaliseLoggerReference(JNIEnv *env, jobject obj)
{
	(*env)->DeleteGlobalRef(env,logger);
}

/* ------------------------------------------------------------------------------
** 		eip_general.c
** ------------------------------------------------------------------------------ */
/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Set_Log_Filter_Level<br>
 * Signature: (I)V<br>
 * @see eip_general.html#EIP_Set_Log_Filter_Level
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Set_1Log_1Filter_1Level(JNIEnv *env, jobject obj, jint level)
{
	EIP_Set_Log_Filter_Level(level);
}

/* ------------------------------------------------------------------------------
** 		eip_read.c
** ------------------------------------------------------------------------------ */
/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Read_Boolean<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;)Z<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_read.html#EIP_Read_Boolean
 */
JNIEXPORT jboolean JNICALL Java_ngat_eip_EIPPLC_EIP_1Read_1Boolean(JNIEnv *env,jobject obj,jstring class_jstring,
								   jstring source_jstring, jobject j_handle,
								   jstring plc_address_jstring)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval,value;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return (jboolean)FALSE; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	retval = EIP_Read_Boolean((char*)class,(char*)source,handle,(char *)plc_address_c,&value);
	/* If we created the C strings we need to free the memory it uses */
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Read_Boolean");
	return (jboolean)value;
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Read_Float<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;)F<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_read.html#EIP_Read_Float
 */
JNIEXPORT jfloat JNICALL Java_ngat_eip_EIPPLC_EIP_1Read_1Float(JNIEnv *env,jobject obj,jstring class_jstring,
							       jstring source_jstring,jobject j_handle, 
							       jstring plc_address_jstring)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval;
	float value;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return (jfloat)FALSE; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	retval = EIP_Read_Float((char*)class,(char*)source,handle,(char *)plc_address_c,&value);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Read_Float");
	return (jfloat)value;	
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Read_Integer<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;)I<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_read.html#EIP_Read_Integer
 */
JNIEXPORT jint JNICALL Java_ngat_eip_EIPPLC_EIP_1Read_1Integer(JNIEnv *env,jobject obj,jstring class_jstring,
							       jstring source_jstring,jobject j_handle, 
							       jstring plc_address_jstring)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval,value;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return (jint)FALSE; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	retval = EIP_Read_Integer((char*)class,(char*)source,handle,(char *)plc_address_c,&value);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Read_Integer");
	return (jint)value;	
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Session_Handle_Create<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Add
 * @see eip_session.html#EIP_Session_Handle_Create
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Session_1Handle_1Create(JNIEnv *env,jobject obj,
									 jstring class_jstring,
									 jstring source_jstring,jobject j_handle)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	int retval;

	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	/* create session handle */
	retval = EIP_Session_Handle_Create((char*)class,(char*)source,&handle);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
	{
		EIPPLC_Throw_Exception(env,obj,"EIP_Session_Handle_Create");
		return;
	}
	/* map the Java handle (obj) to the C layer handle */
	retval = EIPPLC_Handle_Map_Add(env,obj,j_handle,handle);
	if(retval == FALSE)
	{
		/* An error should have been thrown by EIPPLC_Handle_Map_Add */
		return;
	}
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Session_Handle_Open<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IIILngat/eip/EIPHandle;)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Session_Open
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Session_1Handle_1Open(JNIEnv *env,jobject obj,jstring class_jstring,
   jstring source_jstring,jstring hostname_jstring, jint backplane, jint slot, jint plc_type, jobject j_handle)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *hostname_cstring = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(hostname_jstring != NULL)
		hostname_cstring = (*env)->GetStringUTFChars(env,hostname_jstring,0);
	retval = EIP_Session_Open((char*)class,(char*)source,(char*)hostname_cstring,(int)backplane,(int)slot,
				  (int)plc_type,handle);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(hostname_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,hostname_jstring,hostname_cstring);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
	{
		EIPPLC_Throw_Exception(env,obj,"EIP_Session_Handle_Open");
		return;
	}
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Session_Handle_Close<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Session_Close
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Session_1Handle_1Close(JNIEnv *env,jobject obj,jstring class_jstring,
									jstring source_jstring,jobject j_handle)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	/* close session handle */
	retval = EIP_Session_Close((char*)class,(char*)source,handle);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
	{
		EIPPLC_Throw_Exception(env,obj,"EIP_Session_Handle_Close");
		return;
	}
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Session_Handle_Destroy<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see #EIPPLC_Handle_Map_Delete
 * @see eip_session.html#EIP_Session_Destroy
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Session_1Handle_1Destroy(JNIEnv *env,jobject obj,
						   jstring class_jstring,jstring source_jstring,jobject j_handle)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	/* destroy session handle */
	retval = EIP_Session_Handle_Destroy((char*)class,(char*)source,&handle);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
	{
		EIPPLC_Throw_Exception(env,obj,"EIP_Session_Handle_Destroy");
		return;
	}
	/* remove mapping from the Java handle (obj) to the C layer handle */
	retval = EIPPLC_Handle_Map_Delete(env,obj,j_handle);
	if(retval == FALSE)
	{
		/* An error should have been thrown by EIPPLC_Handle_Map_Delete */
		return;
	}
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Write_Boolean<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;Z)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_write.html#EIP_Write_Boolean
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Write_1Boolean(JNIEnv *env,jobject obj,
  jstring class_jstring,jstring source_jstring,jobject j_handle, jstring plc_address_jstring, jboolean value)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	retval = EIP_Write_Boolean((char*)class,(char*)source,handle,(char *)plc_address_c,(int)value);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Write_Boolean");
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Write_Float<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;F)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_write.html#EIP_Write_Float
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Write_1Float(JNIEnv *env,jobject obj, 
     jstring class_jstring,jstring source_jstring,jobject j_handle,jstring plc_address_jstring, jfloat value)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	retval = EIP_Write_Float((char*)class,(char*)source,handle,(char *)plc_address_c,(float)value);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Write_Float");
}

/**
 * Class:     ngat_eip_EIPPLC<br>
 * Method:    EIP_Write_Integer<br>
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lngat/eip/EIPHandle;Ljava/lang/String;I)V<br>
 * @see #EIPPLC_Throw_Exception
 * @see #EIPPLC_Handle_Map_Find
 * @see eip_session.html#EIP_Handle_T
 * @see eip_write.html#EIP_Write_Integer
 */
JNIEXPORT void JNICALL Java_ngat_eip_EIPPLC_EIP_1Write_1Integer(JNIEnv *env, jobject obj, jobject j_handle, 
			  jstring class_jstring,jstring source_jstring,jstring plc_address_jstring, jint value)
{
	EIP_Handle_T *handle = NULL;
	const char *class = NULL;
	const char *source = NULL;
	const char *plc_address_c = NULL;
	int retval;

	/* get interface handle from EIPHandle j_handle instance map */
	if(!EIPPLC_Handle_Map_Find(env,obj,j_handle,&handle))
		return; /* EIPPLC_Handle_Map_Find throws an exception on failure */
	/* Change the java strings to a c null terminated string
	** If the java String is null the C string should be null as well */
	if(class_jstring != NULL)
		class = (*env)->GetStringUTFChars(env,class_jstring,0);
	if(source_jstring != NULL)
		source = (*env)->GetStringUTFChars(env,source_jstring,0);
	if(plc_address_jstring != NULL)
		plc_address_c = (*env)->GetStringUTFChars(env,plc_address_jstring,0);
	retval = EIP_Write_Integer((char*)class,(char*)source,handle,(char *)plc_address_c,(int)value);
	/* If we created the C strings we need to free the memory it uses */
	if(class_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,class_jstring,class);
	if(source_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,source_jstring,source);
	if(plc_address_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,plc_address_jstring,plc_address_c);
	/* if an error occured throw an exception. */
	if(retval == FALSE)
		EIPPLC_Throw_Exception(env,obj,"EIP_Write_Integer");
}

/* ------------------------------------------------------------------------------
** 		Internal routines
** ------------------------------------------------------------------------------ */
/**
 * This routine throws an exception. The error generated is from the error codes in libeip, 
 * it assumes another routine has generated an error and this routine packs this error into an exception to return
 * to the Java code, using EIPPLC_Throw_Exception_String. The total length of the error string should
 * not be longer than EIP_ERROR_LENGTH. A new line is added to the start of the error string,
 * so that the error string returned from libeip is formatted properly.
 * @param env The JNI environment pointer.
 * @param function_name The name of the function in which this exception is being generated for.
 * @param obj The instance of EIPPLC that threw the error.
 * @see eip_general.html#EIP_General_Error_To_String
 * @see #EIPPLC_Throw_Exception_String
 * @see #EIP_ERROR_LENGTH
 */
static void EIPPLC_Throw_Exception(JNIEnv *env,jobject obj,char *function_name)
{
	char error_string[EIP_ERROR_LENGTH];

	strcpy(error_string,"\n");
	EIP_General_Error_To_String(error_string+strlen(error_string));
	EIPPLC_Throw_Exception_String(env,obj,function_name,error_string);
}

/**
 * This routine throws an exception of class ngat/eip/EIPNativeException. 
 * This is used to report all libeip error messages back to the Java layer.
 * @param env The JNI environment pointer.
 * @param obj The instance of EIPPLC that threw the error.
 * @param function_name The name of the function in which this exception is being generated for.
 * @param error_string The string to pass to the constructor of the exception.
 */
static void EIPPLC_Throw_Exception_String(JNIEnv *env,jobject obj,char *function_name,char *error_string)
{
	jclass exception_class = NULL;
	jobject exception_instance = NULL;
	jstring error_jstring = NULL;
	jmethodID mid;
	int retval;

	exception_class = (*env)->FindClass(env,"ngat/eip/EIPNativeException");
	if(exception_class != NULL)
	{
	/* get EIPNativeException constructor */
		mid = (*env)->GetMethodID(env,exception_class,"<init>",
					  "(Ljava/lang/String;Lngat/eip/EIPPLC;)V");
		if(mid == 0)
		{
			/* One of the following exceptions has been thrown:
			** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
			fprintf(stderr,"EIPPLC_Throw_Exception_String:GetMethodID failed:%s:%s\n",function_name,
				error_string);
			return;
		}
	/* convert error_string to JString */
		error_jstring = (*env)->NewStringUTF(env,error_string);
	/* call constructor */
		exception_instance = (*env)->NewObject(env,exception_class,mid,error_jstring,obj);
		if(exception_instance == NULL)
		{
			/* One of the following exceptions has been thrown:
			** InstantiationException, OutOfMemoryError */
			fprintf(stderr,"EIPPLC_Throw_Exception_String:NewObject failed %s:%s\n",
				function_name,error_string);
			return;
		}
	/* throw instance */
		retval = (*env)->Throw(env,(jthrowable)exception_instance);
		if(retval !=0)
		{
			fprintf(stderr,"EIPPLC_Throw_Exception_String:Throw failed %d:%s:%s\n",retval,
				function_name,error_string);
		}
	}
	else
	{
		fprintf(stderr,"EIPPLC_Throw_Exception_String:FindClass failed:%s:%s\n",function_name,
			error_string);
	}
}

/**
 * libeip Log Handler for the Java layer interface. 
 * This calls the ngat.eip.EIPPLC logger's 
 * log(int level,String clazz,String source,String message) method with the parameters supplied to this routine.
 * If the logger instance is NULL, or the log_method_id is NULL the call is not made.
 * Otherwise, A java.lang.String instance is constructed from the string parameter,
 * and the JNI CallVoidMethod routine called to call log().
 * @param level The log level of the message.
 * @param string The message to log.
 * @see #java_vm
 * @see #logger
 * @see #log_method_id
 */
static void EIPPLC_Log_Handler(char *class,char *source,int level,char *string)
{
	JNIEnv *env = NULL;
	jstring java_string = NULL;
	jstring java_class = NULL;
	jstring java_source = NULL;

	if(logger == NULL)
	{
		fprintf(stderr,"EIPPLC_Log_Handler:logger was NULL (%d,%s).\n",level,string);
		return;
	}
	if(log_method_id == NULL)
	{
		fprintf(stderr,"EIPPLC_Log_Handler:log_method_id was NULL (%d,%s).\n",level,string);
		return;
	}
	if(java_vm == NULL)
	{
		fprintf(stderr,"EIPPLC_Log_Handler:java_vm was NULL (%d,%s).\n",level,string);
		return;
	}
/* get java env for this thread */
	(*java_vm)->AttachCurrentThread(java_vm,(void**)&env,NULL);
	if(env == NULL)
	{
		fprintf(stderr,"EIPPLC_Log_Handler:env was NULL (%d,%s).\n",level,string);
		return;
	}
	if(string == NULL)
	{
		fprintf(stderr,"EIPPLC_Log_Handler:string (%d) was NULL.\n",level);
		return;
	}
/* convert C to Java String */
	java_string = (*env)->NewStringUTF(env,string);
	if(class != NULL)
		java_class = (*env)->NewStringUTF(env,class);
	else
		java_class = (*env)->NewStringUTF(env,"-");
	if(source != NULL)
		java_source = (*env)->NewStringUTF(env,source);
	else
		java_source = (*env)->NewStringUTF(env,"-");
/* call log method on logger instance */
	(*env)->CallVoidMethod(env,logger,log_method_id,(jint)level,java_class,java_source,java_string);
}

/**
 * Routine to add a mapping from the Java EIPHandle j_handle instance to the  C EIP_Handle_T
 * c_handle, in the Handle_Map_List.
 * @param instance The EIPPLC instance.
 * @param j_handle The Java EIPHandle instance.
 * @param c_handle The C EIP_Handle_T instance.
 * @return The routine returns TRUE if the map is added (or updated), FALSE if there was no room left
 *         in the mapping list. 
 *         EIPPLC_Throw_Exception_String is used to throw a Java exception if the routine returns FALSE.
 * @see #HANDLE_MAP_SIZE
 * @see #Handle_Map_List
 * @see #EIPPLC_Throw_Exception_String
 */
static int EIPPLC_Handle_Map_Add(JNIEnv *env,jobject instance,jobject j_handle,EIP_Handle_T* c_handle)
{
	int i,done;
	jobject global_j_handle = NULL;

	/* does the map already exist? */
	i = 0;
	done = FALSE;
	while((i < HANDLE_MAP_SIZE)&&(done == FALSE))
	{
		if((*env)->IsSameObject(env,Handle_Map_List[i].Java_Handle,j_handle))
			done = TRUE;
		else
			i++;
	}
	if(done == TRUE)/* found an existing interface handle for this EIPPLC instance */
	{
		/* update handle */
		Handle_Map_List[i].C_Handle = c_handle;
	}
	else
	{
		/* look for a blank index to put the map */
		i = 0;
		done = FALSE;
		while((i < HANDLE_MAP_SIZE)&&(done == FALSE))
		{
			if(Handle_Map_List[i].Java_Handle == NULL)
				done = TRUE;
			else
				i++;
		}
		if(done == FALSE)
		{
			EIPPLC_Throw_Exception_String(env,instance,"EIPPLC_Handle_Map_Add",
							  "No empty slots in handle map.");
			return FALSE;
		}
		/* index i is free, add handle map here */
		global_j_handle = (*env)->NewGlobalRef(env,j_handle);
		if(global_j_handle == NULL)
		{
			EIPPLC_Throw_Exception_String(env,instance,"EIPPLC_Handle_Map_Add",
							  "Failed to create Global reference of j_handle.");
			return FALSE;
		}
		fprintf(stdout,"EIPPLC_Handle_Map_Add:Adding instance %p with handle %p at map index %d.\n",
			(void*)global_j_handle,(void*)c_handle,i);
		Handle_Map_List[i].Java_Handle = global_j_handle;
		Handle_Map_List[i].C_Handle = c_handle;
	}
	return TRUE;
}

/**
 * Routine to delete a mapping from the Java EIPHandle j_handle to the C EIP_Handle_T instance, in the Handle_Map_List.
 * @param instance The EIPPLC instance.
 * @param j_handle The Java EIPHandle instance to remove from the list.
 * @return The routine returns TRUE if the map is deleted, FALSE if the mapping could not be found
 *         in the mapping list.
 *         EIPPLC_Throw_Exception_String is used to throw a Java exception if the routine returns FALSE.
 * @see #HANDLE_MAP_SIZE
 * @see #Handle_Map_List
 * @see #EIPPLC_Throw_Exception_String
 */
static int EIPPLC_Handle_Map_Delete(JNIEnv *env,jobject instance,jobject j_handle)
{
	int i,done;

  	/* does the map already exist? */
	i = 0;
	done = FALSE;
	while((i < HANDLE_MAP_SIZE)&&(done == FALSE))
	{
		if((*env)->IsSameObject(env,Handle_Map_List[i].Java_Handle,j_handle))
			done = TRUE;
		else
			i++;
	}
	if(done == FALSE)
	{
		EIPPLC_Throw_Exception_String(env,instance,"EIPPLC_Handle_Map_Delete",
						  "Failed to find EIPHandle instance in handle map.");
		return FALSE;
	}
	/* found an existing interface handle for this EIPPLC instance at index i */
	/* delete this map at index i */
	fprintf(stdout,"EIPPLC_Handle_Map_Delete:Deleting instance %p with handle %p at map index %d.\n",
		(void*)Handle_Map_List[i].Java_Handle,(void*)Handle_Map_List[i].C_Handle,i);
	(*env)->DeleteGlobalRef(env,Handle_Map_List[i].Java_Handle);
	Handle_Map_List[i].Java_Handle = NULL;
	Handle_Map_List[i].C_Handle = NULL;
	return TRUE;
}

/**
 * Routine to find a mapping from the Java EIPHandle instance to the C EIP_Handle_T, in the Handle_Map_List.
 * @param instance The EIPPLC instance.
 * @param j_handle The Java EIPHandle instance to search for.
 * @param c_handle The address of an interface handle, to fill with the interface handle for
 *        this EIP_Handle_T instance, if one is successfully found.
 * @return The routine returns TRUE if the mapping is found and returned, FALSE if there was no mapping
 *         for this EIPHandle instance, or the c_handle pointer was NULL.
 *         EIPPLC_Throw_Exception_String is used to throw a Java exception if the routine returns FALSE.
 * @see #HANDLE_MAP_SIZE
 * @see #Handle_Map_List
 * @see #EIPPLC_Throw_Exception_String
 */
static int EIPPLC_Handle_Map_Find(JNIEnv *env,jobject instance,jobject j_handle,EIP_Handle_T** c_handle)
{
	int i,done;

	if(j_handle == NULL)
	{
		EIPPLC_Throw_Exception_String(env,instance,"EIPPLC_Handle_Map_Find",
						  "Java EIPHandle handle was NULL.");
		return FALSE;
	}
	i = 0;
	done = FALSE;
	while((i < HANDLE_MAP_SIZE)&&(done == FALSE))
	{
		if((*env)->IsSameObject(env,Handle_Map_List[i].Java_Handle,j_handle))
			done = TRUE;
		else
			i++;
	}
	if(done == FALSE)
	{
		fprintf(stdout,"EIPPLC_Handle_Map_Find:Failed to find Java Handle %p.\n",(void*)j_handle);
		EIPPLC_Throw_Exception_String(env,instance,"EIPPLC_Handle_Map_Find",
						  "Java EIPHandle handle was not found.");
		return FALSE;
	}
	(*c_handle) = Handle_Map_List[i].C_Handle;
	return TRUE;
}

/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2009/02/05 11:36:18  cjm
** Swapped Bitwise for Absolute logging levels.
**
** Revision 1.1  2008/10/15 13:48:23  cjm
** Initial revision
**
*/

