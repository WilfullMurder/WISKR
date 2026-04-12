#include <wiskr/commands/custom_command.h>


namespace whisker {
    namespace cmd {

        CustomCommand::CustomCommand(ExecFunction exec, CanExecFunction canExec): exec_(exec), canExec_(canExec) {}

        void CustomCommand::execute(Environment& env) {
            exec_(env);
        }

        bool CustomCommand::canExecute(Environment&) const {
            return canExec_();
        }
    } // namespace cmd
} // namespace whisker