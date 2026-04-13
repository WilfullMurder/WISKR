#ifndef WISKR_QUIT_H
#define WISKR_QUIT_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>

/**
 * @file `libs/core/include/wiskr/commands/quit.h`
 * @brief Command used to request application shutdown via the Scene event system.
 *
 * The `Quit` command is enqueued on the Dispatcher and executed on the main
 * scene thread. When executed it emits a quit event through the Scene's
 * event interface so the application can perform an orderly shutdown.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that triggers a quit event on the scene.
         *
         * Instances of this command are intended to be posted to the Dispatcher
         * when a remote caller or test wants the application to exit. Execution
         * runs on the scene thread and delegates to Environment::scene() to
         * send the quit event.
         */
        class WISKRCORE_EXPORT Quit : public Command {
        public:

            /**
             * @brief Default constructible command.
             *
             * The command carries no additional state.
             */
            Quit() = default;

            /**
             * @brief Execute the quit command.
             *
             * Sends a quit event to the Scene via the provided Environment. Should
             * be called on the main scene thread (Dispatcher ensures this).
             *
             * @param env Execution environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_QUIT_H
