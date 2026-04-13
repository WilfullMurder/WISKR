#ifndef WISKR_SCREENSHOT_H
#define WISKR_SCREENSHOT_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

/**
 * @file `libs/core/include/wiskr/commands/screenshot.h`
 * @brief Command to capture a screenshot of a scene item and store it to disk.
 *
 * This header declares the Screenshot command which is posted to the Dispatcher
 * and executed on the main scene thread. When executed it resolves the target
 * item identified by an ItemPath and asks the Scene to write an image file at
 * the provided file path. If the item is not found an error is reported to the
 * shared State.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that requests the Scene to take a screenshot of an item.
         *
         * The Screenshot command stores a target item path and an output file path.
         * When executed on the scene thread it resolves the item via the Environment's
         * Scene. If the item exists the Scene is instructed to capture and save the
         * screenshot; otherwise the command reports an error via Environment::state().
         */
        class WISKRCORE_EXPORT Screenshot : public Command {
        public:

            /**
             * @brief Construct a Screenshot command.
             *
             * @param targetPath Path identifying the item to capture.
             * @param filePath Filesystem path where the screenshot image should be saved.
             *
             * The file path is moved into the command to avoid unnecessary copies as
             * the command may be queued before execution.
             */
            Screenshot(const ItemPath& targetPath, std::string filePath);


            /**
             * @brief Execute the screenshot command.
             *
             * Behavior:
             * - Resolve the target item via env.scene().itemAt(targetPath_).
             * - If the item is not found, report an error using env.state().reportError(...).
             * - Otherwise call env.scene().takeScreenshot(targetPath_, filePath_) to
             *   capture and persist the image.
             *
             * @param env Execution environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item to capture in the screenshot.
            std::string filePath_; ///< The filesystem path where the screenshot image should be saved.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_SCREENSHOT_H
