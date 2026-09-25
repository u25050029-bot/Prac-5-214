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
    AreaComponent* visited = pending_.back();
    pending_.pop_back();
    for (std::size_t i = visited->childCount(); i > 0; --i) {
        pending_.push_back(visited->childAt(i - 1));
    }
}

bool AreaDepthFirstIterator::isDone() const {
    return pending_.empty();
}

AreaComponent* AreaDepthFirstIterator::currentItem() const {
    if (pending_.empty()) {
        throw std::out_of_range("area iterator has no current item");
    }
    return pending_.back();
}
