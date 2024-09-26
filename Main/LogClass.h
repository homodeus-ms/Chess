#pragma once
class LogClass
{
public:
	LogClass();
	static void AppendLine(const string& str) { s_log.append(str); s_log.append("\n\n"); }
	static void AppendLine(int32 num) { s_log.append(std::to_string(num)); s_log.append("\n"); }
	static void Clear() { s_log.clear(); }
	static void Save(bool append = false);

private:
	static void SaveAppend();
	
private:
	static string s_log;
	static const string s_path;
};

