#include "StationData.h"

// Empty Constructor
StationData::StationData()
{
}

// Empty Destructor
StationData::~StationData()
{
}

// Load File
bool StationData::Load(string FileName)
{
	// Setup Variables to hold information for each line
	string place;
	int year, month, date, time;
	float temperature;
	// Load the txt file
	string info;
	ifstream temperatureFile;

	// Open the text file
	temperatureFile.open(FileName);

	// Check if file exist
	if (!temperatureFile.good())
	{
		return false;
	}
	else
	{
		// Until end of file do the following
		while (!temperatureFile.eof())
		{
			// Get the current line of information and assign it to the correct place
			temperatureFile >> place >> year >> month >> date >> time >> temperature;
			// Push back the current line information into the vectors for storage
			m_StationName.push_back(place);
			m_Year.push_back(year);
			m_Month.push_back(month);
			m_Day.push_back(date);
			m_Time.push_back(time);
			m_Temp.push_back(temperature);
			m_TempI.push_back((int)(temperature * 10.0f));
		}
		// Return true
		return true;
	}
}