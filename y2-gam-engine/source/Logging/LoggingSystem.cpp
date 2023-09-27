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

// Constructor
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
}
// Destructor
LoggingSystem::~LoggingSystem() {
	/*Flush();
	m_logFile.close();*/
}

//Log
//%L: Log Level, %M: user input message, %D: Date time, %F function name
//for infunctname: write __FUNCTION__ or __LINE__
void LoggingSystem::Log(LogLevel log_level, std::string message, const std::string& infunctname) {

	//std::cout << "LOG ENTERED" << std::endl;
	if (log_level < m_level) {
		return;
	}

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


	std::time_t currentTime;
	std::tm timeInfo;
	std::time(&currentTime);
	localtime_s(&timeInfo, &currentTime);
	char timeBuffer[80];
	std::strftime(timeBuffer, sizeof(timeBuffer), m_formatDate.c_str(), &timeInfo);

	//Info += "[" + LevelMsg + "] " + message + " " + timeBuffer + "\n";
	//std::cout << Info << '\n';

	//rearrange_format("[%L]: %M %T")
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
		//std::cout << "ENTERED BACKTRACE ENABLED PUSH" << '\n';
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
		//std::cout << "M INFO IN TRACE: " << m_info << '\n';
		std::unique_lock<std::mutex> lock2(m_buffer2Mutex);
		m_buffer2.push(m_info);
		m_cq2.notify_one();
		m_info.clear();
	}//Backtracing disabled queue
	else {
		//std::cout << "ENTERED BACKTRACE DISABLED PUSH" << '\n';
		//push string Info into buffer_1
		std::unique_lock<std::mutex> lock(m_buffer1Mutex);
		//and m_write1 == false
		m_buffer1.push(m_info);
		m_cq1.notify_one();
		//clears string that has contents of this one log
		m_info.clear();
		//std::cout << "QUEUESIZE AT END OF LOG: " << m_buffer1.size() << '\n';
	}




}

//THESE ONLY WORK FOR QUEUE 1 THREAD 1
// --------------------------------------------------------------
//for debug purposes get queue size
size_t LoggingSystem::GetQueueSize() {
	std::unique_lock<std::mutex> lock(m_buffer1Mutex);
	return m_buffer1.size();
}

//this function returns bool on whether the queue buffer is empty
bool LoggingSystem::IsQueueEmpty() {
	std::unique_lock<std::mutex> lock(m_buffer1Mutex);
	return m_buffer1.empty();
}
//---------------------------------------------------------------
// 
//this function safely locks and waits for m_buffer1 to not be empty and for condition variable to be notified, pops m_buffer1
std::string LoggingSystem::QueuePop(std::atomic<bool>& loggingThreadActive)
{
	std::unique_lock<std::mutex> lock(m_buffer1Mutex);
	//sleep until condition variable m_cq is notified and queue is not empty
	m_cq1.wait(lock, [this, &loggingThreadActive] {return !m_buffer1.empty() || !loggingThreadActive; });
	//std::cout << "QUEUESIZE AT END OF QUEUEPOP: " << m_buffer1.size();
	if (!m_buffer1.empty()) {
		std::string msg = m_buffer1.front();
		m_buffer1.pop();
		return msg;
	}
	else {
		return "";
	}
}

std::string LoggingSystem::QueuePopBacktrace(std::atomic<bool>& loggingThreadBacktraceActive) {
	std::unique_lock<std::mutex> lock2(m_buffer2Mutex);
	//sleep until condition variable m_cq is notified and queue is not empty
	m_cq2.wait(lock2, [this, &loggingThreadBacktraceActive] {return !m_buffer2.empty() || !loggingThreadBacktraceActive; });
	//std::cout << "entered queue pop backtrace" << '\n';
	//std::cout << "QUEUESIZE AT END OF QUEUEPOP: " << m_buffer1.size();
	if (!m_buffer2.empty()) {
		std::string msg = m_buffer2.front();
		//std::cout << "QUEUEPOP MSG: " << msg << '\n';
		m_buffer2.pop();
		return msg;
	}
	else {
		return "";
	}
}
void LoggingSystem::LoggingThread(std::atomic<bool>& loggingThreadActive) {
	//opening LogFile to write logs to
	m_logFile.open("logFile.txt", std::ios::out | std::ios::app);
	//m_logFile.open("./logFile.txt", std::ios::out | std::ios::app);
	if (!m_logFile.is_open()) {
		std::cout << "LOGFILE NOT OPENED" << std::endl;
	}
	while (loggingThreadActive == true) {
		std::string msg = QueuePop(loggingThreadActive);
		if (!msg.empty()) {
			//write to file
			if (m_pipeType == 0) {
				m_logFile << msg << std::endl;
			}//write to std::cout
			else if (m_pipeType == 1) {
				std::cout << msg << std::endl;
			}
		}
	}
	m_logFile.close();

}

void LoggingSystem::LoggingThreadBacktrace(std::atomic<bool>& loggingThreadBacktraceActive) {
	//opening LogFile to write logs to
	m_logBacktraceFile.open("logBacktraceFile.txt", std::ios::out | std::ios::app);
	//m_logFile.open("./logBacktraceFile.txt", std::ios::out | std::ios::app);
	if (!m_logBacktraceFile.is_open()) {
		std::cout << "LOGBACKTRACEFILE NOT OPENED" << std::endl;
	}
	while (loggingThreadBacktraceActive == true) {
		std::string msg = QueuePopBacktrace(loggingThreadBacktraceActive);
		if (!msg.empty()) {
			//write to file
			if (m_pipeType == 0) {
				m_logBacktraceFile << msg << std::endl;
			}//write to std::cout
			else if (m_pipeType == 1) {
				std::cout << msg << std::endl;
			}
		}
	}
	m_logBacktraceFile.close();
}
//functions to do with logging queue

//In a string format input L, D, M with % in front
//in whichever order with any additional characters or space to format the log (eg "[%D : %L, %M]")
void LoggingSystem::RearrangeOrder(const std::string order) {
	//warning level '%L', message '%M', time '%T'
	m_orderStr = order;

}

//format for date and time follows strftime identifiers (eg D, "%F %T")
void LoggingSystem::SetFormat(char const set_type, const std::string format) {
	switch (set_type) {
	case 'D':
		m_formatDate = format;
		break;
	case 'L':
		m_formatLevel = format;
		break;
	}
}



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

}

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
				//finding '<'
				start = temp.find_first_of("<", end);
				//start = end + 1;
				end = temp.find_first_of(">", start);
				//std::cout << "Start pos: " << start << " end pos: " << end << '\n';
				//std::cout << "end: " << temp[end] << '\n';
				//std::cout << "end-1 " << temp[end - 1] << '\n';
				temp2 = temp.substr(start + 1, end - start - 1);
				//std::cout << "OrderStr: " << temp2 << '\n';
				m_orderStr = temp2;

			}
			else
			{
				continue;
			}


		}


	}
}

//prints stacktrace manually to std::cout
void LoggingSystem::GetStacktrace(int num, int skipFirstNumFrames) {
	backward::TraceResolver resolvetrace;
	backward::StackTrace st;
	// Load the last num frames
	st.load_here(num);
	//skip first number of frames
	st.skip_n_firsts(skipFirstNumFrames);
	// Create a stream to capture the stack trace
	//std::ostringstream stream;
	//prints to std::cout
	backward::Printer printer;
	printer.print(st, std::cout);
}

