#ifndef D_LINK_SEQ_HH_
#define D_LINK_SEQ_HH_

#include "seq.hh"
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

template <typename E> class DLinkSeqConstIter;

/** Doubly Linked Seq implementation */
template <typename E>
class DLinkSeq : public Seq<E> {
private:
  friend class DLinkSeqConstIter<E>;
  class Node {
  public:
    E element;
    Node *next;
    Node *prev;
    Node(const E& element) : element(element) {}
    Node() : element(9999) {}
  };

public:
  Node *head;
  Node *tail;
  DLinkSeq() {
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->prev = head;
  }

  static SeqPtr<E> make() {
    return std::make_unique<DLinkSeq<E>>();
  }


  void clear() {
    head->next = tail;
    }

  // add item to start of seq//
  void unshift(const E& item){
    Node* newNode = new Node(item);
    newNode->next = head->next;
    newNode->prev = head;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    }

  //add item to end of seq//
  void push(const E& item){
    Node* newNode = new Node(item);
    newNode->prev = tail->prev;
    newNode->next = tail;
    newNode->next->prev = newNode;
    newNode->prev->next = newNode;
      }

  //remove and return first item from this seq;
  E shift(){
    // Node* newNode = new Node(item);
    // newNode->next = tail;
    // newNode->prev = head;
    // newNode->prev->next = newNode;
    // newNode->next->prev = newNode;
    assert(head->next != tail && "shift on empty array seq");
    E firstNode = head->next->element;
    head->next->next->prev = head; //2nd element prev points to head
    head->next = head->next->next;
    return firstNode;
    }
  //remove and return last item from this seq;
  E pop(){
    assert(head->next != tail && "pop on empty array seq");
    E lastNode = tail->prev->element;
    tail->prev->prev->next = tail; //2nd to last element next points to tail
    tail->prev = tail->prev->prev;
    return lastNode;
  }

  int size() const {
    int count = 0;
    while(head->next != tail){
      count += 1;
    }
    return count;
  }

  ConstIterPtr<E> cbegin() const {
    const DLinkSeq* constThis = static_cast<const DLinkSeq*>(this);
    return std::make_unique<DLinkSeqConstIter<E>>(constThis);
  }

  virtual ConstIterPtr<E> cend() const {
    const DLinkSeq* constThis = static_cast<const DLinkSeq*>(this);
    return std::make_unique<DLinkSeqConstIter<E>>(constThis, false);
  }

};

template <typename E>
class DLinkSeqConstIter : public ConstIter<E> {
public: //can be private

  DLinkSeqConstIter(const DLinkSeq<E>* const seq, bool isHead = true)
    : seq(seq),
      current(isHead ? seq->head->next : seq->tail->prev)
  { }

  /** pre-increment */
  DLinkSeqConstIter& operator++() {
    current = current->next;
    return *this;
  }

  /** pre-decrement */
  DLinkSeqConstIter& operator--() {
    current = current->prev;
    return *this;
  }

  /** return true iff this pointer is not beginning or end */
  virtual operator bool() {
    return (current != seq->head && current != seq->tail);
  }

  /** Return element this is pointing to */
  const E& operator*() {
    return current->element;
  }

  /** Return pointer to element this is pointing to */
  const E* operator->() {
    return &current->element;
  }

private:
  const DLinkSeq<E>* const seq;
  typename DLinkSeq<E>::Node* current; // might need to be constant;
};

#endif //ifndef D_LINK_SEQ_HH_