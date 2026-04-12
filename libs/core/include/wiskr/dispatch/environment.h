#ifndef WISKR_ENVIRONMENT_H
#define WISKR_ENVIRONMENT_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/dispatch/state.h>

#include <string>

namespace whisker {
    class Scene;


    using CommandError = std::string;

    /**
     * The Environment class provides the context in which commands are executed, giving them access to the current scene and state.
     * Commands can use the Environment to query or modify the scene and report errors to the state during execution.
     */
    class WISKRCORE_EXPORT Environment{
            public:
            Environment(Scene& scene, State& state);
            Scene& scene() const;
            State& state() const;

            private:
            Scene& scene_; ///< Reference to the current scene that commands can modify or query during execution.
            State& state_; ///< Reference to the current state that commands can report errors to during execution.
    };
} // namespace whisker

#endif //WISKR_ENVIRONMENT_H
