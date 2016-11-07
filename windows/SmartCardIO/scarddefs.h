#ifndef SCARDDEFS_H
#define SCARDDEFS_H
#include "stdafx.h"

#ifndef SCARD_E_NO_READERS_AVAILABLE
#define SCARD_E_NO_READERS_AVAILABLE				((DWORD)0x8010002EL)
//
// MessageId: SCARD_E_NO_READERS_AVAILABLE
//
// MessageText:
//
//   Cannot find a smart card reader.
#endif

#ifndef SCARD_E_NO_RESRC_MNGR_RUNNING
#define SCARD_E_NO_RESRC_MNGR_RUNNING				((DWORD)0x8010001DL)
//
// MessageId: SCARD_E_NO_RESRC_MNGR_RUNNING
//
// MessageText:
//
//   The Smart card resource manager is not running.
#endif

#ifndef SCARD_W_WRONG_CHV
#define SCARD_W_WRONG_CHV							((DWORD) 0x8010006BL) 
//
// MessageId: SCARD_W_WRONG_CHV
//
// MessageText:
//
//   The card cannot be accessed because the wrong PIN was presented. 
#endif

#ifndef SCARD_W_CHV_BLOCKED
#define SCARD_W_CHV_BLOCKED							((DWORD)0x8010006CL)
//
// MessageId: SCARD_W_CHV_BLOCKED
//
// MessageText:
//
//   The card cannot be accessed because the maximum number of PIN entry attempts has been reached.  
#endif

#ifndef SCARD_E_BAD_SEEK
#define SCARD_E_BAD_SEEK							((DWORD)0x80100029L)
//
// MessageId: SCARD_E_BAD_SEEK
//
// MessageText:
//
// There was an error trying to set the smart card file object pointer.
#endif

#ifndef SCARD_E_CERTIFICATE_UNAVAILABLE
#define SCARD_E_CERTIFICATE_UNAVAILABLE				((DWORD)0x8010002DL)
//
// MessageId: SCARD_E_CERTIFICATE_UNAVAILABLE
//
// MessageText:
//
// The requested certificate could not be obtained.
#endif

#ifndef SCARD_E_COMM_DATA_LOST
#define SCARD_E_COMM_DATA_LOST						((DWORD)0x8010002FL)
//
// MessageId: SCARD_E_COMM_DATA_LOST
//
// MessageText:
//
// A communications error with the smart card has been detected.  Retry the operation.
#endif

#ifndef SCARD_E_DIR_NOT_FOUND
#define SCARD_E_DIR_NOT_FOUND						((DWORD)0x80100023L)
//
// MessageId: SCARD_E_DIR_NOT_FOUND
//
// MessageText:
//
// The identified directory does not exist in the smart card.
#endif

#ifndef SCARD_E_FILE_NOT_FOUND
#define SCARD_E_FILE_NOT_FOUND						((DWORD)0x80100024L)
//
// MessageId: SCARD_E_FILE_NOT_FOUND
//
// MessageText:
//
// The identified file does not exist in the smart card.
#endif

#ifndef SCARD_E_ICC_CREATEORDER
#define SCARD_E_ICC_CREATEORDER						((DWORD)0x80100021L)
//
// MessageId: SCARD_E_ICC_CREATEORDER
//
// MessageText:
//
// The requested order of object creation is not supported.
#endif

#ifndef SCARD_E_ICC_INSTALLATION
#define SCARD_E_ICC_INSTALLATION					((DWORD)0x80100020L)
//
// MessageId: SCARD_E_ICC_INSTALLATION
//
// MessageText:
//
// No Primary Provider can be found for the smart card.
#endif

#ifndef SCARD_E_INVALID_CHV
#define SCARD_E_INVALID_CHV							((DWORD)0x8010002AL)
//
// MessageId: SCARD_E_INVALID_CHV
//
// MessageText:
//
// The supplied PIN is incorrect.
#endif

#ifndef SCARD_E_NO_ACCESS
#define SCARD_E_NO_ACCESS							((DWORD)0x80100027L)
//
// MessageId: SCARD_E_NO_ACCESS
//
// MessageText:
//
// Access is denied to this file.
#endif

#ifndef SCARD_E_NO_DIR
#define SCARD_E_NO_DIR								((DWORD)0x80100025L)
//
// MessageId: SCARD_E_NO_DIR
//
// MessageText:
//
// The supplied path does not represent a smart card directory.
#endif

#ifndef SCARD_E_NO_FILE
#define SCARD_E_NO_FILE								((DWORD)0x80100026L)
//
// MessageId: SCARD_E_NO_FILE
//
// MessageText:
//
// The supplied path does not represent a smart card file.
#endif

#ifndef SCARD_E_NO_SUCH_CERTIFICATE
#define SCARD_E_NO_SUCH_CERTIFICATE					((DWORD)0x8010002CL)
//
// MessageId: SCARD_E_NO_SUCH_CERTIFICATE
//
// MessageText:
//
// The requested certificate does not exist.
#endif

#ifndef SCARD_E_UNEXPECTED
#define SCARD_E_UNEXPECTED							((DWORD)0x8010001FL)
//
// MessageId: SCARD_E_UNEXPECTED
//
// MessageText:
//
// An unexpected card error has occurred.
#endif

#ifndef SCARD_E_UNKNOWN_RES_MNG
#define SCARD_E_UNKNOWN_RES_MNG						((DWORD)0x8010002BL)
//
// MessageId: SCARD_E_UNKNOWN_RES_MNG
//
// MessageText:
//
// An unrecognized error code was returned from a layered component.
#endif

#ifndef SCARD_E_UNSUPPORTED_FEATURE
#define SCARD_E_UNSUPPORTED_FEATURE					((DWORD)0x80100022L)
//
// MessageId: SCARD_E_UNSUPPORTED_FEATURE
//
// MessageText:
//
// This smart card does not support the requested feature.
#endif

#ifndef SCARD_E_WRITE_TOO_MANY
#define SCARD_E_WRITE_TOO_MANY						((DWORD)0x80100028L)
//
// MessageId: SCARD_E_WRITE_TOO_MANY
//
// MessageText:
//
// The smartcard does not have enough memory to store the information.
#endif

#ifndef SCARD_W_CANCELLED_BY_USER
#define SCARD_W_CANCELLED_BY_USER					((DWORD)0x8010006EL)
//
// MessageId: SCARD_W_CANCELLED_BY_USER
//
// MessageText:
//
// The action was cancelled by the user.
#endif

#ifndef SCARD_W_EOF
#define SCARD_W_EOF									((DWORD)0x8010006DL)
//
// MessageId: SCARD_W_EOF
//
// MessageText:
//
// The end of the smart card file has been reached.
#endif

#ifndef SCARD_W_SECURITY_VIOLATION
#define SCARD_W_SECURITY_VIOLATION					((DWORD)0x8010006AL)
//
// MessageId: SCARD_W_SECURITY_VIOLATION
//
// MessageText:
//
// Access was denied because of a security violation.
#endif

#endif //SCARDDEFS_H