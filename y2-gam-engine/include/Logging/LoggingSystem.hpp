
#pragma once
/******************************************************************************/
/*!
\par        Logging System
\file       LoggingSystem.hpp

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Sep 27, 2023

\brief      This log system is a debugging tool that enables logs to file or console
			with 6 levels of logging, two of which stacktracing can be enabled.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

//int crashFunction(int a, int b);

//#ifndef LOGGINGSYSTEM
//
//#define LOGGINGSYSTEM
//default m_traceFlag true: enable stacktracing false: disable stacktracing
#define DEFAULT_TRACE false
#define DEFAULT_DEBUG false
#define DEFAULT_LOGLEVEL INFO_LEVEL
#define DEFAULT_STACKTRACENUM 5
#define DEFAULT_SKIPFIRSTSTACKTRACENUM 0
//default write logs to where 0: to LogFile 1: to std::cout
#define DEFAULT_PIPETYPE 0

enum LogLevel {
	//Informational purposes, captures flow of application
	//stacktrace available
	TRACE_LEVEL,
	//Similar to trace but more related to debugging purposes
	//stacktrace available
	DEBUG_LEVEL,
	//Normal and expected occurrence, not an error
	//No stacktrace 
	INFO_LEVEL,
	//Unexpected occurrence, not an error
	WARNING_LEVEL,
	//Error occurred
	ERROR_LEVEL,
	//Application-breaking error, take action immediately
	CRITICAL_LEVEL
};

class LoggingSystem {

protected:
	//constructor
	LoggingSystem();
private:
	std::atomic<int> m_pipeType;
	/*static LogLevel level;*/
	LogLevel m_level;
	std::string m_info;
	//thread queue1 logs without stacktraces
	std::vector<std::string> m_buffer1;
	std::ofstream m_logFile;
	//thread queue2 logs with stacktraces
	std::vector<std::string> m_buffer2;
	std::ofstream m_logBacktraceFile;


	std::ifstream m_logConfigFile;
	std::string m_orderStr;
	std::string m_formatStr;
	std::string m_formatDate;

	//flush
	bool m_flushNow;
	double m_timeBeforeFlush;

	//stacktrace
	int m_stacktraceNum;
	int m_stacktraceSkipFirstNum;
	bool m_traceFlag;
	bool m_debugFlag;


	//time
	std::chrono::time_point<std::chrono::steady_clock> m_start;

public:

	/*static void set_level(LogLevel new_level)
	{
		level = new_level;
	}*/
	const std::vector<std::string>& GetLogsBuffer1() const { return m_buffer1; }
	const std::vector<std::string>& GetLogsBuffer2() const { return m_buffer2; }
	//cannot be copied or assigned
	LoggingSystem(const LoggingSystem&) = delete;
	LoggingSystem& operator=(const LoggingSystem&) = delete;

	static LoggingSystem& GetInstance()
	{
		static LoggingSystem instance;
		return instance;
	}



	// Destructor
	~LoggingSystem();

	
	void Log(LogLevel log_level, std::string message, const std::string& infunctname);
	//functions to do with .log(LogLevel, message)
	void RearrangeOrder(const std::string order);
	void Initialization();
	void InitializationFromConfig();
	void GetStacktrace(int num, int skipFirstNumFrames);
	//void RecordStacktraceNum(int stacktraceNum);
	void Flush1();
	void Flush2();
	void SetTime();
	void FlushTimeElapsed();
};

//Initializating an object instance of stopwatch starts clock
class Stopwatch {
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
public:
	//default constructor sets start time
	Stopwatch() { m_start = std::chrono::steady_clock::now(); }
	//returns elapsed time
	constexpr auto ElapsedTime() const {
		std::chrono::duration<double> d = std::chrono::steady_clock::now() - m_start;
		auto final = d.count();
		return final;
	}
	void Reset() {
		m_start = std::chrono::steady_clock::now();
	}
};

//#endif //LOGGINGSYSTEM


