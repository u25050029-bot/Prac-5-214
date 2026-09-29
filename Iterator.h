#ifndef CAMPUSGUARD_ITERATOR_H
#define CAMPUSGUARD_ITERATOR_H

template <typename T>
class Iterator {
public:
    Iterator() {}
    virtual ~Iterator() {}
    Iterator(const Iterator&) = delete;
    Iterator& operator=(const Iterator&) = delete;

    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() const = 0;
    virtual T currentItem() const = 0;
};

#endif
