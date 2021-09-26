#include <ostream>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "arrayseq.hh"
#include "DLinkSeq.hh"

int main(int argc, char *argv[]){
  using namespace std;
  using TestType = int;
  // if number of arguments is less than 3
  if (argc < 2) {
      cerr << "usage: " << argv[0] << " [-a] INTS_FILE_PATH"  << endl;
      exit(1);
    }
  // if there is an option, then the option should only be [-a]
  if(argv[1][0] == '-' && argv[1][1] != 'a'){
      cerr << "usage: " << argv[0] << " [-a] INTS_FILE_PATH"  << endl;
    }
  if(argv[1][0] == '-' && argv[1][1] == 'a'){
    ifstream in(argv[2]);
    auto mySeqPtr = ArraySeq<TestType>::make();
    TestType num;
    while(in.good()){
      in >> num;
      mySeqPtr->push(num);
    }
    if(!in.eof()){
      cout << "No such file or directory";
    }

    auto reverse = mySeqPtr->cend();
    for (auto forward = mySeqPtr->cbegin(); *forward; ++(*forward)) {
      std::cout << **forward << std::endl;
      std::cout << **reverse << std::endl;
      --(*reverse);
    }
  }
  // if there is no option, then the incoming argument should be a filename and we should use a doubly linked list instead of array seq
  else{
    ifstream in(argv[1]);
    auto mySeqPtr = DLinkSeq<TestType>::make();
    while(in.good()){
      TestType num;
      in >> num;
      mySeqPtr->push(num);
    }
    if(!in.eof()){
      cout << "No such file or directory";
    }

    // std::cout << *mySeqPtr << std::endl;
    auto reverse = mySeqPtr->cend();
    for(auto forward = mySeqPtr->cbegin(); *forward; ++(*forward)){
      std::cout << **forward << std::endl;
      std::cout << **reverse << std::endl;
      --(*reverse);
    }

  }
  return 0;
}