#include "AlertService.h"

#include <algorithm>
#include <stdexcept>

#include "AreaComponent.h"
#include "Incident.h"
#include "Log.h"

AlertService::AlertService() : componentName_("AlertService"), nextAlertId_(1) {}

AlertService::~AlertService() {}

const std::string& AlertService::getComponentName() const {
    return componentName_;
}

int AlertService::activateAlert(AreaComponent* area, AlertLevel level, const std::string& message, Incident* incident) {
    requireArea(area);
    ActiveAlert alert = {nextAlertId_++, area, level, message, incident};
    alerts_.push_back(alert);
    Log::line(getComponentName(), toString(level) + " alert #" + std::to_string(alert.id) + " ACTIVE in " + area->getName() + ": " + message);
    return alert.id;
}

void AlertService::deactivateAlert(int alertId) {
    for (std::size_t i = 0; i < alerts_.size(); ++i) {
        if (alerts_[i].id == alertId) {
            Log::line(getComponentName(), "Alert #" + std::to_string(alertId) + " in " + alerts_[i].area->getName() + " withdrawn");
            alerts_.erase(alerts_.begin() + i);
            return;
        }
    }
    throw std::logic_error("alert #" + std::to_string(alertId) + " is not active");
}

void AlertService::issueEvacuation(AreaComponent* area, Incident* incident) {
    requireArea(area);
    Evacuation evacuation = {area, incident};
    evacuations_.push_back(evacuation);
    Log::line(getComponentName(), "EVACUATION ordered for " + area->getName() + ": all occupants leave by the nearest exit");
}

void AlertService::endEvacuation(AreaComponent* area) {
    for (std::size_t i = 0; i < evacuations_.size(); ++i) {
        if (evacuations_[i].area == area) {
            Log::line(getComponentName(), "Evacuation order for " + area->getName() + " rescinded");
            evacuations_.erase(evacuations_.begin() + i);
            return;
        }
    }
    throw std::logic_error("no evacuation in progress for that area");
}

void AlertService::notifyArea(AreaComponent* area, const std::string& message) {
    requireArea(area);
    Log::line(getComponentName(), "Broadcast to " + area->getName() + ": " + message);
}

void AlertService::clearIncident(Incident* incident) {
    std::size_t alertsBefore = alerts_.size();
    std::size_t evacuationsBefore = evacuations_.size();
    for (std::size_t i = 0; i < alerts_.size(); ++i) {
        if (alerts_[i].incident == incident) {
            alerts_.erase(alerts_.begin() + i);
        }
    }
    for (std::size_t i = 0; i < evacuations_.size(); ++i) {
        if (evacuations_[i].incident == incident) {
            evacuations_.erase(evacuations_.begin() + i);
        }
    }
    Log::line(getComponentName(), "Cleared " + std::to_string(alertsBefore - alerts_.size()) + " alert(s) and " + std::to_string(evacuationsBefore - evacuations_.size()) + " evacuation order(s) linked to incident " + incident->label());
}

void AlertService::printStatus() const {
    Log::line(getComponentName(), std::to_string(alerts_.size()) + " active alert(s), " + std::to_string(evacuations_.size()) + " evacuation(s) in progress");
    Log::Scope scope;
    for (std::size_t i = 0; i < alerts_.size(); ++i) {
        std::string line = "#" + std::to_string(alerts_[i].id);
        line += " " + toString(alerts_[i].level);
        line += " in " + alerts_[i].area->getName();
        line += ": " + alerts_[i].message;
        Log::line("Alert", line);
    }
    for (std::size_t i = 0; i < evacuations_.size(); ++i) {
        std::string line = evacuations_[i].area->getName();
        if (evacuations_[i].incident != nullptr) {
            line += " (incident " + evacuations_[i].incident->label() + ")";
        }
        Log::line("Evacuation", line);
    }
}

void AlertService::requireArea(const AreaComponent* area) {
    if (area == nullptr) {
        throw std::invalid_argument("unknown area: it is not part of the campus model");
    }
}
