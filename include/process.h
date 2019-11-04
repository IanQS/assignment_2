#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               //
  std::string User();                      //
  std::string Command();                   //
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const&) const;  // TODO: See src/process.cpp
  bool operator>(Process const&) const;
  explicit Process(int pid): pid_(pid) {};

  // TODO: Declare any necessary private members
 private:
    int pid_;
    float cpu_util_{0.f};
};

#endif