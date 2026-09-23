#ifndef CAMPUSGUARD_AREACOMPONENT_H
#define CAMPUSGUARD_AREACOMPONENT_H

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "Aggregate.h"
#include "Iterator.h"
#include "Types.h"

struct OperationResult {
    OperationResult() : attempted(0) {}
    int attempted;
    std::vector<std::string> failed;
};

class AreaComponent : public Aggregate<AreaComponent*> {
    friend class AreaGroup;

public:
    AreaComponent(const std::string& name, const std::string& kind);
    ~AreaComponent() override;

    const std::string& getName() const;
    const std::string& getKind() const;
    int depth() const;

    virtual void lock(OperationResult& result) = 0;
    virtual void unlock(OperationResult& result) = 0;
    virtual void restrictAccess(AccessLevel level, OperationResult& result) = 0;
    virtual int doorCount() const = 0;
    virtual int securedCount() const = 0;
    virtual std::string statusText() const = 0;
    virtual std::size_t childCount() const;
    virtual AreaComponent* childAt(std::size_t index) const;

    std::unique_ptr<Iterator<AreaComponent*>> createIterator() override;

private:
    std::string name_;
    std::string kind_;
    AreaComponent* parent_;
};

#endif
