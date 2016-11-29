//! \file SCardException.h
//! \brief Header file for SCardException class.
//! \details File is contains class SCardException must implement a card exception based on WinSCard errors
#ifndef SCARDEXCEPTION_H
#define SCARDEXCEPTION_H

#include "stdafx.h"
//! \brief This class must implement a card exception based on WinSCard errors
class SCardException : public std::exception
{
public:
  //! \fn SCardException::SCardException() noexcept
  //! \brief Default constructor.
  SCardException() noexcept;
  //! \fn SCardException::SCardException(const SCardException&) noexcept
  //! \brief copy constructor.
  //! \param[in] _That The other SCardException object.
  SCardException(const SCardException&) noexcept;
  //! \fn SCardException::SCardException( const wchar_t * const &) noexcept
  //! \brief unicode c-string constructor.
  //! \param[in] _What unicode c-string exception message
  explicit SCardException(const wchar_t * const &) noexcept;
  //! \fn SCardException::SCardException( const std::wstring&) noexcept
  //! \brief wstring constructor.
  //! \param[in] _What wstring exception message
  explicit SCardException(const std::wstring&) noexcept;
  //! \fn SCardException::SCardException(long code) noexcept
  //! \brief error code constructor.
  //! \param[in] code long error code
  explicit SCardException(long code) noexcept;
  //! \fn SCardException::~SCardException() noexcept
  //! \brief Destructor.
  virtual ~SCardException() noexcept;
  //! \fn SCardException& SCardException::operator=( const SCardException&)
  //! \brief Assignment operator.
  //! \param[in] _That The other SCardException object.
  //! \return A shallow copy of this object.
  SCardException&  operator=(const SCardException&);
  //! \fn const char* SCardException::what() const noexcept
  //! \brief override std::exception function what. Get c-string exception message
  //! \return "Unknown exception" if it fails, else a pointer to a c-string exception message
  const char* what() const noexcept;
  //! \fn std::wstring errorString(void) const
  //! \brief Get wstring exception message
  //! \return "Unknown exception" if it fails, else a wstring exception message
  std::wstring errorString(void) const;
  //! \fn std::string errorString(void) const
  //! \brief Get string exception message
  //! \return "Unknown exception" if it fails, else a string exception message
  std::string errorStringA(void) const;
  //! \fn void SCardException::setErrorString( const std::wstring& errStr)
  //! \brief Sets exception message wstring
  //! \param[in] errStr The exception message wstring
  void setErrorString(const std::wstring& errStr);
  //! \fn void SCardException::setErrorString( const std::string& errStr)
  //! \brief Sets exception message string
  //! \param[in] errStr The exception message string
  void setErrorString(const std::string& errStr);
  //! \fn long SCardException::errorCode( void ) const
  //! \brief Get error code
  //! \return A long error code
  long errorCode(void) const;
  //! \fn void SCardException::setErrorCode( long code )
  //! \brief Sets error code.
  //! \param[in] code The long error code
  void setErrorCode(long code);
  //! \fn std::wstring getMessage(long code)
  //! \brief static function returns wstring message from winscard long error code
  //! \param[in] code long error code    
  //! \return wstring error message from errorMessages map or message from winAPI FormatMessage function
  static std::wstring getMessage(long code);
private:
 static std::map<long,std::wstring> errorMessages;//!< map contains error messages to winscard codes
 std::wstring m_errorString;//!<The exception message wstring
 std::string m_errorStringA;//!<The exception message string
 long m_errorCode;//!<The long error code
};

#endif //SCARDEXCEPTION_H
