#include "ResponseComponent.h"

#include "CoordinationEvent.h"
#include "ResponseMediator.h"

ResponseComponent::ResponseComponent(const std::string& componentName)
    : componentName_(componentName), mediator_(nullptr) {}

ResponseComponent::~ResponseComponent() {}

void ResponseComponent::setMediator(ResponseMediator* mediator) {
    mediator_ = mediator;
}

const std::string& ResponseComponent::getComponentName() const {
    return componentName_;
}

void ResponseComponent::changed(const CoordinationEvent& event) {
    mediator_->notify(*this, event);
}
