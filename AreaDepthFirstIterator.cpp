#include "AreaDepthFirstIterator.h"

#include <stdexcept>

AreaDepthFirstIterator::AreaDepthFirstIterator(AreaComponent* root) : root_(root) {
    if (root_ != nullptr) {
        pending_.push_back(root_);
    }
}

AreaDepthFirstIterator::~AreaDepthFirstIterator() {}

void AreaDepthFirstIterator::first() {
    pending_.clear();
    if (root_ != nullptr) {
        pending_.push_back(root_);
    }
}

void AreaDepthFirstIterator::next() {
    if (pending_.empty()) {
        throw std::out_of_range("area iterator advanced past the end");
    }
    AreaComponent* visited = pending_.front();
    pending_.erase(pending_.begin());
    std::vector<AreaComponent*> children;
    for (std::size_t i = 0; i < visited->childCount(); ++i) {
        children.push_back(visited->childAt(i));
    }
    pending_.insert(pending_.begin(), children.begin(), children.end());
}

bool AreaDepthFirstIterator::isDone() const {
    return pending_.empty();
}

AreaComponent* AreaDepthFirstIterator::currentItem() const {
    if (pending_.empty()) {
        throw std::out_of_range("area iterator has no current item");
    }
    return pending_.front();
}
