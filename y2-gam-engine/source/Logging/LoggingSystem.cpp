/******************************************************************************/
/*!
\par        Logging System
\file       LoggingSystem.cpp

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Sep 27, 2023

\brief      This log system is a debugging tool that enables logs to file or console 
			with 6 levels of logging, two of which stacktracing can be enabled.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime> 


/*  _________________________________________________________________________ */
  /*! LoggingSystem()

  default constructor, initializes default values of flags and etc opens
  config file to overwrite hardcoded initializations and closes
  */
LoggingSystem::LoggingSystem() {

	Initialization();
	//config file only for reading
	m_logConfigFile.open("Data/logConfigFile.txt", std::ios::in);
	//m_logConfigFile.open("./logConfigFile.txt", std::ios::in);
	if (!m_logConfigFile.is_open()) {
		std::cout << "LOGCONFIGFILE NOT OPENED" << std::endl;
	}
	else
	{
		//init from configfile
		InitializationFromConfig();
		m_logConfigFile.close();
	}

	m_logFile.open("logFile.txt", std::ios::out | std::ios::app);
	//m_logFile.open("./logFile.txt", std::ios::out | std::ios::app);
	if (!m_logFile.is_open()) {
		std::cout << "LOGFILE NOT OPENED" << std::endl;
	}
	m_logBacktraceFile.open("logBacktraceFile.txt", std::ios::out | std::ios::app);
	//m_logFile.open("./logFile.txt", std::ios::out | std::ios::app);
	if (!m_logFile.is_open()) {
		std::cout << "LOGFBACKTRACEFILE NOT OPENED" << std::endl;
	}

}

/*  _________________________________________________________________________ */
  /*! ~LoggingSystem()
	
  Destructor
  */
LoggingSystem::~LoggingSystem() {
	if (m_logFile.is_open()) {
		m_logFile.close();
	}
	if (m_logBacktraceFile.is_open()) {
		m_logBacktraceFile.close();
	}
}

/*  _________________________________________________________________________ */
  /*! Log

  @param logLevel
  The type of log(one of 6 from enum LogLevel)

  @param message
  message specifying info about the log

  @return void

  call rhis to log, formats the log message and pushes to appropriate buffer
  based on stacktrace enabled flags
  */
//%L: Log Level, %M: user input message, %D: Date time, %F function name
//for infunctname(%F): write __FUNCTION__ or __LINE__
void LoggingSystem::Log(LogLevel log_level, std::string message, const std::string& infunctname) {

	//only print logs with levels >= m_level
	if (log_level < m_level) {
		return;
	}
	//creating log message
	std::string levelMsg;

	switch (log_level) {
	case TRACE_LEVEL:
		levelMsg = "TRACE";
		break;
	case DEBUG_LEVEL:
		levelMsg = "DEBUG";
		break;
	case INFO_LEVEL:
		levelMsg = "INFO";
		break;
	case ERROR_LEVEL:
		levelMsg = "ERROR";
		break;
	case WARNING_LEVEL:
		levelMsg = "WARNING";
		break;
	case CRITICAL_LEVEL:
		levelMsg = "CRITICAL";
		break;
	}

	//date time handling
	std::time_t currentTime;
	std::tm timeInfo;
	std::time(&currentTime);
	localtime_s(&timeInfo, &currentTime);
	char timeBuffer[80];
	std::strftime(timeBuffer, sizeof(timeBuffer), m_formatDate.c_str(), &timeInfo);

	//Info += "[" + LevelMsg + "] " + message + " " + timeBuffer + "\n";

	//rearrange_format eg("[%L]: %M %D %F")
	size_t order_size = m_orderStr.size();
	for (size_t i = 0; i < order_size; i++) {
		if (m_orderStr[i] == '%') {

			if (i > order_size - 1) {
				m_info += '%';
				break;
			}
			++i;
			switch (m_orderStr[i]) {
			case 'L':
				m_info += levelMsg;
				break;
			case 'M':
				m_info += message;
				break;
			case 'D':
				m_info += timeBuffer;
				break;
			case 'F':
				m_info += "FN: ";
				m_info += infunctname;
				break;
			default:
				m_info += '%';
				//Info += order_str[i];
				--i;
			}
		}
		else {
			m_info += m_orderStr[i];
		}

	}

	m_info += '\n';

	//Backtracing enabled queue
	if ((log_level == TRACE_LEVEL && m_traceFlag == true) || (log_level == DEBUG_LEVEL && m_debugFlag == true)) {
		
		backward::TraceResolver resolvetrace;
		backward::StackTrace st;
		// Load the last num frames
		st.load_here(m_stacktraceNum);
		st.skip_n_firsts(m_stacktraceSkipFirstNum);
		// Create a stream to capture the stack trace
		std::ostringstream oss;
		backward::Printer printer_trace;
		printer_trace.print(st, oss);
		// Add the stack trace to info
		m_info += '\n';
		m_info += oss.str();
		//when dealing with buffer2, lock2
		//push m_info into buffer2
		m_buffer2.push_back(m_info);
		//notify condition variable queuepopbacktrace
		
	}//Backtracing disabled queue
	else {
		//push string Info into buffer_1
		//and m_write1 == false
		m_buffer1.push_back(m_info);
		//clears string that has contents of this one log
	}
	m_info.clear();
	FlushTimeElapsed();
	if (m_flushNow == true) {
		Flush1();
		Flush2();
		m_flushNow = false;
		SetTime();
	}
}

void LoggingSystem::SetTime() {
	m_start = std::chrono::steady_clock::now();
}
//flush buffer contents into LogFile
void LoggingSystem::Flush1() {
	//if the buffer is not empty, transfer remaining contents to LogFile
	if (!m_buffer1.empty()) {
		std::cout << "Buffer1 size flushed: " << m_buffer1.size() << std::endl;
		if (m_pipeType == 0) {
			for (const auto& elem : m_buffer1) {
				m_logFile << elem << std::endl;
			}
		}
		else if (m_pipeType == 1) {
			for (const auto& elem : m_buffer1) {
				std::cout << elem << std::endl;
			}
		}
	}
	//clear all buffer contents
	m_buffer1.clear();
}

void LoggingSystem::Flush2() {
	if (!m_buffer1.empty()) {
		std::cout << "Buffer2 size flushed: " << m_buffer2.size() << std::endl;
		if (m_pipeType == 0) {
			for (const auto& elem : m_buffer2) {
				m_logFile << elem << std::endl;
			}
		}
		else if (m_pipeType == 1) {
			for (const auto& elem : m_buffer2) {
				std::cout << elem << std::endl;
			}
		}
	}
	//clear all buffer contents
	m_buffer2.clear();
}

void LoggingSystem::FlushTimeElapsed() {
	std::chrono::duration<double> d = std::chrono::steady_clock::now() - m_start;
	auto final = d.count();
	if (final > m_timeBeforeFlush) {
		m_flushNow = true;
	}
}
//functions to do with logging queue

/*  _________________________________________________________________________ */
  /*! RearrangeOrder

  @param order
  //In a string format input L, D, M, Fith % in front

  @return void

  sets private variable m_orderStr to order given
  in whichever order with any additional characters or space to format the log
  (eg "[%D : %L, %M %F]")
  */
void LoggingSystem::RearrangeOrder(const std::string order) {
	m_orderStr = order;
}

/*  _________________________________________________________________________ */
  /*! Initialization

  @return void

  default initializations
  */
void LoggingSystem::Initialization() {
	//INITIALISATION
	//default order and format
	m_orderStr = "[%L]: %M %D %F";
	m_formatDate = "%F %T";
	//default set level
	m_level = DEFAULT_LOGLEVEL;
	//stacktrace
	m_stacktraceNum = DEFAULT_STACKTRACENUM;
	m_stacktraceSkipFirstNum = DEFAULT_SKIPFIRSTSTACKTRACENUM;
	//flags
	m_traceFlag = DEFAULT_TRACE;
	m_debugFlag = DEFAULT_DEBUG;
	m_pipeType = DEFAULT_PIPETYPE;
	//flush
	//default flush from buffer to file status
	m_flushNow = false;
	//Time for periodic flushing
	SetTime();
	m_timeBeforeFlush = 0.5;

}

/*  _________________________________________________________________________ */
  /*! InitializationFromConfig

  @return void

  Reads from Config file and processes it to set initializations for flags and order
  */
void LoggingSystem::InitializationFromConfig()
{
	std::string temp;
	std::string temp2 = "";
	size_t start;
	size_t end;
	while (getline(m_logConfigFile, temp, '\n'))
	{
		temp2.clear();
		start = 0;
		end = temp.find_first_of(" ");
		temp2 = temp.substr(start, end);
		//std::cout << "start of funct: " << temp2 << '\n';
		if (temp2 == "#")
		{
			start = end + 1;
			end = temp.find_first_of(" ", start);
			temp2 = temp.substr(start, end - start);
			//std::cout << "after #: " << temp2 << '\n';
			if (temp2 == "TRACE")
			{
				start = end + 1;
				end = temp.find_first_of(" ", start);
				temp2 = temp.substr(start, end - start);
				//std::cout << "Trace value: " << temp2 << '\n';
				if (temp2 == "1")
				{
					m_traceFlag = true;
				}
				else if (temp2 == "0")
				{
					m_traceFlag = false;
				}

			}
			else if (temp2 == "DEBUG")
			{
				start = end + 1;
				end = temp.find_first_of(" ", start);
				temp2 = temp.substr(start, end - start);
				//std::cout << "Debug: " << temp2 << '\n';
				if (temp2 == "1")
				{
					m_debugFlag = true;
				}
				else if (temp2 == "0")
				{
					m_debugFlag = false;
				}

			}
			else if (temp2 == "LOGORDER")
			{
				start = temp.find_first_of("<", end);
				end = temp.find_first_of(">", start);
				temp2 = temp.substr(start + 1, end - start - 1);
				//std::cout << "OrderStr: " << temp2 << '\n';
				m_orderStr = temp2;
			}
			else if (temp2 == "PIPETYPE")
			{
				start = end + 1;
				end = temp.find_first_of(" ", start);
				temp2 = temp.substr(start, end - start);
				//std::cout << "Debug: " << temp2 << '\n';
				if (temp2 == "1")
				{
					m_pipeType = 1;
				}
				else if (temp2 == "0")
				{
					m_pipeType = 0;
				}
			}
			else
			{
				continue;
			}
		}
	}
}

/*  _________________________________________________________________________ */
  /*! GetStacktrace

  @param num
  number of stacktrace to print

  @param skipFirstNumFrames
  number of stacktrace to skip from the start
  @return void

  prints stacktrace manually to std::cout
  */
void LoggingSystem::GetStacktrace(int num, int skipFirstNumFrames) {
	backward::TraceResolver resolvetrace;
	backward::StackTrace st;
	// Load the last num frames
	st.load_here(num);
	//skip first number of frames
	st.skip_n_firsts(skipFirstNumFrames);
	backward::Printer printer;
	printer.print(st, std::cout);
}

