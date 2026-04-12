#ifndef WISKR_CUSTOM_COMMAND_H
#define WISKR_CUSTOM_COMMAND_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/dispatch/environment.h>
#include <functional>

/**
 * @file libs/core/include/wiskr/commands/custom_command.h
 * @brief Command that executes a custom function when executed.
 *
 * This header declares the CustomCommand class, which allows users to define
 * arbitrary functions to be executed as commands within the WISKR command
 * system. The CustomCommand takes a function that receives an Environment
 * reference, enabling it to interact with the scene and state during execution.
 *
 * This is useful for scenarios where predefined command types do not cover
 * specific use cases, allowing for flexible extension of command behavior
 * without modifying the core command hierarchy.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief A command that executes a custom function when executed.
         *
         * This command allows you to define a custom function that will be called when the command is executed. The function receives an Environment object, which provides access to the scene and state, allowing you to perform any necessary operations within the command's execution context.
         */
        class WISKRCORE_EXPORT CustomCommand : public Command {
        public:
            using ExecFunction = std::function<void(Environment&)>;
            using CanExecFunction = std::function<bool()>;

            /**
             * @brief Construct a CustomCommand with the given execution and can-execute functions.
             *
             * @param exec The function to execute when the command is executed. This function should take an Environment reference and perform the desired operations.
             * @param canExec An optional function that determines whether the command can be executed. If not provided, the command will always be executable.
             */
            CustomCommand(ExecFunction exec, CanExecFunction canExec);

            void execute(Environment&) override;
            bool canExecute(Environment&) const override;

        private:
            ExecFunction exec_; ///< The function to execute when the command is executed.
            CanExecFunction canExec_; ///< The function that determines whether the command can be executed.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_CUSTOM_COMMAND_H
