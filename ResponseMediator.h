#ifndef CAMPUSGUARD_RESPONSEMEDIATOR_H
#define CAMPUSGUARD_RESPONSEMEDIATOR_H

class ResponseComponent;
struct CoordinationEvent;

class ResponseMediator {
public:
    ResponseMediator() {}
    virtual ~ResponseMediator() {}
    ResponseMediator(const ResponseMediator&) = delete;
    ResponseMediator& operator=(const ResponseMediator&) = delete;

    virtual void notify(ResponseComponent& sender, const CoordinationEvent& event) = 0;
};

#endif
