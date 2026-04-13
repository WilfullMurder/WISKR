#include <wiskr/commands/quit.h>

#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        void Quit::execute(Environment& env) {
            env.scene().events().quit();
        }
    } // namespace cmd
} // namespace whisker