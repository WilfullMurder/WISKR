#ifndef WISKR_CUSTOM_COMMAND_H
#define WISKR_CUSTOM_COMMAND_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/dispatch/environment.h>
#include <functional>

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

            CustomCommand(ExecFunction exec, CanExecFunction canExec);

            void execute(Environment&) override;
            bool canExecute(Environment&) const override;

        private:
            ExecFunction exec_;
            CanExecFunction canExec_;
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_CUSTOM_COMMAND_H
