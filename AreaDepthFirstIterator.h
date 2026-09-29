#ifndef CAMPUSGUARD_AREADEPTHFIRSTITERATOR_H
#define CAMPUSGUARD_AREADEPTHFIRSTITERATOR_H

#include <vector>

#include "AreaComponent.h"
#include "Iterator.h"

class AreaDepthFirstIterator : public Iterator<AreaComponent*> {
public:
    explicit AreaDepthFirstIterator(AreaComponent* root);
    ~AreaDepthFirstIterator() override;

    void first() override;
    void next() override;
    bool isDone() const override;
    AreaComponent* currentItem() const override;

private:
    AreaComponent* root_;
    std::vector<AreaComponent*> pending_;
};

#endif
