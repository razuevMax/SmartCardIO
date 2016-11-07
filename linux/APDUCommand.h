#ifndef APDUCOMMAND_H
#define APDUCOMMAND_H

namespace Smartcards
{
class APDUParam;
/// <summary>
/// This class represents a command APDU
/// </summary>
class APDUCommand
{
public:
	/// <summary>
	/// Minimum size in bytes of an APDU command
	/// </summary>
	static const int	APDU_MIN_LENGTH = 4;
private:
 BYTE m_bClass{ 0 };
 BYTE m_bIns{ 0 };
 BYTE m_bP1{ 0 };
 BYTE m_bP2{ 0 };
 BYTE m_nLe{ 0 };
	std::vector<BYTE> m_vData;
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="bCla">Class byte</param>
	/// <param name="bIns">Instruction byte</param>
	/// <param name="bP1">Parameter P1 byte</param>
	/// <param name="bP2">Parameter P2 byte</param>
	/// <param name="baData">Data to send to the card if any, null if no data to send</param>
	/// <param name="bLe">Number of data expected, 0 if none</param>
	APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const std::vector<BYTE>& vData=std::vector<BYTE>(), BYTE nLe=0);
 APDUCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, std::initializer_list<BYTE> ilist, BYTE nLe = 0);
	virtual ~APDUCommand(void);
 void setCommand(BYTE bCla, BYTE bIns, BYTE bP1, BYTE bP2, const std::vector<BYTE>& vData = std::vector<BYTE>(), BYTE nLe=0);
	/// <summary>
	/// Overrides the ToString method to format to a string the APDUCommand object
	/// </summary>
	/// <returns></returns>
	std::string toString(void) const;
 void setClass(BYTE cls) { m_bClass=cls; }
 void setP1(BYTE p1) { m_bP1=p1; }
 void setP2(BYTE p2) { m_bP2=p2; }
 void setIns(BYTE ins) { m_bIns=ins; }
 void setLe(BYTE le) { m_nLe = le; }
 void setData(const std::vector<BYTE>& data) { m_vData = data; }
 BYTE getClass(void) const { return m_bClass; }
 BYTE getP1(void) const { return m_bP1; }
 BYTE getP2(void) const { return m_bP2; }
 BYTE getIns(void) const { return m_bIns; }
 BYTE getLe(void) const { return m_nLe; }
 std::vector<BYTE> getData(void) const { return m_vData; }
 size_t getDataSize(void) const { return m_vData.size(); }
 bool emptyData(void) const { return m_vData.empty(); }
 std::vector<BYTE> createCommandVector(void);
};

}
#endif