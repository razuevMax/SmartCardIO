#ifndef WINSCARDERR_H
#define WINSCARDERR_H

#if defined(WIN32) || defined(_WIN32)
#include "winerror.h"
#elif defined(__linux__) || defined(__linux)
#include "pcsclite.h"
#endif

namespace Smartcards
{
 
 enum FACILITY_CODES:DWORD
 {
  SYSTEM_FACILITY,
  SCARD_FACILITY=FACILITY_SCARD
 };

 enum ERRCODES : long
 {
 SUCCESS=SCARD_S_SUCCESS,
 INTERNAL_ERROR=SCARD_F_INTERNAL_ERROR,//0x80100001
 CANCELLED=SCARD_E_CANCELLED,//0x80100002
 INVALID_HANDLE=SCARD_E_INVALID_HANDLE,//0x80100003
 INVALID_PARAMETER=SCARD_E_INVALID_PARAMETER,//0x80100004
 INVALID_TARGET=SCARD_E_INVALID_TARGET,//0x80100005
 NO_MEMORY=SCARD_E_NO_MEMORY,//0x80100006
 WAITED_TOO_LONG=SCARD_F_WAITED_TOO_LONG,//0x80100007
 INSUFFICIENT_BUFFER=SCARD_E_INSUFFICIENT_BUFFER,//0x80100008
 UNKNOWN_READER=SCARD_E_UNKNOWN_READER,//0x80100009
 TIMEOUT=SCARD_E_TIMEOUT,//0x8010000A
 SHARING_VIOLATION=SCARD_E_SHARING_VIOLATION,//0x8010000B
 NO_SMARTCARD=SCARD_E_NO_SMARTCARD,//0x8010000C
 UNKNOWN_CARD=SCARD_E_UNKNOWN_CARD,//0x8010000D
 CANT_DISPOSE=SCARD_E_CANT_DISPOSE,//0x8010000E
 PROTO_MISMATCH=SCARD_E_PROTO_MISMATCH,//0x8010000F
 NOT_READY=SCARD_E_NOT_READY,//0x80100010
 INVALID_VALUE=SCARD_E_INVALID_VALUE,//0x80100011
 SYSTEM_CANCELLED=SCARD_E_SYSTEM_CANCELLED,//0x80100012
 COMM_ERROR=SCARD_F_COMM_ERROR,//0x80100013
 UNKNOWN_ERROR=SCARD_F_UNKNOWN_ERROR,//0x80100014
 INVALID_ATR=SCARD_E_INVALID_ATR,//0x80100015
 NOT_TRANSACTED=SCARD_E_NOT_TRANSACTED,//0x80100016
 READER_UNAVAILABLE=SCARD_E_READER_UNAVAILABLE,//0x80100017
 SHUTDOWN=SCARD_P_SHUTDOWN,//0x80100018
 PCI_TOO_SMALL=SCARD_E_PCI_TOO_SMALL,//0x80100019
 READER_UNSUPPORTED=SCARD_E_READER_UNSUPPORTED,//0x8010001A
 DUPLICATE_READER=SCARD_E_DUPLICATE_READER,//0x8010001B
 CARD_UNSUPPORTED=SCARD_E_CARD_UNSUPPORTED,//0x8010001C
 NO_SERVICE=SCARD_E_NO_SERVICE,//0x8010001D
 CARD_SERVICE_STOPPED=SCARD_E_SERVICE_STOPPED,//0x8010001E
 UNEXPECTED=SCARD_E_UNEXPECTED,//0x8010001F
 ICC_INSTALLATION=SCARD_E_ICC_INSTALLATION,//0x80100020
 ICC_CREATEORDER=SCARD_E_ICC_CREATEORDER,//0x80100021
 UNSUPPORTED_FEATURE=SCARD_E_UNSUPPORTED_FEATURE,//0x80100022
 DIR_NOT_FOUND=SCARD_E_DIR_NOT_FOUND,//0x80100023
 FILE_NOT_FOUND=SCARD_E_FILE_NOT_FOUND,//0x80100024
 NO_DIR=SCARD_E_NO_DIR,//0x80100025
 NO_FILE=SCARD_E_NO_FILE,//0x80100026
 NO_ACCESS=SCARD_E_NO_ACCESS,//0x80100027
 WRITE_TOO_MANY=SCARD_E_WRITE_TOO_MANY,//0x80100028
 BAD_SEEK=SCARD_E_BAD_SEEK,//0x80100029
 INVALID_CHV=SCARD_E_INVALID_CHV,//0x8010002A
 UNKNOWN_RES_MNG =SCARD_E_UNKNOWN_RES_MNG,//0x8010002B
 NO_SUCH_CERTIFICATE =SCARD_E_NO_SUCH_CERTIFICATE,//0x8010002C
 CERTIFICATE_UNAVAILABLE =SCARD_E_CERTIFICATE_UNAVAILABLE,//0x8010002D
 NO_READERS_AVAILABLE =SCARD_E_NO_READERS_AVAILABLE,//0x8010002E
 COMM_DATA_LOST =SCARD_E_COMM_DATA_LOST,//0x8010002F
 NO_KEY_CONTAINER =SCARD_E_NO_KEY_CONTAINER,//0x80100030
 SERVER_TOO_BUSY =SCARD_E_SERVER_TOO_BUSY,//0x80100031
 UNSUPPORTED_CARD =SCARD_W_UNSUPPORTED_CARD,//0x80100065
 UNRESPONSIVE_CARD =SCARD_W_UNRESPONSIVE_CARD,//0x80100066
 UNPOWERED_CARD =SCARD_W_UNPOWERED_CARD,//0x80100067
 RESET_CARD =SCARD_W_RESET_CARD,//0x80100068
 REMOVED_CARD =SCARD_W_REMOVED_CARD,//0x80100069
 SECURITY_VIOLATION =SCARD_W_SECURITY_VIOLATION,//0x8010006A
 WRONG_CHV =SCARD_W_WRONG_CHV,//0x8010006B
 CHV_BLOCKED =SCARD_W_CHV_BLOCKED,//0x8010006C
 CARD_EOF =SCARD_W_EOF,//0x8010006D
 CANCELLED_BY_USER =SCARD_W_CANCELLED_BY_USER,//0x8010006E
 CARD_NOT_AUTHENTICATED =SCARD_W_CARD_NOT_AUTHENTICATED,//0x8010006F
 CACHE_ITEM_NOT_FOUND =SCARD_W_CACHE_ITEM_NOT_FOUND,//0x80100070
 CACHE_ITEM_STALE =SCARD_W_CACHE_ITEM_STALE,//0x80100071
#if _MSC_VER>1700
 PIN_CACHE_EXPIRED =SCARD_E_PIN_CACHE_EXPIRED,//0x80100032
 NO_PIN_CACHE =SCARD_E_NO_PIN_CACHE,//0x80100033
 READ_ONLY_CARD =SCARD_E_READ_ONLY_CARD,//0x80100034
 CACHE_ITEM_TOO_BIG =SCARD_W_CACHE_ITEM_TOO_BIG,//0x80100072
#endif //Standard codes
 SYSTEM_INVALID_HANDLE = ERROR_INVALID_HANDLE,//0x6
 SYSTEM_NOT_SUPPORTED = ERROR_NOT_SUPPORTED//0x80070032
};

}//namespace Smartcards

#endif
