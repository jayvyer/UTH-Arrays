#include <cctype>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "command.hh"

static int
skipLinearSpace(std::istream& in) {
  int c;
  while ((c = in.peek()) == ' ' || c == '\t') {
    in.get();
  }
  return c;
}


Command::Command(Seq<TestType>& seq, std::istream& in,
		 std::ostream& out, std::ostream& err)
  : seq(seq), in(in), out(out), err(err)
{
  getArgs();
}

bool Command::isEof() { return command == "EOF"; }

void Command::getArgs() {
  command = "";
  repeat = 1;
  value = 0;
  const int maxArgs = 3;
  int c = skipLinearSpace(in);
  for (int i = 0; i < maxArgs; i++) {
    if (c == '\n' || c == EOF) break;
    std::string arg;
    in >> arg;
    if (std::isdigit(arg[0])) {
      *((i == 0) ? &repeat : &value) = std::stoi(arg);
    }
    else {
      command = arg;
    }
    c = skipLinearSpace(in);
  }
  if (c == '\n') in.get();
  if (c == EOF) command = "EOF";
}


using OperateFn = TestType(Seq<TestType> *seq);

enum class CommandType {
  CLEAR, PUSH, POP, SHIFT, UNSHIFT, HELP, ERROR
};
struct CommandDef {
  std::string command;
  CommandType type;
};

static const CommandDef COMMANDS[] = {
  { "clear", CommandType::CLEAR },
  { "unshift", CommandType::UNSHIFT },
  { "shift", CommandType::SHIFT },
  { "push", CommandType::PUSH },
  { "pop", CommandType::POP },
  { "help", CommandType::HELP },
};
static int N_COMMANDS = sizeof(COMMANDS)/sizeof(COMMANDS[0]);

static CommandType
getCommandType(std::string command) {
  for (int i = 0; i < N_COMMANDS; i++) {
    if (COMMANDS[i].command == command) return COMMANDS[i].type;
  }
  return CommandType::ERROR;
}


void Command::help(std::ostream& out) {
  out << "[REPEAT] COMMAND [VALUE]" << std::endl
      << "COMMAND is one of ";
  for (int i = 0; i < N_COMMANDS; ++i) {
    out << COMMANDS[i].command;
    if (i < N_COMMANDS - 1) out << "|";
  }
  out << std::endl;
}


void Command::run() {
  if (command == "" || command == "EOF") return;
  CommandType type = getCommandType(command);
  for (int i = 0; i < repeat; i++) {
    switch (type) {
    case CommandType::CLEAR:
      seq.clear();
      break;
    case CommandType::UNSHIFT:
      seq.unshift(value);
      break;
    case CommandType::SHIFT:
      out << seq.shift() << std::endl;
      break;
    case CommandType::PUSH:
      seq.push(value);
      break;
    case CommandType::POP:
      out << seq.pop() << std::endl;
      break;
    case CommandType::HELP:
      help(out);
      break;
    default:
      err << "bad command \"" << command << "\"" << std::endl;
    }
  }
  out << seq << std::endl;
}