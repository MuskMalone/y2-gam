
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
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime> 
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
//int crashFunction(int a, int b);

#ifndef LOGGINGSYSTEM

#define LOGGINGSYSTEM
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
	std::queue<std::string> m_buffer1;
	std::mutex m_buffer1Mutex;
	std::condition_variable m_cq1;
	std::ofstream m_logFile;
	//thread queue2 logs with stacktraces
	std::queue<std::string> m_buffer2;
	std::mutex m_buffer2Mutex;
	std::condition_variable m_cq2;
	std::ofstream m_logBacktraceFile;


	std::ifstream m_logConfigFile;
	std::string m_orderStr;
	std::string m_formatStr;

	std::string m_formatDate;
	std::string m_formatLevel;

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

	//thread 2 backtrace enabled
	void LoggingThreadBacktrace(std::atomic<bool>& loggingThreadBacktraceActive);
	std::string QueuePopBacktrace(std::atomic<bool>& loggingThreadBacktraceActive);
	//thread 1 backtrace disabled
	void LoggingThread(std::atomic<bool>& loggingThreadActive);
	//functions to do with log queue
	std::string QueuePop(std::atomic<bool>& loggingThreadActive);
	bool IsQueueEmpty();
	size_t GetQueueSize();

	void Log(LogLevel log_level, std::string message, const std::string& infunctname);
	//functions to do with .log(LogLevel, message)
	void RearrangeOrder(const std::string order);
	void Initialization();
	void InitializationFromConfig();
	void GetStacktrace(int num, int skipFirstNumFrames);
	//void RecordStacktraceNum(int stacktraceNum);

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

#endif //LOGGINGSYSTEM


