#include "arrayseq.hh"
#include "command.hh"
#include "DLinkSeq.hh"
#include "seq.hh"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

//pair mapping sequence name to sequence instance
struct NameToSeq {
  const std::string name;
  const SeqPtr<TestType> seq;  //smart pointer to sequence
};

//this structure keeps track of smart pointers to sequences, ensuring
//that the sequences will automatically be cleaned up on program exit.
static const NameToSeq nameSeqMap[] = {
  { "ArraySeq", ArraySeq<TestType>::make() },
  { "DLinkSeq", DLinkSeq<TestType>::make() },
};
static const int nSeqs = sizeof(nameSeqMap)/sizeof(nameSeqMap[0]);

/** return a |-separated string of names of all supported sequences */
static std::string makeNames() {
  std::string names = "";
  for (int i = 0; i < nSeqs; ++i) {
    const auto nameToSeqP = &nameSeqMap[i];
    if (names.length() > 0) names += "|";
    names += nameToSeqP->name;
  }
  return names;
}

static const std::string names = makeNames();

/** Returns a raw pointer to the sequence specified by seqName. */
static Seq<TestType>*
getSeqContainer(const std::string seqName) {
  for (int i = 0; i < nSeqs; ++i) {
    const auto nameToSeqP = &nameSeqMap[i];
    //get() on unique pointer returns underlying raw pointer
    if (nameToSeqP->name == seqName) return nameToSeqP->seq.get();
  }
  std::cerr << "invalid container " << seqName
	    << "; must be one of " << names << std::endl;
  std::exit(1);
}

static void prompt(std::ostream& out) { out << ">> " << std::flush; }

// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     std::cerr << "usage: " << argv[0] << " " << names << std::endl;
//     std::exit(1);
//   }
//   Seq<TestType>* seq = getSeqContainer(argv[1]);
//   std::istream& in = std::cin;
//   std::ostream& out = std::cout;
//   std::ostream& err = std::cerr;
//   Command::help(out);
//   do {
//     prompt(out);
//     //note that since Seq is abstract, command takes a Seq& argument
//     Command command(*seq, in, out, err);
//     if (command.isEof()) break;
//     command.run();
//   } while (true);
// }