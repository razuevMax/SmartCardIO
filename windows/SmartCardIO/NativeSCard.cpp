#include "stdafx.h"
#include "NativeSCard.h"
#include "scardexception.h"
#include <winscard.h>

using namespace Smartcards;

const std::wstring WinSCard::ALL_READERS{ SCARD_ALL_READERS };
const std::wstring WinSCard::DEFAULT_READERS{ SCARD_DEFAULT_READERS };
const std::wstring WinSCard::LOCAL_READERS{ SCARD_LOCAL_READERS };
const std::wstring WinSCard::SYSTEM_READERS{ SCARD_SYSTEM_READERS };

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
{
 assign(readerStates);
}

ReadersStates::~ReadersStates()
{
 for (auto& item : mReaderStates)
  delete[] item.szReader;
}

void ReadersStates::assign(const std::vector<SCARD_READERSTATEW>& readerStates)
{
 mReaderStates.assign(readerStates.begin(), readerStates.end());
 DWORD cbReaderStates = readerStates.size();
 for (DWORD idx = 0; idx<cbReaderStates; idx++)
 {
  std::wstring readerName(mReaderStates[idx].szReader);
  mReaderStatesMap[readerName] = idx;;
 }
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
 LPCBYTE pAtr = mReaderStates.at(mReaderStatesMap.at(readerName)).rgbAtr;
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
 stateData.szReader = toWCharArray(readerName);
 stateData.cbAtr = 0;
 mReaderStates.push_back(stateData);
 DWORD idx = mReaderStates.size() - 1;
 mReaderStatesMap[readerName] = idx;
 return idx;
}

wchar_t* ReadersStates::toWCharArray(const std::wstring& str)
{
 size_t size = str.size();
 wchar_t *wszReader = new wchar_t[size + 1];
 wcsncpy_s(wszReader, size, str.c_str(), size);
 wszReader[size] = 0;
 return wszReader;
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
 stateData.szReader = toWCharArray(readerName);
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
 m_bThrowingErrors = false;
 if (mContextEstablished)
  ReleaseContext();
}

std::vector<BYTE> WinSCard::GetCardStatus(DWORD& state, DWORD& protocol)
{
 std::vector<BYTE> atr;
 if(!mCardConnected)
 {
  m_nLastError = READER_UNAVAILABLE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return atr;
 }
 DWORD	pchReaders = AUTOALLOCATE;
 DWORD cByte = 32;
 wchar_t *szListReaders=nullptr;
 atr.resize(32);
 m_nLastError=SCardStatusW(m_hCard,szListReaders,&pchReaders,&state,&protocol,atr.data(),&cByte);
 if(SUCCESS==m_nLastError)
  {
   atr.resize(cByte);
   m_nLastError=SCardFreeMemory(m_hContext, szListReaders);
  }
 if(SUCCESS!=m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return atr;
}

void WinSCard::GetStatusChange(DWORD dwTimeout, ReadersStates& readerStates)
{
 if(!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
 }
 m_nLastError = SCardGetStatusChangeW(m_hContext, dwTimeout, readerStates.mReaderStates.data(), readerStates.mReaderStates.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::LocateCards(const std::vector<std::wstring>& cards, ReadersStates& readerStates)
{
 if(!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 unsigned int count = cards.size();
 std::vector<wchar_t> mszCards;
 for(unsigned int i=0;i<count;i++)
 {
  std::wstring cardName = cards.at(i);
  std::copy(cardName.begin(), cardName.end(), mszCards.end());
  mszCards.push_back(0);
 }
 mszCards.push_back(0);
 m_nLastError = SCardLocateCardsW(m_hContext, mszCards.data(), readerStates.mReaderStates.data(), readerStates.mReaderStates.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::LocateCardsByATR(const std::vector<std::pair<std::vector<BYTE>, std::vector<BYTE>>>& atrMask, ReadersStates& readerStates)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 std::vector<_SCARD_ATRMASK> atrMaskVector;
 for (auto& item : atrMask)
 {
  _SCARD_ATRMASK atrMaskData;
  atrMaskData.cbAtr = item.first.size();
  std::copy(item.first.begin(), item.first.end(), atrMaskData.rgbAtr);
  std::copy(item.second.begin(), item.second.end(), atrMaskData.rgbMask);
  atrMaskVector.push_back(atrMaskData);
 }
 m_nLastError = SCardLocateCardsByATRW(m_hContext, atrMaskVector.data(), atrMaskVector.size(), readerStates.mReaderStates.data(), readerStates.mReaderStates.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::CardCancel()
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardCancel(m_hContext);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
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

GUID WinSCard::GetProviderId(const std::wstring& cardName)
{
 GUID result = { 0,0,0,0 };
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 m_nLastError = SCardGetProviderIdW(m_hContext, cardName.c_str(), &result);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

std::vector<std::wstring> WinSCard::ListCards(const std::vector<BYTE>& cardAtr, const std::vector<GUID>& guidInterfaces)
{
 std::vector<std::wstring> result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 std::vector<wchar_t> cardsV;
 DWORD	pchCards = 0;
 wchar_t *pCardName;
 m_nLastError = SCardListCardsW(m_hContext, (cardAtr.size()<ATR_LENGTH)?nullptr:cardAtr.data(), guidInterfaces.empty() ? nullptr : guidInterfaces.data(), guidInterfaces.size(), nullptr, &pchCards);
 if (SUCCESS == m_nLastError)
 {
  cardsV.resize(static_cast<size_t>(pchCards));
  m_nLastError = SCardListCardsW(m_hContext, (cardAtr.size()<ATR_LENGTH) ? nullptr : cardAtr.data(), guidInterfaces.empty() ? nullptr : guidInterfaces.data(), guidInterfaces.size(), cardsV.data(), &pchCards);
  if (SUCCESS == m_nLastError)
  {
   pCardName = cardsV.data();
   while ('\0' != *pCardName)
   {
    result.push_back(pCardName);
    pCardName = pCardName + wcslen(pCardName) + 1;
   }
  }
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

std::vector<std::wstring> WinSCard::ListCards(const std::vector<GUID>& guidInterfaces)
{
 if (guidInterfaces.empty())
 {
  m_nLastError = INVALID_PARAMETER;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return std::vector<std::wstring>();
 }
 return ListCards(std::vector<BYTE>(), guidInterfaces);
}

std::vector<std::wstring> WinSCard::ListCards(const std::vector<BYTE>& cardAtr)
{
 if (cardAtr.size()<ATR_LENGTH)
 {
  m_nLastError = INVALID_ATR;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return std::vector<std::wstring>();
 }
 return ListCards(cardAtr, std::vector<GUID>());
}

std::vector<std::wstring> WinSCard::ListCards(void)
{
 return ListCards(std::vector<BYTE>(), std::vector<GUID>());
}

std::vector<GUID> WinSCard::ListInterfaces(const std::wstring& cardName)
{
 std::vector<GUID> result;
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return result;
 }
 DWORD pcguidInterfaces = 0;
 m_nLastError = SCardListInterfacesW(m_hContext, cardName.c_str(),nullptr,&pcguidInterfaces);
 if (SUCCESS == m_nLastError)
 {
  result.resize(static_cast<size_t>(pcguidInterfaces));
  m_nLastError = SCardListInterfacesW(m_hContext, cardName.c_str(), result.data(), &pcguidInterfaces);
 }
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return result;
}

void WinSCard::AddReaderToGroup(const std::wstring& ReaderName, const std::wstring& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardAddReaderToGroupW(m_hContext,ReaderName.c_str(),GroupName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetCardType(const std::wstring& CardName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetCardTypeW(m_hContext, CardName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetReader(const std::wstring& ReaderName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetReaderW(m_hContext, ReaderName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::ForgetReaderGroup(const std::wstring& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardForgetReaderGroupW(m_hContext, GroupName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceReader(const std::wstring& ReaderName, const std::wstring& DeviceName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardIntroduceReaderW(m_hContext, ReaderName.c_str(), DeviceName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceReaderGroup(const std::wstring& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardIntroduceReaderGroupW(m_hContext, GroupName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::RemoveReaderFromGroup(const std::wstring& ReaderName, const std::wstring& GroupName)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardRemoveReaderFromGroupW(m_hContext, ReaderName.c_str(), GroupName.c_str());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceCardType(const std::wstring& CardName, const std::vector<GUID>& guidInterfaces, const std::vector<BYTE>& cardAtr, const std::vector<BYTE>& cardAtrMask, LPCGUID pguidPrimaryProvider)
{
 if (!mContextEstablished)
 {
  m_nLastError = NO_SERVICE;
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return;
 }
 m_nLastError = SCardIntroduceCardTypeW(m_hContext, CardName.c_str(), pguidPrimaryProvider, guidInterfaces.empty()?nullptr:guidInterfaces.data(),guidInterfaces.size(),cardAtr.data(),cardAtrMask.empty()?nullptr:cardAtrMask.data(),cardAtr.size());
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
}

void WinSCard::IntroduceCardType(const std::wstring& CardName, const std::vector<GUID>& guidInterfaces, const std::vector<BYTE>& cardAtr, LPCGUID pguidPrimaryProvider)
{
 IntroduceCardType(CardName, guidInterfaces, cardAtr, std::vector<BYTE>(), pguidPrimaryProvider);
}

void WinSCard::IntroduceCardType(const std::wstring& CardName, const std::vector<BYTE>& cardAtr, const std::vector<BYTE>& cardAtrMask, LPCGUID pguidPrimaryProvider)
{
 IntroduceCardType(CardName, std::vector<GUID>(), cardAtr, cardAtrMask, pguidPrimaryProvider);
}

void WinSCard::IntroduceCardType(const std::wstring& CardName, const std::vector<BYTE>& cardAtr, LPCGUID pguidPrimaryProvider)
{
 IntroduceCardType(CardName, std::vector<GUID>(), cardAtr, std::vector<BYTE>(), pguidPrimaryProvider);
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

#if (NTDDI_VERSION >= NTDDI_WIN8)
std::vector<BYTE> WinSCard::GetReaderIcon(const std::wstring& Reader)
{
 std::vector<BYTE> pbIcon;
 if (!mContextEstablished)
  return pbIcon;
 DWORD cbIcon;
 m_nLastError = SCardGetReaderIconW(m_hContext,Reader.c_str(),nullptr,&cbIcon);
 if (SUCCESS != m_nLastError)
 {
  if (m_bThrowingErrors)
   throw SCardException(m_nLastError);
  return pbIcon;
 }
 pbIcon.resize(cbIcon);
 m_nLastError = SCardGetReaderIconW(m_hContext, Reader.c_str(), pbIcon.data(), &cbIcon);
 if (SUCCESS != m_nLastError && m_bThrowingErrors)
  throw SCardException(m_nLastError);
 return pbIcon;
}
#endif

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
