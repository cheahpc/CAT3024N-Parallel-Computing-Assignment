#ifndef STATIONDATA_H
#define STATIONDATA_H

// Include
#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
class StationData
{
public:
	// Empty constructor and destructor
	StationData();
	~StationData();

	// Load weather text file
	bool StationData::Load(string FileName);

	// Getter functions
	vector<float> &GetTemp() { return m_Temp; }
	vector<int> &GetTempI() { return m_TempI; }
	vector<int> &GetDay() { return m_Day; }
	vector<int> &GetYear() { return m_Year; }
	vector<int> &GetMonth() { return m_Month; }
	vector<int> &GetTime() { return m_Time; }
	vector<string> &GetStationName() { return m_StationName; }

private:
	// Vector to hold integer temperature values
	vector<int> m_TempI;
	// Vector to hold float temperature values
	vector<float> m_Temp;
	// Vectors to hold days, years, months, time
	vector<int> m_Day;
	vector<int> m_Year;
	vector<int> m_Month;
	vector<int> m_Time;
	// Vector to hold names
	vector<string> m_StationName;
};

#endif // STATIONDATA_H