#include <wiskr/commands/command.h>


namespace whisker {
    namespace cmd {

        bool Command::canExecute(Environment& /*env*/) const {
            return true;
        }

    } // namespace cmd
} // namespace whisker