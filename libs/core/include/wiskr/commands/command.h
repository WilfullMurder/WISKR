 patt#ifndef WISKR_COMMAND_H
#define WISKR_COMMAND_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/dispatch/environment.h>

namespace whisker {
    namespace cmd {

        /**
         * The Command class represents an action that can be executed to modify the state of the application.
         */
        class WISKRCORE_EXPORT Command {
        public:
            virtual ~Command() = default;

            /**
             * Executes the command, modifying the state of the application as needed.
             * @param env The environment in which the command is executed, providing access to the current scene and state.
             */
            virtual void execute(Environment& env) = 0;

            /**
             * Determines whether the command can be executed in the current environment
             * @param env The environment to check for command execution conditions.
             * @return true if the command should not be executed at this time, false if it can be executed.
             * @note By default, this method returns true, indicating that the command can be executed.
             * Subclasses can override this method to implement specific conditions under which the command should or should not be executed.
             */
            virtual bool canExecute(Environment& env) const;
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_COMMAND_H
