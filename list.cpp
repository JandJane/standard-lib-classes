#include <iostream>

template <typename T>
struct Node {
    Node * prev;
    Node * next;
    T value;

    Node(const T& val = T()) : value(val) {
        prev = nullptr;
        next = nullptr;
    }
};

template <typename T>
class List {
public:
    Node<T> * head;
    Node<T> * tail;
    size_t sz;

    List() {
        head = tail = nullptr;
        sz = 0;
    }

    List(const List<T>& other) {
        head = tail = nullptr;
        sz = 0;
        try {
            for (auto it = other.begin(); it != other.end(); ++it) {
                this->push_back(T(*it));
            }
        } catch (...) {
            while (sz) {
                this->pop_back();
            }
            throw;
        }
    }

    List& operator=(const List<T>& other) {
        while (sz) {
            this->pop_back();
        }
        try {
            for (auto it = other.begin(); it != other.end(); ++it) {
                this->push_back(T(*it));
            }
        } catch (...) {
            while (sz) {
                this->pop_back();
            }
            throw;
        }
        return *this;
    }

    void push_back(const T& val) {
        if (sz == 0) {
            head = tail = new Node<T>(val);
        } else {
            tail->next = new Node<T>(val);
            tail->next->prev = tail;
            tail = tail->next;
        }
        ++sz;
    }

    void push_front(const T& val) {
        if (sz == 0) {
            head = tail = new Node<T>(val);
        } else {
            head->prev = new Node<T>(val);
            head->prev->next = head;
            head = head->prev;
        }
        ++sz;
    }

    void pop_back() {
        if (sz == 0) {
            ++sz;
        } else if (sz == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            delete tail->next;
            tail->next = nullptr;
        }
        --sz;
    }

    void pop_front() {
        if (sz == 0) {
            ++sz;
        } else if (sz == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            head = head->next;
            delete head->prev;
            head->prev = nullptr;
        }
        --sz;
    }

    size_t size() const {
        return sz;
    }

    class IterList {
private:
    Node<T> * data;
    const List<T>& base_list;

public:
    IterList(Node<T> * link, const List<T>& l) : data(link), base_list(l) {}

    IterList operator++() {
        if (data) {
            data = data->next;
        } else {
            data = base_list.head;
        }
        return *this;
    }

    IterList operator--() {
        if (data) {
            data = data->prev;
        } else {
            data = base_list.tail;
        }
        return *this;
    }

    IterList operator++(int) {
        IterList tmp = *this;
        if (data) {
            data = data->next;
        } else {
            data = base_list.head;
        }
        return tmp;
    }

    IterList operator--(int) {
        IterList tmp = *this;
        if (data) {
            data = data->prev;
        } else {
            data = base_list.tail;
        }
        return tmp;
    }

    bool operator==(const IterList& other) const {
        return (other.data == data);
    }

    bool operator!=(const IterList& other) const {
        return (!(*this == other));
    }

    T& operator*() {
        if (data) {
            return data->value;
        }
    }
};

    IterList begin() const {
        return IterList(head, *this);
    }

    IterList end() const {
        return IterList(nullptr, *this);
    }

    ~List() {
        while (sz) {
            this->pop_back();
        }
    }
};
