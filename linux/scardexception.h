#ifndef SCARDEXCEPTION_H
#define SCARDEXCEPTION_H

#include "stdafx.h"

class SCardException : public std::exception
{
	public:
  SCardException() noexcept;
  SCardException(const SCardException&) noexcept;
  explicit SCardException(const wchar_t * const &) noexcept;
  explicit SCardException(const std::wstring&) noexcept;
  explicit SCardException(long code) noexcept;
  virtual ~SCardException() noexcept;
  SCardException&  operator=(const SCardException&);
  const char* what() const override;
  std::wstring errorString(void) const;
  std::string errorStringA(void) const;
  void setErrorString(const std::wstring&);
  void setErrorString(const std::string&);
  long errorCode(void) const;
  void setErrorCode(long code);
  static std::wstring getMessage(long code);
private:
 static std::map<long,std::wstring> errorMessages;
 std::wstring m_errorString;
 std::string m_errorStringA;
 long m_errorCode;
};

#endif //SCARDEXCEPTION_H