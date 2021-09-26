#ifndef COMMAND_HH_
#define COMMAND_HH_

#include <iostream>
#include <string>

#include "command.hh"
#include "seq.hh"

using TestType = int;

/** Crude class; no error checking;
 *  uses switch rather than subclassing or dispatch table
 */
class Command {
public:
  Command(Seq<TestType>& seq, std::istream& in,
	  std::ostream& out, std::ostream& err);
  void run();
  bool isEof();
  static void help(std::ostream& out);
private:
  void getArgs();
  Seq<TestType>& seq;
  std::string command;
  int value;
  int repeat;
  std::istream& in;
  std::ostream& out;
  std::ostream& err;
};


#endif //ifndef COMMAND_HH_