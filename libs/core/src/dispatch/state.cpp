#include <wiskr/dispatch/state.h>

namespace whisker {
    void State::reportError(CommandError error){
        errors_.emplace_back(std::move(error));
    }

    bool State::hasErrors() const {
        return errors_.size() > 0;
    }

    State::Errors State::errors() const{
        return errors_;
    }

    std::string State::errorSummary() const{
        std::string sum;
        for(auto& err: errors_){
            if(sum.length() > 0){
                sum += "\n";
            }
            sum += err;
        }
        return sum;
    }
}
