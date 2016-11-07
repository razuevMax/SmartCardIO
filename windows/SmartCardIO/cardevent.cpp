#include "stdafx.h"
#include "cardevent.h"
#include "NativeSCard.h"
#include <thread>

using namespace Smartcards;

CardEvent::CardEvent(void)
{
 m_sCard->EstablishContext(Smartcards::User);
 initStartTokenWatchThread();
}

CardEvent::~CardEvent()
{
 terminateStopSCardWatchThread();
 m_sCard->ReleaseContext();
}

void CardEvent::RegisterEventHandler(eventHandlerFn pEventHandler, void* pParam)
{
 std::lock_guard<std::mutex> lock(mMutex);
 eventHandlerList.push_back(std::unique_ptr<CEventHandlerEntry>(new CEventHandlerEntry(pEventHandler,&this->mLastReader,&this->mLastATR, pParam)));
}

void CardEvent::UnregisterEventHandler(eventHandlerFn pEventHandler)
{
 std::lock_guard<std::mutex> lock(mMutex);
 bool found = false;
 std::list<std::unique_ptr<CEventHandlerEntry>>::iterator curEvtHandler;
 while (true) {
  for (curEvtHandler = eventHandlerList.begin(); curEvtHandler != eventHandlerList.end(); ++curEvtHandler) {
   if (curEvtHandler->get()->getEventHandler() == pEventHandler) {
    found = true;
    break;
   }
  }
  if (!found)
   break;

  eventHandlerList.erase(curEvtHandler);
  found = false;
 };
}

void CardEvent::CardInserted(const std::wstring& readerName, const std::vector<BYTE>& ATR)
{
 mLastReader = readerName;
 mLastATR = ATR;
 raiseEvent(ES_TOKEN_EVENT_INSERT);
}

void CardEvent::CardRemoved(const std::wstring& readerName, const std::vector<BYTE>& ATR)
{
 mLastReader = readerName;
 mLastATR = ATR;
 raiseEvent(ES_TOKEN_EVENT_REMOVE);
}

long CardEvent::SCardWatchThread(bool& doExit, Smartcards::WinSCard *psCardIface, CardEvent *pInstance)
{
 std::recursive_mutex sCardWatchMutex;
 DWORD state;
 std::vector<BYTE> ATR;
 DWORD	nProtocol;
 std::vector<std::wstring> readers;
 bool throwErrorsOld = psCardIface->isThrowingErrors();
 try
 {
  std::lock_guard<std::recursive_mutex> lock(sCardWatchMutex);
  psCardIface->throwErrors(false);
  readers = psCardIface->ListReaders();
  for (auto it = readers.begin(); it != readers.end(); ++it)
  {
   std::wstring readerName = *it;
   psCardIface->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
   if (!psCardIface->isError())
   {
    ATR = psCardIface->GetCardStatus(state, nProtocol);
    psCardIface->Disconnect(Smartcards::Leave);
    if (state == Smartcards::Specific)
    {
     pInstance->m_Readers.insert({ readerName, true });
     pInstance->CardInserted(readerName, ATR);
    }
   }
   else
    pInstance->m_Readers.insert({ readerName, false });
  }
 }
 catch (...)
 {
  return psCardIface->lastError();
 }
 while (false == doExit)
 {
  try
  {
   std::lock_guard<std::recursive_mutex> lock(sCardWatchMutex);
   readers = psCardIface->ListReaders();
   if (psCardIface->isError())
    continue;
   if (true == doExit)
    break;
   for (auto it = readers.begin(); it != readers.end(); ++it)
   {
    if (true == doExit)
     break;
    std::wstring readerName = *it;
    if (pInstance->m_Readers.find(readerName) != pInstance->m_Readers.end())
    {
     psCardIface->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
     if (!psCardIface->isError())
     {
      ATR = psCardIface->GetCardStatus(state, nProtocol);
      psCardIface->Disconnect(Smartcards::Leave);
      if (state == Smartcards::Specific && !pInstance->m_Readers[readerName])
      {
       pInstance->m_Readers[readerName] = true;
       pInstance->CardInserted(readerName, ATR);
      }
     }
     else
     {
      if (pInstance->m_Readers[readerName])
      {
       pInstance->m_Readers[readerName] = false;
       pInstance->CardRemoved(readerName, ATR);
      }
     }
    }
    else
    {
     psCardIface->Connect(readerName, Smartcards::Shared, Smartcards::T0orT1);
     if (!psCardIface->isError())
     {
      ATR = psCardIface->GetCardStatus(state, nProtocol);
      psCardIface->Disconnect(Smartcards::Leave);
      if (state == Smartcards::Specific)
      {
       pInstance->m_Readers[readerName] = true;
       pInstance->CardInserted(readerName, ATR);
      }
      else
       pInstance->m_Readers[readerName] = false;
     }
     else
      pInstance->m_Readers[readerName] = false;
    }
   }
   for (auto it = pInstance->m_Readers.begin(); it != pInstance->m_Readers.end();)
   {
    std::wstring readerName = it->first;
    if (std::find(readers.begin(), readers.end(), readerName) == readers.end())
    {
     it = pInstance->m_Readers.erase(it);
     pInstance->CardRemoved(readerName, ATR);
    }
    else
     it++;
   }
  }
  catch (...)
  {
   return psCardIface->lastError();
  }
 }
 psCardIface->throwErrors(throwErrorsOld);
 return Smartcards::SUCCESS;
}

void CardEvent::initStartTokenWatchThread()
{
 mThreadDoExit = false;
 mThread.reset(new std::thread(&(CardEvent::SCardWatchThread), std::ref(mThreadDoExit), m_sCard.get(), this));
}

void CardEvent::terminateStopSCardWatchThread()
{
 if (true == mThreadDoExit) {
  return;
 }
 std::mutex lMutex;
 lMutex.lock();
 mThreadDoExit = true;
 lMutex.unlock();
 mThread->join();
}

void CardEvent::raiseEvent(ES_TOKEN_EVENT_TYPE eventType)
{
 std::lock_guard<std::mutex> lock(mMutex);
 try
 {
  for (auto curEvtHandler = eventHandlerList.begin(); curEvtHandler != eventHandlerList.end(); ++curEvtHandler)
  {
   if (nullptr == curEvtHandler->get())
    continue;
   curEvtHandler->get()->raiseEvent(eventType);
  }
 }
 catch (...)
 {

 }
}
