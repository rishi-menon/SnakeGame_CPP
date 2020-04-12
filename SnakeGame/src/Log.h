#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#ifdef _DEBUG
#define ASSERT_M(x) if(!(x)) __debugbreak()
#define ASSERT(x, m) if (!(x)) LOG_INFO("{0} (Line: {1}, File: {2})", m, __LINE__, __FILE__); \
                     ASSERT_M (x)

#define LOG_TRACE(...)       ::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)        ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)        ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)       ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)       ::Log::GetLogger()->critical(__VA_ARGS__)

#else
#define ASSERT_M(x) 
#define ASSERT(x, m) 

#define LOG_TRACE(...)      
#define LOG_INFO(...)       
#define LOG_WARN(...)       
#define LOG_ERROR(...)      
#define LOG_FATAL(...)      

#endif// _DEBUG defined

class Log
{
public:
   static void Init();
   inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
   static std::shared_ptr<spdlog::logger> s_Logger;
};


