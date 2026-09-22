#include "AreaGroup.h"

#include <stdexcept>

#include "Log.h"

AreaGroup::AreaGroup(const std::string& name, const std::string& kind) : AreaComponent(name, kind) {}

AreaGroup::~AreaGroup() {}

void AreaGroup::adopt(std::unique_ptr<AreaComponent> child) {
    child->parent_ = this;
    children_.push_back(std::move(child));
}

void AreaGroup::lock(OperationResult& result) {
    Log::line("Composite", getKind() + " '" + getName() + "': " + "lock" + " forwarded to " + std::to_string(children_.size()) + " child component(s)");
    Log::Scope scope;
    for (std::size_t i = 0; i < children_.size(); ++i) {
        children_[i]->lock(result);
    }
}

void AreaGroup::unlock(OperationResult& result) {
    Log::line("Composite", getKind() + " '" + getName() + "': " + "unlock" + " forwarded to " + std::to_string(children_.size()) + " child component(s)");
    Log::Scope scope;
    for (std::size_t i = 0; i < children_.size(); ++i) {
        children_[i]->unlock(result);
    }
}

void AreaGroup::restrictAccess(AccessLevel level, OperationResult& result) {
    Log::line("Composite", getKind() + " '" + getName() + "': " + "restrict(" + toString(level) + ")" + " forwarded to " + std::to_string(children_.size()) + " child component(s)");
    Log::Scope scope;
    for (std::size_t i = 0; i < children_.size(); ++i) {
        children_[i]->restrictAccess(level, result);
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
        if (child->securedCount() > 0) {
            total += child->doorCount();
        }
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
