#include "stdafx.h"
#include "APDUResponse.h"
#include <sstream>
#include <iomanip>
#include <vector>

using namespace Smartcards;

APDUResponse::APDUResponse(const std::vector<BYTE>& vData)
{
 setData(vData);
}

APDUResponse::APDUResponse( const APDUResponse& response )
{
 m_bSw1=response.m_bSw1;
 m_bSw2=response.m_bSw2;
 m_vData=response.m_vData;
}

APDUResponse& APDUResponse::operator=(const APDUResponse& other)
{
 if(this!=&other)
 {
  m_bSw1=other.m_bSw1;
  m_bSw2=other.m_bSw2;
  m_vData=other.m_vData;
 }
 return *this;
}

APDUResponse::APDUResponse(BYTE sw1, BYTE sw2)
{
 m_bSw1=sw1;
 m_bSw2=sw2;
}

APDUResponse::~APDUResponse(void)
{
}

std::string APDUResponse::toString( void ) const
{
 std::string result;
 std::stringstream sDataStream(result, ios_base::in);
 sDataStream.setf(std::ios::hex, std::ios::basefield);
 sDataStream.setf(ios_base::uppercase);
 sDataStream.width(4);
 sDataStream << std::endl << "SW=" << std::setfill('0') << getStatus() << std::setfill(' ') << ' ';
 size_t count = m_vData.size();
 if (count)
 {
  sDataStream << std::endl << "Data=";
  sDataStream.width(2);
  unsigned int nI = 0;
  while (nI < count)
  {
   unsigned int di = 0;
   while (di<8 && nI<count)
   {
    sDataStream << std::setfill('0') << m_vData.at(nI++) << std::setfill(' ') << ' ';
    di++;
   }
   sDataStream << endl;
  }
 }
 sDataStream.flush();
	return result;
}

void APDUResponse::setData( const std::vector<BYTE>& vData )
{
 if(vData.empty())
  {
   m_vData.clear();
   m_bSw1=0;
   m_bSw2=0;
   return;
  }
 size_t size=vData.size();
 if (size>SW_LENGTH)
  m_vData.assign(vData.begin(), vData.end() - 2);
 m_bSw1 = vData.at(size - SW_LENGTH);
 m_bSw2 = vData.at(size - SW_LENGTH + 1);
}
