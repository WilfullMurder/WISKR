#include <wiskr/commands/wait.h>

namespace whisker {
    namespace cmd {

        Wait::Wait(std::chrono::milliseconds duration) : duration_(duration) {}

        void Wait::execute(Environment&) {
        }

        bool Wait::canExecute(Environment &) const {
            if (!timerStarted_) {
                timerStarted_ = true;
                startTime_ = std::chrono::steady_clock::now();
                return false;
            }
            auto timeSinceStart = std::chrono::steady_clock::now() - startTime_;
            return timeSinceStart >= duration_;
        }
    } // namespace cmd
} // namespace whisker