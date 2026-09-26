#include "AccessControlService.h"

#include <memory>
#include <stdexcept>

#include "AreaComponent.h"
#include "CoordinationEvent.h"
#include "Iterator.h"
#include "Log.h"

AccessControlService::AccessControlService(AreaComponent& campusRoot)
    : ResponseComponent("AccessControl"), campusRoot_(campusRoot) {}

AccessControlService::~AccessControlService() {}

AreaComponent* AccessControlService::findArea(const std::string& name) const {
    std::unique_ptr<Iterator<AreaComponent*>> it = campusRoot_.createIterator();
    int visited = 0;
    for (it->first(); !it->isDone(); it->next()) {
        ++visited;
        if (it->currentItem()->getName() == name) {
            Log::line(getComponentName(), "AreaDepthFirstIterator found '" + name + "' after visiting " + std::to_string(visited) + " component(s)");
            return it->currentItem();
        }
    }
    Log::line(getComponentName(), "AreaDepthFirstIterator visited all " + std::to_string(visited) + " component(s); no area named '" + name + "'");
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
    return conclude("lock", area, context, result, true);
}

bool AccessControlService::restrictArea(AreaComponent* area, AccessLevel level, Incident* context) {
    requireArea(area);
    Log::line(getComponentName(), "Restrict " + area->getKind() + " '" + area->getName() + "' to " + toString(level));
    OperationResult result;
    {
        Log::Scope scope;
        area->restrictAccess(level, result);
    }
    return conclude("restrict", area, context, result, false);
}

bool AccessControlService::releaseArea(AreaComponent* area, Incident* context) {
    requireArea(area);
    Log::line(getComponentName(), "Release " + area->getKind() + " '" + area->getName() + "'");
    OperationResult result;
    {
        Log::Scope scope;
        area->unlock(result);
    }
    return conclude("unlock", area, context, result, false);
}

void AccessControlService::printAccessReport(AreaComponent* area) const {
    requireArea(area);
    Log::line(getComponentName(), "Access report for '" + area->getName() + "' (AreaDepthFirstIterator):");
    Log::Scope scope;
    const int baseDepth = area->depth();
    std::unique_ptr<Iterator<AreaComponent*>> it = area->createIterator();
    for (it->first(); !it->isDone(); it->next()) {
        AreaComponent* current = it->currentItem();
        std::string indent(static_cast<std::size_t>(current->depth() - baseDepth) * 2, ' ');
        Log::line("Area", indent + current->getKind() + " " + current->getName() + ": " + current->statusText());
    }
}

void AccessControlService::printCampusReport() const {
    printAccessReport(&campusRoot_);
}

bool AccessControlService::conclude(const std::string& action, AreaComponent* area, Incident* context, const OperationResult& result, bool announceSecured) {
    int confirmed = result.attempted - static_cast<int>(result.failed.size());
    Log::line(getComponentName(), action + " of '" + area->getName() + "': " + std::to_string(confirmed) + "/" + std::to_string(result.attempted) + " doors confirmed by gateway");
    if (!result.failed.empty()) {
        std::string doors;
        for (const std::string& door : result.failed) {
            doors += (doors.empty() ? "" : ", ") + door;
        }
        changed(CoordinationEvent(EventType::AccessFailure, context, area, nullptr, action + " failed at " + doors));
        return false;
    }
    if (announceSecured) {
        changed(CoordinationEvent(EventType::AreaSecured, context, area, nullptr));
    }
    return true;
}

void AccessControlService::requireArea(const AreaComponent* area) {
    if (area == nullptr) {
        throw std::invalid_argument("unknown area: it is not part of the campus model");
    }
}
