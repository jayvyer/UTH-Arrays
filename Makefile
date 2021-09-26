output: seq-test.cc arrayseq.hh command.cc seq.hh nums.cc DLinkSeq.hh
	g++ seq-test.cc arrayseq.hh command.cc seq.hh nums.cc DLinkSeq.hh -o nums

clean:
	rm *.o nums
