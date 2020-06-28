#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <vector>
#include <string>
#include "Types.h"
#include "Timestamp.h"

namespace yz
{

namespace ProcessInfo
{
  pid_t pid();
  std::string pidString();
  uid_t uid();
  std::string username();
  uid_t euid();
  Timestamp startTime();

  std::string hostname();
  std::string procStatus();

  int openedFiles();
  int maxOpenFiles();

  int numThreads();
  std::vector<pid_t> threads();
}

}

#endif  // PROCESSINFO_H
