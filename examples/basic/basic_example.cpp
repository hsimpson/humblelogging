#include "humble-logging.h"

#ifdef __linux__
#include <sys/time.h>
#endif
#include <stdio.h>

HUMBLE_LOGGER(logger, "default");

using namespace humble::logging;

#ifdef __linux__
long getTimestampMillis()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return (t.tv_sec * 1000 + t.tv_usec/1000) + 0.5;
}
#endif

#ifdef WIN32
long getTimestampMillis()
{
  static LARGE_INTEGER s_frequency;
  static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
  if (s_use_qpc) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (1000LL * now.QuadPart) / s_frequency.QuadPart;
  } else {
    return GetTickCount();
  }
}
#endif

int main(int argc, char **argv)
{
  // Initialize humble logging
  Factory &fac = Factory::getInstance();
  
  // Change the default Formatter (optional)
  fac.setDefaultFormatter(new PatternFormatter("[%date] [%lls] [line=%line] [file=%filename] %m\n"));
  
  // Add Appender which logs to STDOUT.
  ConsoleAppender *consoleAppender = new ConsoleAppender();
  fac.registerAppender(consoleAppender);
  
  //fac.registerAppender(new NullAppender());
  //fac.registerAppender(new FileAppender("humble.log"));
  
  fac.changeGlobalLogLevel(LogLevel::All);
  
  HL_TRACE(logger, "Trace log");
  HL_DEBUG(logger, "Debug log");
  HL_INFO (logger, "Info log");
  HL_WARN (logger, "Warn log");
  HL_ERROR(logger, "Error log");
  HL_FATAL(logger, "Fatal log");

  const long startMs = getTimestampMillis();
  HL_TRACE(logger, "Begin of loop.");
  for (int i = 0, max = 10000; i < max; ++i) {
    HL_TRACE(logger, std::string("Blubb"));
  }
  HL_TRACE(logger, "End of loop.");
  printf("Elapsed %ld ms\n", getTimestampMillis() - startMs);
  return 0;
}
