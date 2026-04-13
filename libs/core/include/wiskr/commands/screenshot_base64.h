#ifndef WISKR_SCREENSHOT_BASE64_H
#define WISKR_SCREENSHOT_BASE64_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

#include <future>

/**
 * @file `libs/core/include/wiskr/commands/screenshot_base64.h`
 * @brief Command to capture a scene item's screenshot and return it as Base64.
 *
 * The `ScreenshotBase64` command is enqueued on the Dispatcher and executed on
 * the main scene thread. When executed it resolves the target item identified
 * by an `ItemPath`. If the item exists the Scene is used to capture an image
 * and return a Base64 encoded string via the provided `std::promise<std::string>`.
 * If the item cannot be found an error is reported to the shared `State` and
 * an empty string is set on the promise.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that captures a screenshot and fulfills a promise with Base64 data.
         *
         * Instances hold the target item path and a moved-in `std::promise<std::string>`.
         * Execution runs on the scene thread and must:
         *  - resolve the item via `env.scene().itemAt(targetPath_)`
         *  - on failure: report an error to `env.state()` and set the promise to an empty string
         *  - on success: call `env.scene().takeScreenshotBase64(targetPath_)` and set the promise
         *    with the resulting Base64 string.
         */
        class WISKRCORE_EXPORT ScreenshotBase64 : public Command {
        public:

            /**
             * @brief Construct a ScreenshotBase64 command.
             *
             * @param targetPath Path identifying the item to capture.
             * @param promise Promise to be fulfilled with the Base64 image string.
             *
             * The promise is moved into the command so that the caller's future can
             * later receive the result when the command executes on the scene thread.
             */
            ScreenshotBase64(const ItemPath& targetPath, std::promise<std::string> promise);


            /**
             * @brief Execute the command.
             *
             * Resolves the target item via the provided Environment's Scene. On
             * failure reports an error to Environment::state() and sets the promise
             * to an empty string. On success obtains the Base64 representation from
             * the Scene and sets the promise with it.
             *
             * @param env Execution environment providing access to Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item to capture in the screenshot.
            std::promise<std::string> promise_; ///< Promise to be fulfilled with the Base64 image string result of the screenshot operation.
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_SCREENSHOT_BASE64_H
