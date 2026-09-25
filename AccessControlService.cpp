#include "AccessControlService.h"

#include <memory>
#include <stdexcept>

#include "AreaComponent.h"
#include "Incident.h"
#include "Iterator.h"
#include "Log.h"

namespace {

void reportNode(AreaComponent* node, int indent) {
    Log::line("Area", std::string(static_cast<std::size_t>(indent) * 2, ' ') + node->getKind() + " " + node->getName() + ": " + node->statusText());
    for (std::size_t i = 0; i < node->childCount(); ++i) {
        reportNode(node->childAt(i), indent + 1);
    }
}

}

AccessControlService::AccessControlService(AreaComponent& campusRoot)
    : componentName_("AccessControl"), campusRoot_(campusRoot) {}

AccessControlService::~AccessControlService() {}

const std::string& AccessControlService::getComponentName() const {
    return componentName_;
}

AreaComponent* AccessControlService::findArea(const std::string& name) const {
    std::unique_ptr<Iterator<AreaComponent*>> it = campusRoot_.createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        if (it->currentItem()->getName() == name) {
            return it->currentItem();
        }
    }
    Log::line(getComponentName(), "no area named '" + name + "'");
    return nullptr;
}

bool AccessControlService::secureArea(AreaComponent* area, Incident* context) {
    requireArea(area);
    Log::line(getComponentName(), "Lock down " + area->getKind() + " '" + area->getName() + "'");
    OperationResult result;
    {
        Log::Scope scope;
        area->lock(result);
    }
    int confirmed = result.attempted - static_cast<int>(result.failed.size());
    Log::line(getComponentName(), "lock of '" + area->getName() + "': " + std::to_string(confirmed) + "/" + std::to_string(result.attempted) + " doors confirmed by gateway");
    if (!result.failed.empty()) {
        std::string doors;
        for (std::size_t i = 0; i < result.failed.size(); ++i) {
            if (i > 0) {
                doors += ", ";
            }
            doors += result.failed[i];
        }
        Log::line(getComponentName(), "lock failed at " + doors + (context != nullptr ? " during incident " + context->label() : std::string()));
        return false;
    }
    Log::line(getComponentName(), "'" + area->getName() + "' secured");
    return true;
}

bool AccessControlService::restrictArea(AreaComponent* area, AccessLevel level, Incident* context) {
    requireArea(area);
    Log::line(getComponentName(), "Restrict " + area->getKind() + " '" + area->getName() + "' to " + toString(level));
    OperationResult result;
    {
        Log::Scope scope;
        area->restrictAccess(level, result);
    }
    int confirmed = result.attempted - static_cast<int>(result.failed.size());
    Log::line(getComponentName(), "restrict of '" + area->getName() + "': " + std::to_string(confirmed) + "/" + std::to_string(result.attempted) + " doors confirmed by gateway");
    if (!result.failed.empty()) {
        std::string doors;
        for (std::size_t i = 0; i < result.failed.size(); ++i) {
            if (i > 0) {
                doors += ", ";
            }
            doors += result.failed[i];
        }
        Log::line(getComponentName(), "restrict failed at " + doors + (context != nullptr ? " during incident " + context->label() : std::string()));
        return false;
    }
    return true;
}

bool AccessControlService::releaseArea(AreaComponent* area, Incident* context) {
    requireArea(area);
    Log::line(getComponentName(), "Release " + area->getKind() + " '" + area->getName() + "'");
    OperationResult result;
    {
        Log::Scope scope;
        area->unlock(result);
    }
    int confirmed = result.attempted - static_cast<int>(result.failed.size());
    Log::line(getComponentName(), "unlock of '" + area->getName() + "': " + std::to_string(confirmed) + "/" + std::to_string(result.attempted) + " doors confirmed by gateway");
    if (!result.failed.empty()) {
        std::string doors;
        for (std::size_t i = 0; i < result.failed.size(); ++i) {
            if (i > 0) {
                doors += ", ";
            }
            doors += result.failed[i];
        }
        Log::line(getComponentName(), "unlock failed at " + doors + (context != nullptr ? " during incident " + context->label() : std::string()));
        return false;
    }
    return true;
}

void AccessControlService::printAccessReport(AreaComponent* area) const {
    requireArea(area);
    Log::line(getComponentName(), "Access report for '" + area->getName() + "':");
    Log::Scope scope;
    reportNode(area, 0);
}

void AccessControlService::printCampusReport() const {
    printAccessReport(&campusRoot_);
}

void AccessControlService::requireArea(const AreaComponent* area) {
    if (area == nullptr) {
        throw std::invalid_argument("unknown area: it is not part of the campus model");
    }
}
