#ifndef WISKR_GET_TEST_STATUS_H
#define WISKR_GET_TEST_STATUS_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/commands/command.h>
#include <wiskr/data/item_path.h>

#include <future>

/**
 * @file libs/core/include/wiskr/commands/get_test_status.h
 * @brief Command to retrieve the status of tests in the scene.
 *
 * This header declares the GetTestStatus command, which is used to obtain the
 * status of tests in the scene. The command executes on the main scene thread
 * and fulfills a std::promise<std::vector<std::string>> with the status of
 * tests if found, or an empty vector if not.
 *
 * The command can be submitted to a Dispatcher and will safely access the scene's
 * item hierarchy to retrieve test status information.
 */

namespace whisker {
    namespace cmd {


        /**
         * @brief Command that retrieves test status strings from the Environment's State.
         *
         * The command is intended for synchronous queries from other threads. A
         * std::promise<std::vector<std::string>> is moved into the command and
         * will be satisfied with the list of error/status strings when executed
         * on the scene thread.
         */
        class WISKRCORE_EXPORT GetTestStatus : public Command {
        public:
            using StatusStrings = std::vector<std::string>; ///< Alias for the type of status strings returned by the command.


            /**
             * @brief Construct a GetTestStatus command.
             *
             * @param errorsOnly If true the caller indicates interest only in error entries.
             *                   Note: the current implementation ignores this flag but it
             *                   exists for API symmetry and future extension.
             * @param promise A std::promise<StatusStrings> that will be fulfilled with the
             *                collected status strings when the command executes. The
             *                promise is moved into the command.
             */
            GetTestStatus(bool errorsOnly, std::promise<StatusStrings> promise);

            /**
             * @brief Execute the command using the provided Environment.
             *
             * On execution the command obtains the list of errors/status strings from
             * env.state().errors() and sets the moved-in promise value with that list.
             *
             * @param env Execution Environment providing access to the Scene and State.
             */
            void execute(Environment& env) override;

        private:
            std::promise<StatusStrings> promise_; ///< The promise to be fulfilled with the status strings when the command executes.
        };

    } // namespace cmd
} // namespace whisker

#endif //WISKR_GET_TEST_STATUS_H
