#ifndef WISKR_STATE_H
#define WISKR_STATE_H

#include <wiskr/wiskr_core_export.h>

#include <string>
#include <vector>

namespace whisker {
    using CommandError = std::string;

    /**
     * The State class represents the current state of the application that can be modified by commands and queried for errors.
     */
    class WISKRCORE_EXPORT State {
            public:
            using Errors = std::vector<CommandError>;

            /**
             * Reports an error that occurred during command execution. The error message will be stored in the state and can be retrieved later for debugging or user feedback purposes.
             * @param error The human-readable error message to report.
             */
            void reportError(CommandError error);

            /**
             * Checks if any errors have been reported in the state. This can be used by commands or the application to determine if there were issues during command execution.
             * @return true if there are one or more errors reported, false otherwise.
             */
            bool hasErrors() const;

            /**
             * Retrieves a copy of the list of errors that have been reported in the state. This allows callers to inspect the individual error messages without modifying the internal state of the State object.
             * @return
             */
            Errors errors() const;

            /**
            * Generates a summary string of all reported errors, concatenating them with newlines. This can be useful for displaying a summary of errors to the user or for logging purposes.
            * @return A single string containing all error messages separated by newlines. If there are no errors, this will return an empty string.
            */
            std::string errorSummary() const;

            private:
            std::vector<CommandError> errors_; ///< Internal storage for the list of reported errors.
    };

} // namespace whisker


#endif //WISKR_STATE_H
