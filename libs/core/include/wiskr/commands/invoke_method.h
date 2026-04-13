#ifndef WISKR_INVOKE_METHOD_H
#define WISKR_INVOKE_METHOD_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>
#include <skitr/core/value.h>

#include <future>

/**
 * @file `libs/core/include/wiskr/commands/invoke_method.h`
 * @brief Command to invoke a named method on a scene item and return its result.
 *
 * This header declares the InvokeMethod command which is posted to the
 * Dispatcher and executed on the Scene thread. The command resolves the item
 * identified by an ItemPath, attempts to invoke the specified method with the
 * provided arguments and fulfills a std::promise\<skitter::core::Value\> with
 * the returned value. On failure the command reports an error via the shared
 * State and sets an empty Value on the promise.
 */
namespace whisker {
    namespace cmd {

        /**
       * @brief Command that invokes a method on a scene item.
       *
       * InvokeMethod stores the item path, the method name and the arguments to
       * pass to the method. A std::promise\<skitter::core::Value\> is moved into
       * the command and will be satisfied with the method's return value when
       * the command executes on the scene thread.
       */
        class WISKRCORE_EXPORT InvokeMethod : public Command {
        public:

            /**
             * @brief Construct an InvokeMethod command.
             *
             * @param targetPath Path identifying the item on which to invoke the method.
             * @param methodName Name of the method to invoke on the target item.
             * @param args Vector of arguments to pass to the invoked method.
             * @param promise Promise that will be fulfilled with the method's return
             *                value when execution completes. The promise is moved
             *                into the command.
             */
            InvokeMethod(const ItemPath& targetPath, const std::string& methodName, const std::vector<skitter::core::Value>& args, std::promise<skitter::core::Value> promise);


            /**
             * @brief Execute the command using the provided Environment.
             *
             * Behavior:
             * - Resolve the target item via env.scene().itemAt(targetPath_).
             * - If the item is not found: report an error on env.state() and set
             *   the promise to an empty skitter::core::Value.
             * - Otherwise call item->invokeMethod(methodName_, args_, ret). If the
             *   invocation fails, report an error and set an empty Value on the
             *   promise. On success set the promise with the returned Value.
             *
             * @param env Execution Environment providing access to Scene and State.
             */
            void execute(Environment& env) override;

        private:
            ItemPath targetPath_; ///< The path to the target item on which to invoke the method.
            std::string methodName_; ///< The name of the method to invoke on the target item.
            std::vector<skitter::core::Value> args_; ///< The arguments to pass to the method when invoking it.
            std::promise<skitter::core::Value> promise_; ///< The promise to be fulfilled with the method's return value when execution completes.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_INVOKE_METHOD_H
