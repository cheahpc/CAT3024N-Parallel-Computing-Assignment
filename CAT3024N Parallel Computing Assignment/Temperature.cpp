#include "Temperature.h"

//Empty Constructor
Temperature::Temperature()
{
}

//Empty Destructor
Temperature::~Temperature()
{
}

//Load File
bool Temperature::Load(std::string FileName)
{
	//Setup Variables to hold information for each line
	std::string place;
	int year, month, date, time;
	float temperature;
	//Load the txt file
	std::string info;
	std::ifstream temperatureFile;
	//Open the text file
	temperatureFile.open(FileName);
	//Display Message
	std::cout << "Starting Loading" << std::endl;
	//Until end of file do the following
	while (!temperatureFile.eof())
	{
		//Get the current line of information and assign it to the correct place
		temperatureFile >> place >> year >> month >> date >> time >> temperature;
		//Push back the current line information into the vectors for storage
		m_Name.push_back(place);
		m_Year.push_back(year);
		m_Month.push_back(month);
		m_Day.push_back(date);
		m_Time.push_back(time);
		m_Temp.push_back(temperature);
		m_TempI.push_back((int)(temperature * 10.0f));
	}
	//Display loaded message to console
	std::cout << "File Loaded\n" << std::endl;
	//Return true
	return true;
}