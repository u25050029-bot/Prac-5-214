#include "AreaComponent.h"

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
    if (parent_ == nullptr) {
        return 0;
    }
    return parent_->depth() + 1;
}

std::size_t AreaComponent::childCount() const {
    return 0;
}

AreaComponent* AreaComponent::childAt(std::size_t) const {
    return nullptr;
}
