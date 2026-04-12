#include <wiskr/dispatch/environment.h>


namespace whisker {

    Environment::Environment(Scene& scene, State& state) : scene_(scene), state_(state) {}

    Scene& Environment::scene() const {
        return scene_;
    }

    State& Environment::state() const {
        return state_;
    }
} // namespace whisker