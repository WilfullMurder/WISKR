#ifndef WISKR_WAIT_FOR_ITEM_H
#define WISKR_WAIT_FOR_ITEM_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

#include <chrono>
#include <future>

/**
 * @file `libs/core/include/wiskr/commands/wait_for_item.h`
 * @brief Command that waits for a scene item to appear (with timeout) and reports the result.
 *
 * The `WaitForItem` command is intended to be enqueued on the Dispatcher and
 * executed on the main scene thread. It inspects the Scene for the presence
 * of an item identified by an `ItemPath`. If the item is found before the
 * configured timeout expires, the command fulfills the provided `std::promise<bool>`
 * with `true`. If the timeout elapses without the item appearing, the command
 * becomes executable to allow the Dispatcher to proceed; the waiting logic is
 * implemented via `canExecute`.
 *
 * The command uses `canExecute` to repeatedly check for the item's presence
 * and to implement timeout behaviour without blocking the scene thread.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command that waits for an item to appear in the Scene.
         *
         * Construct with the target item path, a timeout duration and a promise
         * that will be satisfied with the result (`true` if the item was found,
         * otherwise `false` or left unsatisfied if the promise has no receiver).
         *
         * Lifecycle:
         * - When enqueued the Dispatcher will call `canExecute` repeatedly.
         * - If the item is present `canExecute` returns true and `execute` will
         *   be invoked; `execute` sets the promise to true.
         * - If the timeout elapses `canExecute` returns true to allow the queue
         *   to progress; the command may set the promise to false or rely on
         *   external handling depending on the implementation of `execute`.
         */
        class WISKRCORE_EXPORT WaitForItem : public Command {
        public:

            /**
             * @brief Create a WaitForItem command.
             *
             * @param targetPath Path identifying the item to wait for.
             * @param timeout Maximum duration to wait before giving up.
             * @param promise Promise to be fulfilled with the boolean result.
             *
             * The promise is moved into the command so the caller's future will
             * receive the value once the command completes.
             */
            WaitForItem(const ItemPath& targetPath, std::chrono::milliseconds timeout, std::promise<bool> promise);


            /**
             * @brief Execute the command on the scene thread.
             *
             * If the item exists when executed this method should set the
             * provided promise to `true`. If the item cannot be found this
             * implementation may leave error reporting to `canExecute`/timeout
             * logic or set the promise appropriately.
             *
             * @param env Execution environment providing access to Scene and State.
             */
            void execute(Environment&) override;


            /**
             * @brief Determine whether the command is ready to execute.
             *
             * This method:
             *  - returns true if the target item exists in the Scene (so `execute`
             *    will run and satisfy the promise with `true`),
             *  - returns true if the configured timeout has elapsed (allowing the
             *    Dispatcher to advance the queue),
             *  - otherwise returns false while waiting, starting an internal timer
             *    on the first invocation.
             *
             * @param env Execution environment providing access to Scene and State.
             * @return true if the command may be executed now, false to defer.
             */
            bool canExecute(Environment&) const override;

        private:
            ItemPath targetPath_; ///< The path to the target item to wait for in the Scene.
            std::chrono::milliseconds timeout_; ///< The maximum duration to wait for the item to appear before timing out.
            std::promise<bool> promise_; ///< The promise to be fulfilled with the result of whether the item was found before the timeout.
            mutable bool timerStarted_ = false; ///< Indicates whether the internal timer has been started to track the timeout duration.
            mutable std::chrono::steady_clock::time_point startTime_; ///< The time point when the timer was started to measure elapsed time against the timeout.
        };

    } // namespace cmd
} // namespace whisker


#endif //WISKR_WAIT_FOR_ITEM_H
