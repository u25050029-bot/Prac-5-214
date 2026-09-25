#ifndef CAMPUSGUARD_UNITROSTER_H
#define CAMPUSGUARD_UNITROSTER_H

#include <cstddef>
#include <memory>
#include <vector>

#include "Aggregate.h"
#include "Iterator.h"
#include "ResponseUnit.h"

class UnitRoster : public Aggregate<ResponseUnit*> {
public:
    UnitRoster();
    ~UnitRoster() override;

    ResponseUnit* add(std::unique_ptr<ResponseUnit> unit);
    std::size_t size() const;
    ResponseUnit* at(std::size_t index) const;

    std::unique_ptr<Iterator<ResponseUnit*>> createIterator() override;
    std::unique_ptr<Iterator<ResponseUnit*>> createAvailableIterator(UnitType type);

private:
    std::vector<std::unique_ptr<ResponseUnit>> units_;
};

#endif
