#include "stdafx.h"
#include "scardexception.h"
#include "winscarderr.h"

using namespace std;

std::map<long, wstring> SCardException::errorMessages(
{ { Smartcards::SUCCESS,L"No error was encountered." },
  { Smartcards::INTERNAL_ERROR,L"An internal consistency check failed."},
  { Smartcards::INVALID_HANDLE,L"The supplied handle was not valid." },
  { Smartcards::INVALID_PARAMETER,L"One or more of the supplied parameters could not be properly interpreted." },
  { Smartcards::CANCELLED,L"The action was canceled by an SCardCancel request." },
  { Smartcards::INVALID_TARGET,L"Registry startup information is missing or not valid." },
  { Smartcards::NO_MEMORY,L"Not enough memory available to complete this command." },
  { Smartcards::WAITED_TOO_LONG,L"An internal consistency timer has expired." },
  { Smartcards::INSUFFICIENT_BUFFER,L"The data buffer for returned data is too small for the returned data." },
  { Smartcards::UNKNOWN_READER,L"The specified reader name is not recognized." },
  { Smartcards::TIMEOUT,L"The user-specified time-out value has expired." },
  { Smartcards::SHARING_VIOLATION,L"The smart card cannot be accessed because of other outstanding connections." },
  { Smartcards::NO_SMARTCARD,L"The operation requires a smart card, but no smart card is currently in the device." },
  { Smartcards::UNKNOWN_CARD,L"The specified smart card name is not recognized." },
  { Smartcards::CANT_DISPOSE,L"The system could not dispose of the media in the requested manner." },
  { Smartcards::PROTO_MISMATCH,L"The requested protocols are incompatible with the protocol currently in use with the card." },
  { Smartcards::NOT_READY,L"The reader or card is not ready to accept commands." },
  { Smartcards::INVALID_VALUE,L"One or more of the supplied parameter values could not be properly interpreted." },
  { Smartcards::SYSTEM_CANCELLED,L"The action was canceled by the system, presumably to log off or shut down." },
  { Smartcards::COMM_ERROR,L"An internal communications error has been detected." },
  { Smartcards::UNKNOWN_ERROR,L"An internal error has been detected, but the source is unknown." },
  { Smartcards::INVALID_ATR,L"An ATR L obtained from the registry is not a valid ATR L." },
  { Smartcards::NOT_TRANSACTED,L"An attempt was made to end a nonexistent transaction." },
  { Smartcards::READER_UNAVAILABLE,L"The specified reader is not currently available for use." },
  { Smartcards::SHUTDOWN,L"The operation has been aborted to allow the server application to exit." },
  { Smartcards::PCI_TOO_SMALL,L"The PCI receive buffer was too small." },
  { Smartcards::READER_UNSUPPORTED,L"The reader driver does not meet minimal requirements for support." },
  { Smartcards::DUPLICATE_READER,L"The reader driver did not produce a unique reader name." },
  { Smartcards::CARD_UNSUPPORTED,L"The smart card does not meet minimal requirements for support." },
  { Smartcards::NO_SERVICE,L"The Smart Card Resource Manager is not running." },
  { Smartcards::CARD_SERVICE_STOPPED,L"The Smart Card Resource Manager has shut down." },
  { Smartcards::UNEXPECTED,L"An unexpected card error has occurred." },
  { Smartcards::ICC_INSTALLATION,L"No primary provider can be found for the smart card." },
  { Smartcards::ICC_CREATEORDER,L"The requested order of object creation is not supported." },
  { Smartcards::UNSUPPORTED_FEATURE,L"This smart card does not support the requested feature." },
  { Smartcards::DIR_NOT_FOUND,L"The specified directory does not exist in the smart card." },
  { Smartcards::FILE_NOT_FOUND,L"The specified file does not exist in the smart card." },
  { Smartcards::NO_DIR,L"The supplied path does not represent a smart card directory." },
  { Smartcards::NO_FILE,L"The supplied path does not represent a smart card file." },
  { Smartcards::NO_ACCESS,L"Access is denied to the file." },
  { Smartcards::WRITE_TOO_MANY,L"An attempt was made to write more data than would fit in the target object." },
  { Smartcards::BAD_SEEK,L"An error occurred in setting the smart card file object pointer." },
  { Smartcards::INVALID_CHV,L"The supplied PIN is incorrect." },
  { Smartcards::UNKNOWN_RES_MNG,L"An unrecognized error code was returned." },
  { Smartcards::NO_SUCH_CERTIFICATE,L"The requested certificate does not exist." },
  { Smartcards::CERTIFICATE_UNAVAILABLE,L"The requested certificate could not be obtained." },
  { Smartcards::NO_READERS_AVAILABLE,L"Cannot find a smart card reader." },
  { Smartcards::COMM_DATA_LOST,L"A communications error with the smart card has been detected." },
  { Smartcards::NO_KEY_CONTAINER,L"The requested key container does not exist on the smart card." },
  { Smartcards::SERVER_TOO_BUSY,L"The Smart Card Resource Manager is too busy to complete this operation." },
  { Smartcards::PIN_CACHE_EXPIRED,L"The smart card PIN cache has expired." },
  { Smartcards::NO_PIN_CACHE,L"The smart card PIN cannot be cached." },
  { Smartcards::READ_ONLY_CARD,L"The smart card is read only and cannot be written to." },
  { Smartcards::UNSUPPORTED_CARD,L"The reader cannot communicate with the card, due to ATR string configuration conflicts." },
  { Smartcards::UNRESPONSIVE_CARD,L"The smart card is not responding to a reset." },
  { Smartcards::UNPOWERED_CARD,L"Power has been removed from the smart card, so that further communication is not possible." },
  { Smartcards::RESET_CARD,L"The smart card was reset." },
  { Smartcards::REMOVED_CARD,L"The smart card has been removed, so further communication is not possible." },
  { Smartcards::SECURITY_VIOLATION,L"Access was denied because of a security violation." },
  { Smartcards::WRONG_CHV,L"The card cannot be accessed because the wrong PIN was presented." },
  { Smartcards::CHV_BLOCKED,L"The card cannot be accessed because the maximum number of PIN entry attempts has been reached." },
  { Smartcards::CARD_EOF,L"The end of the smart card file has been reached." },
  { Smartcards::CANCELLED_BY_USER,L"The action was canceled by the user." },
  { Smartcards::CARD_NOT_AUTHENTICATED,L"No PIN was presented to the smart card." },
  { Smartcards::CACHE_ITEM_NOT_FOUND,L"The requested item could not be found in the cache." },
  { Smartcards::CACHE_ITEM_STALE,L"The requested cache item is too old and was deleted from the cache." },
  { Smartcards::CACHE_ITEM_TOO_BIG,L"The new cache item exceeds the maximum per-item size defined for the cache." },
}
);

SCardException::SCardException() noexcept
 :m_errorCode(Smartcards::UNEXPECTED)
{

}

SCardException::SCardException(const wchar_t * const & _What) noexcept
 : m_errorCode(Smartcards::UNEXPECTED)
{
 setErrorString(_What);
}

SCardException::SCardException(const SCardException& _That) noexcept
{
 *this = _That;
}

SCardException::SCardException(const std::wstring& _What) noexcept
 : m_errorCode(Smartcards::UNEXPECTED)
{
 setErrorString(_What);
}

SCardException::SCardException(long code) noexcept
{
 wstring errorString;
 errorString.assign(L"WinSCard Error:");
 errorString.append(getMessage(code));
 errorString.append(L":");
 errorString.append(to_wstring(code));
 setErrorString(errorString);
 setErrorCode(code);
}

SCardException::~SCardException() noexcept
{

}

SCardException& SCardException::operator=(const SCardException& _That)
{
 m_errorString = _That.m_errorString;
 m_errorStringA = _That.m_errorStringA;
 m_errorCode = _That.m_errorCode;
 return *this;
}

const char* SCardException::what() const
{
 return m_errorStringA.c_str();
}

wstring SCardException::errorString(void) const
{
 return m_errorString.empty() ? L"Unknown exception" : m_errorString;
}

std::string SCardException::errorStringA() const
{
 return m_errorStringA.empty() ? "Unknown exception" : m_errorStringA;
}

void SCardException::setErrorString(const wstring& errStr)
{
 m_errorString = errStr;
 std::vector<char> errorMsg;
 size_t size= m_errorString.size()+1;
 errorMsg.resize(size);
 errno_t err = wcstombs_s(&size, &errorMsg[0], size, m_errorString.c_str(), size);
 if(err!=0)
 {
  m_errorStringA = "";
  return;
 }
 m_errorStringA.assign(&errorMsg[0], size);
}

void SCardException::setErrorString(const std::string& errStr)
{
 m_errorStringA = errStr;
 std::vector<wchar_t> errorMsg;
 size_t size;
 errorMsg.resize(m_errorString.size() + 1);
 errno_t err = mbstowcs_s(&size, &errorMsg[0], m_errorStringA.size(), m_errorStringA.c_str(), m_errorStringA.size());
 if (err != 0)
 {
  m_errorString = L"";
  return;
 }
 m_errorString.assign(&errorMsg[0], size);
}

long SCardException::errorCode(void) const
{
 return m_errorCode;
}

void SCardException::setErrorCode(long code)
{
 m_errorCode = code;
}

std::wstring SCardException::getMessage(long code)
{
 if(errorMessages.find(code)!=errorMessages.end())
  return errorMessages.at(code);
#ifdef WIN32
 wchar_t *err=nullptr;
 DWORD len = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(code), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 0, nullptr);
 if(len==0)
  return L"Unexpected error.";
 wstring errMsg(err, len);
 LocalFree(err);
 return errMsg;
#else
 return L"Unexpected error.";
#endif
}