#include "AreaComponent.h"

#include <stdexcept>

#include "AreaDepthFirstIterator.h"

AreaComponent::AreaComponent(const std::string& name, const std::string& kind)
    : name_(name), kind_(kind), parent_(nullptr) {}

AreaComponent::~AreaComponent() {}

const std::string& AreaComponent::getName() const {
    return name_;
}

const std::string& AreaComponent::getKind() const {
    return kind_;
}

int AreaComponent::depth() const {
    int result = 0;
    for (const AreaComponent* node = parent_; node != nullptr; node = node->parent_) {
        ++result;
    }
    return result;
}

std::size_t AreaComponent::childCount() const {
    return 0;
}

AreaComponent* AreaComponent::childAt(std::size_t) const {
    throw std::out_of_range(kind_ + " '" + name_ + "' has no child components");
}

std::unique_ptr<Iterator<AreaComponent*>> AreaComponent::createIterator() {
    return std::unique_ptr<Iterator<AreaComponent*>>(new AreaDepthFirstIterator(this));
}
