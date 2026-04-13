#include <wiskr/commands/wait_for_item.h>
#include <wiskr/scene/scene.h>

namespace whisker {
    namespace cmd {

        WaitForItem::WaitForItem(const ItemPath &targetPath, std::chrono::milliseconds timeout, std::promise<bool> promise)
                : targetPath_(targetPath), timeout_(timeout), promise_(std::move(promise)) {}

        void WaitForItem::execute(Environment &env) {
            auto item = env.scene().itemAt(targetPath_);
            if (item) {
                try {
                   promise_.set_value(true);
                } catch (const std::future_error&) {
                    // promise already satisfied or no associated future; ignore silently
                }
            }
        }

        bool WaitForItem::canExecute(Environment &env) const {
            auto item = env.scene().itemAt(targetPath_);
            if (item) {
                return true;
            }

            // If timeout is zero or negative, become ready immediately.
            if (timeout_ <= std::chrono::milliseconds::zero()) {
                return true;
            }

            if (!timerStarted_) {
                timerStarted_ = true;
                startTime_ = std::chrono::steady_clock::now();
                return false;
            }

            auto now = std::chrono::steady_clock::now();
            if (now - startTime_ >= timeout_) {
                return true;
            }

            return false;
        }
    } // namespace cmd
} // namespace whisker