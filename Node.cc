template <typename E>
DLinkSeq{
private:
  class Node {
  public:
    E element;
    Node *next;
    Node *prev;
    Node(T &element) : element(element) {};
    friend class ArraySeqConstIter<E>;
  };
public:
  Node* head;
  Node* tail;
  DLinkSeq {
    head = new Node();
    tail = new Node();
  }

  @override
  void unshift(const E &item){
    Node* newNode = new Node(item);
    newNode->next = head->next;
    newNode->prev = head;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
  }

  void shift(const E &element){
    Node* newNode = new Node(item);
    newNode->next = tail;
    newNode->prev = head;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
  }


};
