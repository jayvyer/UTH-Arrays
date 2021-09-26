#ifndef SEQ_H_
#define SEQ_H_

#include <iostream>
#include <memory>

//forward declaration for sequence
template <typename E> class Seq;

//sequences are wrapped in a unique ptr
template <typename E> using SeqPtr = std::unique_ptr<Seq<E>>;

//forward declaration for a constant iterator.
//const as it does not allow object being iterated over to be changed
template <typename E> class ConstIter;

//iterators are returned wrapped in a unique_ptr
template <typename E> using ConstIterPtr = std::unique_ptr<ConstIter<E>>;

/** Seq ADT */
template <typename E>
class Seq {

public:
  //no copy constructor
  Seq(const Seq&) = delete;

  //no assignment operator
  void operator=(const Seq&) = delete;

  /** base destructor */
  virtual ~Seq() {}

  /** clear contents from this seq, making it empty. */
  virtual void clear() = 0;

  /** add item to the start of this seq. */
  virtual void unshift(const E& item) = 0;

  /** remove and return first item from this seq.
   *  fails if this seq is empty.
   */
  virtual E shift() = 0;

  /** add item at the end of this seq. */
  virtual void push(const E& item) = 0;

  /** remove and return last item from this seq.
   *  fails if this seq is empty.
   */
  virtual E pop() = 0;

  /** return number of elements in this seq. */
  virtual int size() const = 0;

  /** return an iterator initialized to start of this seq */
  virtual ConstIterPtr<E> cbegin() const = 0;

  /** return an iterator initialized to end of this seq */
  virtual ConstIterPtr<E> cend() const = 0;

protected:
  /** constructor */
  //protected so that only sub-classes can call it
  Seq() {}

};

/** const iterator: not possible to change underlying element E */
template <typename E>
class ConstIter {
public:

  //destructor
  virtual ~ConstIter() {}

  /** pre-increment */
  virtual ConstIter& operator++() = 0;

  /** pre-decrement */
  virtual ConstIter& operator--() = 0;

  // /** post-increment */
  // virtual ConstIter& operator++(int) = 0;

  // /** post-decrement */
  // virtual ConstIter& operator--(int) = 0;

  /** return true iff this pointer is not done */
  virtual operator bool() = 0;

  /** Return element this is pointing to */
  virtual const E& operator*() = 0;

  /** Return pointer to element this is pointing to */
  virtual const E* operator->() = 0;

protected:
  //constructor
  //protected so that only sub-classes can call it
  ConstIter() {}

};

//following is a use of the above ConstIter to output elements of a seq.
//ostream << E must be defined
template <typename E>
std::ostream&
operator<<(std::ostream& out, const Seq<E>& seq)
{
  out << "[ ";
  ConstIterPtr<E> iterP = seq.cbegin();
  //*iterP accessed iterator wrapped by smart-pointer iterP
  for (ConstIter<E>& p = *iterP; p; ++p) {
    out << *p << " ";
  }
  out << "]";
  return out;
}


#endif //ifndef SEQ_H_