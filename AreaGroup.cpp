#include "AreaGroup.h"

#include <stdexcept>

#include "Log.h"

AreaGroup::AreaGroup(const std::string& name, const std::string& kind) : AreaComponent(name, kind) {}

AreaGroup::~AreaGroup() {}

void AreaGroup::adopt(std::unique_ptr<AreaComponent> child) {
    if (!child) {
        throw std::invalid_argument("cannot add an empty component to " + getName());
    }
    child->parent_ = this;
    children_.push_back(std::move(child));
}

void AreaGroup::announce(const std::string& operation) const {
    Log::line("Composite", getKind() + " '" + getName() + "': " + operation + " forwarded to " + std::to_string(children_.size()) + " child component(s)");
}

void AreaGroup::lock(OperationResult& result) {
    announce("lock");
    Log::Scope scope;
    for (const auto& child : children_) {
        child->lock(result);
    }
}

void AreaGroup::unlock(OperationResult& result) {
    announce("unlock");
    Log::Scope scope;
    for (const auto& child : children_) {
        child->unlock(result);
    }
}

void AreaGroup::restrictAccess(AccessLevel level, OperationResult& result) {
    announce("restrict(" + toString(level) + ")");
    Log::Scope scope;
    for (const auto& child : children_) {
        child->restrictAccess(level, result);
    }
}

int AreaGroup::doorCount() const {
    int total = 0;
    for (const auto& child : children_) {
        total += child->doorCount();
    }
    return total;
}

int AreaGroup::securedCount() const {
    int total = 0;
    for (const auto& child : children_) {
        total += child->securedCount();
    }
    return total;
}

std::string AreaGroup::statusText() const {
    return std::to_string(securedCount()) + "/" + std::to_string(doorCount()) + " doors secured";
}

std::size_t AreaGroup::childCount() const {
    return children_.size();
}

AreaComponent* AreaGroup::childAt(std::size_t index) const {
    if (index >= children_.size()) {
        throw std::out_of_range("child index out of range for " + getName());
    }
    return children_[index].get();
}
