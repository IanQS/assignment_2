#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
 private:
    int user{0}; //: normal processes executing in user mode
    int nice{0}; //: niced processes executing in user mode
    int system{0}; //: processes executing in kernel mode
    int idle{0}; //: twiddling thumbs
    int iowait{0}; //: waiting for I/O to complete
    int irq{0};//: servicing interrupts
    int softirq{0}; //: servicing softirqs
    int steal{0}; //: involuntary wait
};

#endif