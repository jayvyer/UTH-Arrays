#ifndef ARRAY_SEQ_HH_
#define ARRAY_SEQ_HH_

#include "seq.hh"
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

template <typename E> class ArraySeqConstIter;


/** Array Seq implementation */
template <typename E>
class ArraySeq : public Seq<E> {
private:

  int maxSize;    //# of allocated elements
  int index;      //index one beyond last element
  E *seq;         //allocated array

  friend class ArraySeqConstIter<E>;

  enum class Private { TAG, DEFAULT_SIZE = 8 };
public:

  /** constructor */
  ArraySeq(int size, Private tag) :
    maxSize(size),
    index(0),
    seq(new E[size])
  {}

  static SeqPtr<E> make(int size = static_cast<int>(Private::DEFAULT_SIZE)) {
    return std::make_unique<ArraySeq<E>>(size, Private::TAG);
  }

  //destructor
  ~ArraySeq() { delete[] seq; }

  /** empty out the list */
  void clear() { index = 0; }

  /** add item to the start of this seq. */
  void unshift(const E& item) {
    for (auto i = index; i > 0; i--) seq[i] = seq[i - 1];
    ++index;
    seq[0] = item;
  }

  /** add item at the end of this seq. */
  void push(const E& item) {
    assert(index < maxSize && "arrayseq overflow");
    seq[index++] = item;
  }

  /** remove and return first item from this seq.
   *  fails with failing assert() when this seq is empty.
   */
  E shift() {
    assert(index > 0 && "shift on empty array seq");
    E it = seq[0];
    for (auto i = 0; i < index - 1; i++) {
      seq[i] = seq[i + 1];
    }
    --index;
    return it;
  }

  /** remove and return last item from this seq.
   *  fails with failing assert() when this seq is empty.
   */
  E pop() {
    assert(index > 0 && "pop on empty array seq");
    return seq[--index];
  }

  /** return number of elements currently in this */
  int size() const { return index; }

  /** Return an iterator initialized to start of this seq */
  ConstIterPtr<E> cbegin() const {
    const ArraySeq* constThis = static_cast<const ArraySeq*>(this);
    return std::make_unique<ArraySeqConstIter<E>>(constThis);
  }

  /** Return an iterator initialized to end of this seq */
  virtual ConstIterPtr<E> cend() const {
    const ArraySeq* constThis = static_cast<const ArraySeq*>(this);
    return std::make_unique<ArraySeqConstIter<E>>(constThis, false);
  }

};

template <typename E>
class ArraySeqConstIter : public ConstIter<E> {
public:

  ArraySeqConstIter(const ArraySeq<E>* const seq, bool isHead=true)
    : seq(seq),
      index(isHead ? 0 : seq->index - 1)
  { }

  /** pre-increment */
  ArraySeqConstIter& operator++() {
    ++index;
    return *this;
  }

  /** pre-decrement */
  ArraySeqConstIter& operator--() {
    --index;
    return *this;
  }

  // /** post-increment */
  // ArraySeqConstIter& operator++(int) { }

  // /** post-decrement */
  // ArraySeqConstIter& operator--(int) { }

  /** return true iff this pointer is not "NULL" */
  virtual operator bool() {
    return 0 <= index && index < seq->index;
  }

  /** Return element this is pointing to */
  const E& operator*() {
    return seq->seq[index];
  }

  /** Return pointer to element this is pointing to */
  const E* operator->() {
    return &seq->seq[index];
  }

private:
  const ArraySeq<E>* const seq;
  int index;
};

#endif //ifndef ARRAY_SEQ_HH_