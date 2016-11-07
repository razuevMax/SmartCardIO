#ifndef CARDEVENT_H
#define CARDEVENT_H

#include "stdafx.h"
/// <summary>
/// This class must implement a card detection mechanism.
/// 
#include "NativeSCard.h"

#include <mutex>

namespace Smartcards
{

 class CardEvent
 {
 public:
  enum ES_TOKEN_EVENT_TYPE
  {
   ES_TOKEN_EVENT_INSERT,
   ES_TOKEN_EVENT_REMOVE,
   ES_TOKEN_EVENT_ERROR
  };
  typedef void(__cdecl *eventHandlerFn)(ES_TOKEN_EVENT_TYPE scEvent, std::wstring* pReaderName, std::vector<BYTE>* pATR, void * pParam);
  CardEvent(void);
  ~CardEvent();
  void RegisterEventHandler(eventHandlerFn pEventHandler, void * pParam);
  void UnregisterEventHandler(eventHandlerFn pEventHandler);
  std::wstring lastReader(void) const { return mLastReader; }
  std::vector<BYTE> lastATR(void) const { return mLastATR; }
 private:
  std::unique_ptr<Smartcards::WinSCard> m_sCard{ new Smartcards::WinSCard };
  std::unique_ptr<std::thread> mThread;
  std::mutex mMutex;
  std::map<std::wstring, bool> m_Readers;
  std::wstring mLastReader;
  std::vector<BYTE> mLastATR;
  bool mThreadDoExit{false};
  static long SCardWatchThread(bool& doExit, Smartcards::WinSCard *psCardIface, CardEvent *pInstance);
  void CardInserted(const std::wstring& readerName, const std::vector<BYTE>& ATR);
  void CardRemoved(const std::wstring& readerName, const std::vector<BYTE>& ATR);
  void initStartTokenWatchThread(void);
  void terminateStopSCardWatchThread(void);
  //Events interface
  void raiseEvent(ES_TOKEN_EVENT_TYPE eventType);
  class CEventHandlerEntry
  {
  public:
   CEventHandlerEntry(eventHandlerFn pEventHandler, std::wstring* pReaderName, std::vector<BYTE>* pATR, void * pParam)
   {
    this->pEventHandler = pEventHandler;
    this->pParam = pParam;
    this->pReaderName = pReaderName;
    this->pATR = pATR;
   }
   void raiseEvent(ES_TOKEN_EVENT_TYPE eventType) const
   {
    pEventHandler(eventType, pReaderName,pATR, pParam);
   }
   inline bool operator==(const CEventHandlerEntry& other) const
   {
    return ((*this).pEventHandler == other.pEventHandler);
   }
   eventHandlerFn getEventHandler() const
   {
    return pEventHandler;
   }
  private:
   eventHandlerFn pEventHandler;
   std::wstring *pReaderName;
   std::vector<BYTE> *pATR;
   void * pParam;
  };
  std::list<std::unique_ptr<CEventHandlerEntry>> eventHandlerList;
 };
}
#endif // CARDEVENT_H
