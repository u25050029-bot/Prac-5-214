#ifndef CAMPUSGUARD_AREAGROUP_H
#define CAMPUSGUARD_AREAGROUP_H

#include <memory>
#include <string>
#include <vector>

#include "AreaComponent.h"

class AreaGroup : public AreaComponent {
public:
    AreaGroup(const std::string& name, const std::string& kind);
    ~AreaGroup() override;

    template <typename T>
    T* add(std::unique_ptr<T> child) {
        T* raw = child.get();
        adopt(std::unique_ptr<AreaComponent>(std::move(child)));
        return raw;
    }

    void lock(OperationResult& result) override;
    void unlock(OperationResult& result) override;
    void restrictAccess(AccessLevel level, OperationResult& result) override;
    int doorCount() const override;
    int securedCount() const override;
    std::string statusText() const override;
    std::size_t childCount() const override;
    AreaComponent* childAt(std::size_t index) const override;

private:
    void adopt(std::unique_ptr<AreaComponent> child);
    void announce(const std::string& operation) const;

    std::vector<std::unique_ptr<AreaComponent>> children_;
};

#endif
