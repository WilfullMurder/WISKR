#include <wiskr/commands/get_test_status.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        GetTestStatus::GetTestStatus(bool /*errorsOnly*/, std::promise<StatusStrings> promise) : promise_(std::move(promise)) {}

        void GetTestStatus::execute(Environment& env)
        {
            promise_.set_value(env.state().errors());
        }
    } // namespace cmd
} // namespace whisker