#include "pch.h"
#include "LogClass.h"

string LogClass::s_log;
const string LogClass::s_path = "C:\\kiwi\\Chess\\Chess\\Log\\log.txt";

LogClass::LogClass()
{
	s_log.reserve(1000);
	
}

void LogClass::Save(bool append)
{
    if (append)
    {
        SaveAppend();
        return;
    }

    std::ofstream outFile(s_path);

    if (outFile.is_open())
    {
        outFile << s_log;

        outFile.close();
    }
    else
        cout << "File Open Failed!" << endl;

    s_log.clear();
}
void LogClass::SaveAppend()
{
    std::ofstream outFile(s_path, std::ios::app);

    if (outFile.is_open())
    {
        outFile << s_log;

        outFile.close();
    }
    else
        cout << "File Open Failed!" << endl;

    s_log.clear();
}
