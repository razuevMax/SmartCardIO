#include "stdafx.h"
#include "APDUCommand.h"
#include <sstream>
#include <iomanip>
#include <vector>

using namespace Smartcards;
using namespace std;

APDUCommand::APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const std::vector<BYTE>& vData, BYTE nLe)
 : m_bClass(bCla), m_bIns(bIns), m_bP1(bP1), m_bP2(bP2), m_nLe(nLe), m_vData(vData)
{
}

APDUCommand::APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, std::initializer_list<BYTE> ilist, BYTE nLe)
 : m_bClass(bCla), m_bIns(bIns), m_bP1(bP1), m_bP2(bP2), m_nLe(nLe), m_vData(ilist)
{
}

APDUCommand::~APDUCommand(void)
{
}

void APDUCommand::setCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const std::vector<BYTE>& vData, BYTE nLe)
{
 m_bClass = bCla;
 m_bIns = bIns;
 m_bP2 = bP2;
 m_bP1 = bP1;
 m_nLe = nLe;
 setData(vData);
}

std::string APDUCommand::toString() const
{
 string strData;
 stringstream sDataStream(strData, ios_base::in);
 sDataStream.setf(std::ios::hex, std::ios::basefield);
 sDataStream.setf(ios_base::uppercase);
 BYTE bLc = 0, bP3 = m_nLe;
 size_t count = m_vData.size();
 if (count)
 {
  sDataStream.width(2);
  unsigned int nI = 0;
  while (nI < count)
  {
   unsigned int di = 0;
   while (di<8 && nI<count)
   {
    sDataStream << setfill('0') << m_vData.at(nI++) << setfill(' ') << ' ';
    di++;
   }
   sDataStream << endl;
  }
  bLc = static_cast<BYTE>(m_vData.size());
  bP3 = bLc;
 }
 string result;
 sDataStream.flush();
 sDataStream.str(result);
 sDataStream.setf(std::ios::hex, std::ios::basefield);
 sDataStream.setf(ios_base::uppercase);
 sDataStream.width(2);
 sDataStream << endl << "Class=" << setfill('0') << m_bClass << setfill(' ') << " Ins=" << setfill('0') << m_bIns << setfill(' ') << " P1=" <<
  setfill('0') << m_bP1 << setfill(' ') << " P2=" << setfill('0') << m_bP2 << setfill(' ') << " P3=" << setfill('0') << bP3 << setfill(' ') << " Data=" << endl << strData;
 sDataStream.flush();
 return result;
}

std::vector<BYTE> APDUCommand::createCommandVector(void)
{
 // Build the command APDU
 std::vector<BYTE> commandVector;
 size_t dataSize = m_vData.size();
 commandVector.push_back(m_bClass);
 commandVector.push_back(m_bIns);
 commandVector.push_back(m_bP1);
 commandVector.push_back(m_bP2);
 if (m_vData.empty())
  commandVector.push_back(m_nLe);
 else
 {
  commandVector.push_back(static_cast<BYTE>(dataSize));
  commandVector.reserve(APDU_MIN_LENGTH + dataSize+1);
  commandVector.insert(commandVector.end(), m_vData.begin(), m_vData.end());
 }
 return commandVector;
}
