//! \file cardevent.h
//! \brief Header file for CardEvent class.
//! \details File is contains class CardEvent must implement a card detection mechanism.
#ifndef CARDEVENT_H
#define CARDEVENT_H

#include "stdafx.h"
#include "NativeSCard.h"

#include <mutex>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <memory>

//! \namespace Smartcards
//! \brief Namespace of smartcard classes
namespace Smartcards
{
 /*!
 \brief This class must implement a card detection mechanism.
 When card insertion or removal is detected, it must callback registered functions
 */
 class CardEvent
 {
 public:
  //! \brief card event type enum
  enum ES_TOKEN_EVENT_TYPE
  {
   ES_TOKEN_EVENT_INSERT,//!< Insert card event
   ES_TOKEN_EVENT_REMOVE,//!< Remove card event
   ES_TOKEN_EVENT_ERROR//!< Error card event
  };
  //! \brief Callback function typedef for card event handler
  //! \param[in] scEvent - card event type
  //! \param[in,out] pReaderName - pointer to reader name
  //! \param[in,out] pATR - pointer to byte vector contains ATR
  //! \param[in,out] pParam - pointer to user data transfer from registry function
  typedef void(__cdecl *eventHandlerFn)(ES_TOKEN_EVENT_TYPE scEvent, std::wstring* pReaderName, std::vector<BYTE>* pATR, void * pParam);
  //! \fn CardEvent::CardEvent()
  //! \brief Default constructor.
  CardEvent(void);
  //! \fn CardEvent::~CardEvent()
  //! \brief Destructor.
  ~CardEvent();
  //! \fn void RegisterEventHandler(eventHandlerFn pEventHandler, void * pParam)
  //! \brief Register callback function in card event handler
  //! \param[in] pEventHandler pointer to callback function.
  //! \param[in] pParam pointer to user data transer in function.
  void RegisterEventHandler(eventHandlerFn pEventHandler, void * pParam);
  //! \fn void UnregisterEventHandler(eventHandlerFn pEventHandler)
  //! \brief Unregister callback function in card event handler
  //! \param[in] pEventHandler pointer to callback function.
  void UnregisterEventHandler(eventHandlerFn pEventHandler);
  //! \fn std::wstring lastReader(void) const
  //! \brief Get last connected reader name.
  //! \return Reader name connected last
  std::wstring lastReader(void) const { return mLastReader; }
  //! \fn std::vector<BYTE> lastATR(void) const
  //! \brief Get last connected card ATR.
  //! \return Byte array contains ATR last connected card
  std::vector<BYTE> lastATR(void) const { return mLastATR; }
 private:
  std::unique_ptr<Smartcards::WinSCard> m_sCard{ new Smartcards::WinSCard };//!< Pointer to WinSCard interface
  std::unique_ptr<std::thread> mThread;//!< Pointer to thread
  std::mutex mMutex;//!< mutex for thread working
  std::map<std::wstring, bool> m_Readers;//!< container for tracking readers
  std::wstring mLastReader;//!< last connected reader name
  std::vector<BYTE> mLastATR;//!< last connected card ATR.
  bool mThreadDoExit{ false };//!< card detection thread exit flag
                              //! \fn long SCardWatchThread(bool& doExit, Smartcards::WinSCard *psCardIface, CardEvent *pInstance)
                              //! \brief card detection thread function
                              //! \details Contains main cycle of detection mechanism
                              //! \param[in,out] doExit flag controlled exit from thread.
                              //! \param[in,out] psCardIface pointer to winscard interface.
                              //! \param[in,out] pInstance pointer to instance this class.
  static long SCardWatchThread(bool& doExit, Smartcards::WinSCard *psCardIface, CardEvent *pInstance);
  //! \fn void CardInserted(const std::wstring& readerName, const std::vector<BYTE>& ATR)
  //! \brief inner function calls from card event thread for card inserted
  //! \param[in] readerName Reader name where inserted card
  //! \param[in] ATR Byte array contains ATR inserted card
  void CardInserted(const std::wstring& readerName, const std::vector<BYTE>& ATR);
  //! \fn void CardRemoved(const std::wstring& readerName, const std::vector<BYTE>& ATR)
  //! \brief inner function calls from card event thread for card removed
  //! \param[in] readerName Reader name where removed card
  //! \param[in] ATR Byte array contains ATR removed card
  void CardRemoved(const std::wstring& readerName, const std::vector<BYTE>& ATR);
  //! \fn CardEvent::initStartTokenWatchThread()
  //! \brief internal function for initialize card detection mechanism
  void initStartTokenWatchThread(void);
  //! \fn CardEvent::terminateStopSCardWatchThread()
  //! \brief internal function for terminate card detection mechanism
  void terminateStopSCardWatchThread(void);
  //! \fn void raiseEvent(ES_TOKEN_EVENT_TYPE eventType)
  //! \brief inner function raise card event
  void raiseEvent(ES_TOKEN_EVENT_TYPE eventType);
  //! \brief Class implement card event handler
  class CEventHandlerEntry
  {
  public:
   //! \brief Constructor
   //! \param[in] pEventHandler - pointer to callback function.
   //! \param[in,out] pReaderName - pointer to reader name
   //! \param[in,out] pATR - pointer to byte vector contains ATR
   //! \param[in,out] pParam - pointer to user data
   CEventHandlerEntry(eventHandlerFn pEventHandler, std::wstring* pReaderName, std::vector<BYTE>* pATR, void * pParam)
   {
    this->pEventHandler = pEventHandler;
    this->pParam = pParam;
    this->pReaderName = pReaderName;
    this->pATR = pATR;
   }
   //! \fn void raiseEvent(ES_TOKEN_EVENT_TYPE eventType)
   //! \brief inner function call card event callback function
   void raiseEvent(ES_TOKEN_EVENT_TYPE eventType) const
   {
    pEventHandler(eventType, pReaderName, pATR, pParam);
   }
   //! \fn bool operator==(const CEventHandlerEntry& other) const
   //! \brief Assignment operator.
   //! \param[in] other The other card event handler object.
   //! \return A shallow copy of this object.
   inline bool operator==(const CEventHandlerEntry& other) const
   {
    return ((*this).pEventHandler == other.pEventHandler);
   }
   //! \fn eventHandlerFn getEventHandler() const
   //! \brief inner function return card event callback function pointer
   eventHandlerFn getEventHandler() const
   {
    return pEventHandler;
   }
  private:
   eventHandlerFn pEventHandler;//!< card event callback function pointer
   std::wstring *pReaderName;//!< pointer to reader name
   std::vector<BYTE> *pATR;//!< pointer to byte vector contains ATR
   void * pParam;//!< pointer to user data
  };
  std::list<std::unique_ptr<CEventHandlerEntry>> eventHandlerList;//!< list contains card event handlers
 };
}
#endif // CARDEVENT_H