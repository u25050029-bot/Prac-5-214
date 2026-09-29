#ifndef CAMPUSGUARD_RESPONSECOMPONENT_H
#define CAMPUSGUARD_RESPONSECOMPONENT_H

#include <string>

class ResponseMediator;
struct CoordinationEvent;

class ResponseComponent {
public:
    explicit ResponseComponent(const std::string& componentName);
    virtual ~ResponseComponent();
    ResponseComponent(const ResponseComponent&) = delete;
    ResponseComponent& operator=(const ResponseComponent&) = delete;

    void setMediator(ResponseMediator* mediator);
    const std::string& getComponentName() const;

protected:
    void changed(const CoordinationEvent& event);

private:
    std::string componentName_;
    ResponseMediator* mediator_;
};

#endif
