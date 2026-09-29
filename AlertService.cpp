#include "AlertService.h"

#include <algorithm>
#include <stdexcept>

#include "AreaComponent.h"
#include "CoordinationEvent.h"
#include "Incident.h"
#include "Log.h"

AlertService::AlertService() : ResponseComponent("AlertService"), nextAlertId_(1) {}

AlertService::~AlertService() {}

int AlertService::activateAlert(AreaComponent* area, AlertLevel level, const std::string& message, Incident* incident) {
    requireArea(area);
    ActiveAlert alert = {nextAlertId_++, area, level, message, incident};
    alerts_.push_back(alert);
    Log::line(getComponentName(), toString(level) + " alert #" + std::to_string(alert.id) + " ACTIVE in " + area->getName() + ": " + message);
    return alert.id;
}

void AlertService::deactivateAlert(int alertId) {
    std::vector<ActiveAlert>::iterator it = std::find_if(alerts_.begin(), alerts_.end(), [alertId](const ActiveAlert& alert) { return alert.id == alertId; });
    if (it == alerts_.end()) {
        throw std::logic_error("alert #" + std::to_string(alertId) + " is not active");
    }
    Log::line(getComponentName(), "Alert #" + std::to_string(alertId) + " in " + it->area->getName() + " withdrawn");
    alerts_.erase(it);
}

void AlertService::issueEvacuation(AreaComponent* area, Incident* incident) {
    requireArea(area);
    if (isEvacuating(area)) {
        throw std::logic_error("an evacuation of " + area->getName() + " is already in progress");
    }
    Evacuation evacuation = {area, incident};
    evacuations_.push_back(evacuation);
    Log::line(getComponentName(), "EVACUATION ordered for " + area->getName() + ": all occupants leave by the nearest exit");
    changed(CoordinationEvent(EventType::EvacuationIssued, incident, area, nullptr));
}

void AlertService::endEvacuation(AreaComponent* area) {
    std::vector<Evacuation>::iterator it = std::find_if(evacuations_.begin(), evacuations_.end(), [area](const Evacuation& e) { return e.area == area; });
    if (it == evacuations_.end()) {
        throw std::logic_error("no evacuation in progress for that area");
    }
    Log::line(getComponentName(), "Evacuation order for " + area->getName() + " rescinded");
    evacuations_.erase(it);
}

void AlertService::notifyArea(AreaComponent* area, const std::string& message) {
    requireArea(area);
    Log::line(getComponentName(), "Broadcast to " + area->getName() + ": " + message);
}

void AlertService::clearIncident(Incident* incident) {
    std::size_t alertsBefore = alerts_.size();
    std::size_t evacuationsBefore = evacuations_.size();
    alerts_.erase(std::remove_if(alerts_.begin(), alerts_.end(), [incident](const ActiveAlert& a) { return a.incident == incident; }), alerts_.end());
    evacuations_.erase(std::remove_if(evacuations_.begin(), evacuations_.end(), [incident](const Evacuation& e) { return e.incident == incident; }), evacuations_.end());
    Log::line(getComponentName(), "Cleared " + std::to_string(alertsBefore - alerts_.size()) + " alert(s) and " + std::to_string(evacuationsBefore - evacuations_.size()) + " evacuation order(s) linked to incident " + incident->label());
}

void AlertService::printStatus() const {
    Log::line(getComponentName(), std::to_string(alerts_.size()) + " active alert(s), " + std::to_string(evacuations_.size()) + " evacuation(s) in progress");
    Log::Scope scope;
    for (const ActiveAlert& alert : alerts_) {
        Log::line("Alert", "#" + std::to_string(alert.id) + " " + toString(alert.level) + " in " + alert.area->getName() + ": " + alert.message);
    }
    for (const Evacuation& evacuation : evacuations_) {
        Log::line("Evacuation", evacuation.area->getName() + (evacuation.incident != nullptr ? " (incident " + evacuation.incident->label() + ")" : std::string()));
    }
}

bool AlertService::isEvacuating(const AreaComponent* area) const {
    for (const Evacuation& evacuation : evacuations_) {
        if (evacuation.area == area) {
            return true;
        }
    }
    return false;
}

void AlertService::requireArea(const AreaComponent* area) {
    if (area == nullptr) {
        throw std::invalid_argument("unknown area: it is not part of the campus model");
    }
}
