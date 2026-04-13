#ifndef WISKR_WAIT_H
#define WISKR_WAIT_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>

#include <chrono>

/**
 * @file `libs/core/include/wiskr/commands/wait.h`
 * @brief Command that pauses command processing for a specified duration.
 *
 * The Wait command is enqueued on the Dispatcher and executed on the main
 * scene thread. Execution itself is a no-op; readiness is controlled via
 * canExecute which prevents the Dispatcher from executing subsequent queued
 * commands until the configured duration has elapsed.
 */

namespace whisker {
    namespace cmd {

        /**
         * @brief Command used to introduce a timed delay in the command queue.
         *
         * Construct with a duration in milliseconds. When the Dispatcher inspects
         * the front command it will repeatedly call canExecute(); this command
         * initially reports not-ready and only becomes executable after the
         * requested duration has passed. The execute method intentionally does
         * nothing — the delay behavior is implemented via canExecute.
         */
        class WISKRCORE_EXPORT Wait : public Command {
        public:

            /**
             * @brief Create a Wait command.
             *
             * @param duration Duration to wait before the command becomes executable.
             */
            explicit Wait(std::chrono::milliseconds duration);

            /**
             * @brief Execute the command.
             *
             * The execute method performs no action; the timing behavior is
             * managed by canExecute. This method will be called once the command
             * becomes ready.
             *
             * @param env Execution environment (unused).
             */
            void execute(Environment& env) override;

            /**
             * @brief Determine whether the wait period has elapsed.
             *
             * The first invocation starts an internal timer and returns false so
             * the Dispatcher will re-check later. Subsequent calls return true
             * only when the elapsed time is greater than or equal to the
             * configured duration.
             *
             * @param env Execution environment (unused).
             * @return true if the configured duration has passed, false otherwise.
             */
            bool canExecute(Environment&) const override;

        private:
            mutable bool timerStarted_ = false; ///< Indicates whether the timer has been started.
            mutable std::chrono::steady_clock::time_point startTime_; ///< The time point when the timer was started.
            std::chrono::milliseconds duration_; ///< The duration to wait before the command becomes executable.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_WAIT_H
