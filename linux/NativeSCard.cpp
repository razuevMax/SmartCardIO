#include "stdafx.h"
#include "NativeSCard.h"
#include "scardexception.h"
#include <winscard.h>

using namespace Smartcards;

ReadersStates::ReadersStates()
{
}

ReadersStates::ReadersStates(const std::initializer_list<std::pair<std::wstring, DWORD>>& ilist)
{
 for (auto& item : ilist)
  add(item.first, item.second);
}

ReadersStates::ReadersStates(PSCARD_READERSTATEW pReaderStates, DWORD cbReaderStates)
 :mReaderStates(pReaderStates,pReaderStates+cbReaderStates)
{
 for(DWORD idx=0;idx<cbReaderStates;idx++)
 {
  std::wstring readerName(mReaderStates[idx].szReader);
  mReaderStatesMap[readerName] = idx;
 }
}

ReadersStates::ReadersStates(const std::vector<SCARD_READERSTATEW>& readerStates)
 :mReaderStates(readerStates)
{
 DWORD cbReaderStates = readerStates.size();
 for (DWORD idx = 0; idx<cbReaderStates; idx++)
 {
  std::wstring readerName(mReaderStates[idx].szReader);
  mReaderStatesMap[readerName] = idx;;
 }
}

ReadersStates::~ReadersStates()
{
}

void ReadersStates::addNewReader()
{
 addDefault(L"\\\\?PnP?\\Notification");
}

void ReadersStates::removeNewReader()
{
 remove(L"\\\\?PnP?\\Notification");
}

DWORD ReadersStates::currentState(const std::wstring& readerName)
{
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  addDefault(readerName);
 return mReaderStates.at(mReaderStatesMap.at(readerName)).dwCurrentState;
}

DWORD ReadersStates::eventState(const std::wstring& readerName)
{
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  addDefault(readerName);
 return mReaderStates.at(mReaderStatesMap.at(readerName)).dwEventState;
}

std::vector<BYTE> ReadersStates::ATR(const std::wstring& readerName)
{
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  addDefault(readerName);
 LPBYTE pAtr = mReaderStates.at(mReaderStatesMap.at(readerName)).rgbAtr;
 DWORD cbAtr= mReaderStates.at(mReaderStatesMap.at(readerName)).cbAtr;
 std::vector<BYTE> Atr(pAtr, pAtr + cbAtr);
 return Atr;
}

LPVOID ReadersStates::userData(const std::wstring& readerName)
{
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  addDefault(readerName);
 return mReaderStates.at(mReaderStatesMap.at(readerName)).pvUserData;
}

void ReadersStates::setCurrentState(const std::wstring& readerName, DWORD state)
{
 DWORD idx;
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  idx=addDefault(readerName);
 else
  idx = mReaderStatesMap.at(readerName);
 mReaderStates.at(idx).dwCurrentState = state;
}

void ReadersStates::setEventState(const std::wstring& readerName, DWORD state)
{
 DWORD idx;
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  idx = addDefault(readerName);
 else
  idx = mReaderStatesMap.at(readerName);
 mReaderStates.at(idx).dwEventState = state;
}

void ReadersStates::setATR(const std::wstring& readerName, const std::vector<BYTE>& ATR)
{
 DWORD idx;
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  idx = addDefault(readerName);
 else
  idx = mReaderStatesMap.at(readerName);
 mReaderStates.at(idx).cbAtr = ATR.size();
 std::copy(ATR.begin(), ATR.end(), mReaderStates.at(idx).rgbAtr);
}

void ReadersStates::setUserData(const std::wstring& readerName, LPVOID userData)
{
 DWORD idx;
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  idx = addDefault(readerName);
 else
  idx = mReaderStatesMap.at(readerName);
 mReaderStates.at(idx).pvUserData = userData;
}

bool ReadersStates::readerPresent(const std::wstring& readerName)
{
 return (mReaderStatesMap.find(readerName) != mReaderStatesMap.end());
}

DWORD ReadersStates::addDefault(const std::wstring& readerName)
{
 SCARD_READERSTATEW stateData;
 stateData.pvUserData = nullptr;
 stateData.dwCurrentState = 0;
 stateData.dwEventState = 0;
 stateData.szReader = readerName.c_str();
 stateData.cbAtr = 0;
 mReaderStates.push_back(stateData);
 DWORD idx = mReaderStates.size() - 1;
 mReaderStatesMap[readerName] = idx;
 return idx;
}

void ReadersStates::add(const std::wstring& readerName, DWORD currentState, LPVOID userData/*=nullptr*/, DWORD eventState/* = 0*/)
{
 add(readerName, currentState, std::vector<BYTE>(),userData,eventState);
}

void ReadersStates::add(const std::wstring& readerName, DWORD currentState, const std::vector<BYTE>& ATR, LPVOID userData/*=nullptr*/, DWORD eventState/* = 0*/)
{
 DWORD idx = 0;
 auto it = mReaderStatesMap.find(readerName);
 if(it!=mReaderStatesMap.end())
 {
  idx = mReaderStatesMap.at(readerName);
  mReaderStatesMap.erase(it);
  mReaderStates.erase(mReaderStates.begin() + static_cast<int>(idx));
 }
 SCARD_READERSTATEW stateData;
 stateData.pvUserData = userData;
 stateData.dwCurrentState = currentState;
 stateData.dwEventState = eventState;
 stateData.szReader = readerName.c_str();
 stateData.cbAtr = ATR.size();
 std::copy(ATR.begin(), ATR.end(), stateData.rgbAtr);
 idx = mReaderStates.size();
 mReaderStates.push_back(stateData);
 mReaderStatesMap[readerName]=idx;
}

bool ReadersStates::remove(const std::wstring& readerName)
{
 auto it = mReaderStatesMap.find(readerName);
 if (it == mReaderStatesMap.end())
  return false;
 DWORD idx = mReaderStatesMap.at(readerName);
 mReaderStatesMap.erase(it);
 mReaderStates.erase(mReaderStates.begin() + static_cast<int>(idx));
 return true;
}

WinSCard::WinSCard(void)
{
}

WinSCard::~WinSCard(void)
{
 m_bThrowingErrors=false;
 if(mContextEstablished)
  ReleaseContext();
}

std::vector<BYTE> WinSCard::GetCardStatus(DWORD& state, DWORD& protocol)
{
 std::vector<BYTE> atr;
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return atr;
 }
 DWORD	pchReaders = AUTOALLOCATE;
 DWORD cByte = 32;
 wchar_t *szListReaders = nullptr;
 atr.resize(32);
 m_nLastError = SCardStatusW(m_hCard, szListReaders, &pchReaders, &state, &protocol, atr.data(), &cByte);
 if (SUCCESS == m_nLastError)
 {
  atr.resize(cByte);
  m_nLastError = SCardFreeMemory(m_hContext, szListReaders);
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return atr;
}

std::vector<std::wstring> WinSCard::ListReaders(void)
{
 std::vector<std::wstring> result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 std::vector<wchar_t> readersV;
 DWORD	pchReaders = 0;
 wchar_t *pReader;
 m_nLastError=SCardListReadersW(m_hContext, nullptr, nullptr,&pchReaders);
 if(SUCCESS==m_nLastError)
 {
  readersV.resize(static_cast<size_t>(pchReaders));
  m_nLastError=SCardListReadersW(m_hContext, nullptr, readersV.data(),&pchReaders);
  if(SUCCESS==m_nLastError)
   {
    pReader=readersV.data();
    while('\0'!=*pReader)
     {
      result.push_back(pReader);
      pReader = pReader + wcslen(pReader) + 1;
     }
   }
 }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

std::vector<std::wstring> WinSCard::ListReaderGroups(void)
{
 std::vector<std::wstring> result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 std::vector<wchar_t> readerGroupsV;
 DWORD	pchreaderGroups = 0;
 wchar_t *preaderGroup;
 m_nLastError = SCardListReaderGroupsW(m_hContext, nullptr, &pchreaderGroups);
 if (SUCCESS == m_nLastError)
 {
  readerGroupsV.resize(static_cast<size_t>(pchreaderGroups));
  m_nLastError = SCardListReaderGroupsW(m_hContext, readerGroupsV.data(), &pchreaderGroups);
  if (SUCCESS == m_nLastError)
  {
   preaderGroup = readerGroupsV.data();
   while ('\0' != *preaderGroup)
   {
    result.push_back(preaderGroup);
    preaderGroup = preaderGroup + wcslen(preaderGroup) + 1;
   }
  }
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

void WinSCard::EstablishContext(SCOPE Scope)
{
 if (mContextEstablished)
  return;
 m_nLastError=SCardEstablishContext(Scope, nullptr, nullptr, &m_hContext);
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 mContextEstablished = true;
}

bool WinSCard::isContextValid()
{
 if (!mContextEstablished)
  return false;
 m_nLastError = SCardIsValidContext(m_hContext);
 if (SYSTEM_INVALID_HANDLE == m_nLastError)
  return false;
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return true;
}

void WinSCard::ReleaseContext(void)
{
 if (!mContextEstablished)
  return;
 m_nLastError=SCardReleaseContext(m_hContext);
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 mContextEstablished = false;
 mCardConnected = false;
}

long WinSCard::Connect(const std::wstring& Reader, SHARE ShareMode, PROTOCOL PreferredProtocols)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return NO_SERVICE;
 }
 if (mCardConnected)
  Disconnect(Leave);
 m_nLastError=SCardConnectW(m_hContext,Reader.c_str(),ShareMode,PreferredProtocols,&m_hCard,&m_nProtocol);
 if(RESET_CARD==m_nLastError)
  {
   m_nLastError=SCardReconnect(m_hCard,ShareMode,PreferredProtocols, Leave,&m_nProtocol);
  }
 if (SUCCESS != m_nLastError)
 {
  if(m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return m_nLastError;
 }
 mCardConnected = true;
 return SUCCESS;
}

PROTOCOL WinSCard::Reconnect(SHARE ShareMode, PROTOCOL PreferredProtocols, DISCONNECT Initialization)
{
 if(!mCardConnected || !mContextEstablished)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return Undefined;
 }
 DWORD retProtocol;
 m_nLastError = SCardReconnect(m_hCard, ShareMode, PreferredProtocols, Initialization, &retProtocol);
 if (SUCCESS != m_nLastError)
 {
  mCardConnected = false;
  if(m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 return static_cast<PROTOCOL>(retProtocol);
}

void WinSCard::Disconnect(DISCONNECT Disposition)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 if (!mCardConnected)
  return;
 m_nLastError=SCardDisconnect(m_hCard, Disposition);
 if(SUCCESS!=m_nLastError)
  throw SCardException(m_nLastError);
 mCardConnected = false;
}

APDUResponse WinSCard::Transmit(APDUCommand ApduCmd)
{
 if(!mCardConnected)
  {
   m_nLastError = READER_UNAVAILABLE;
   if (m_bThrowingErrors)
    throw SCardException(m_nLastError);
   return APDUResponse(0x90,0x00);
  }
 SCARD_IO_REQUEST	ioRequest;
 ioRequest.dwProtocol=m_nProtocol;
 ioRequest.cbPciLength=8;
 std::vector<BYTE> commandData, retData;
 DWORD	RecvLength=RESPONSE_MAX_LENGTH;
 std::vector<BYTE> responseData(RESPONSE_MAX_LENGTH);
 mLastCommand = ApduCmd;
 commandData=ApduCmd.createCommandVector();
 m_nLastError=SCardTransmit(m_hCard,&ioRequest,commandData.data(), static_cast<DWORD>(commandData.size()), nullptr, responseData.data(), &RecvLength);
 if(RESET_CARD==m_nLastError)
  {
   m_nProtocol=Reconnect(Shared,T0orT1,Leave);
   RecvLength=RESPONSE_MAX_LENGTH;
   responseData.clear();
   m_nLastError=SCardTransmit(m_hCard,&ioRequest, commandData.data(), static_cast<DWORD>(commandData.size()), nullptr, responseData.data(), &RecvLength);
  }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 responseData.resize(RecvLength);
 mLastResponse.setData(responseData);
 return mLastResponse;
}

void WinSCard::BeginTransaction(void)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError=SCardBeginTransaction(m_hCard);
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::EndTransaction(DISCONNECT Disposition)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError=SCardEndTransaction(m_hCard,Disposition);
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

std::vector<BYTE> WinSCard::GetAttribute(DWORD AttribId)
{
 std::vector<BYTE> attr;
 DWORD attrLen = 0;
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return attr;
 }
 m_nLastError=SCardGetAttrib(m_hCard,AttribId,nullptr,&attrLen);
 if(SUCCESS==m_nLastError && 0!=attrLen)
  {
   attr.resize(attrLen);
   m_nLastError = SCardGetAttrib(m_hCard,AttribId,attr.data(),&attrLen);
  }
 if (SUCCESS != m_nLastError)
 {
  if (SYSTEM_NOT_SUPPORTED == m_nLastError)
   return std::vector<BYTE>();
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 return attr;
}

void WinSCard::GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 m_nLastError = SCardGetStatusChangeW(m_hContext, dwTimeout, readerStates.mReaderStates.data(), readerStates.mReaderStates.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::SetAttribute(DWORD AttribId, const std::vector<BYTE>& attr)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardSetAttrib(m_hCard, AttribId, attr.data(), attr.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

bool WinSCard::CardControl(DWORD controlCode, const std::vector<BYTE>& inData, std::vector<BYTE>& outData)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return false;
 }
 DWORD retBytesCount = outData.size();
 m_nLastError = SCardControl(m_hCard, controlCode, inData.data(), inData.size(), outData.data(), outData.size(), &retBytesCount);
 if (SUCCESS != m_nLastError)
 {
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return false;
 }
 outData.resize(retBytesCount);
 return true;
}

DWORD WinSCard::CardControl(DWORD controlCode, LPVOID pInData, DWORD cInData, LPVOID pOutData, DWORD cOutData)
{
 if (!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return 0;
 }
 DWORD retBytesCount = cOutData;
 m_nLastError = SCardControl(m_hCard, controlCode, pInData, cInData,pOutData,cOutData,&retBytesCount);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return retBytesCount;
}
