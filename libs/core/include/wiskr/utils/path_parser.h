#ifndef WISKR_PATH_PARSER_H
#define WISKR_PATH_PARSER_H

#include <wiskr/wiskr_core_export.h>

#include <string>
#include <vector>

/**
 * @file libs/core/include/wiskr/utils/path_parser.h
 * @brief Utilities for parsing and formatting item path strings used by the WISKR scene system.
 */

namespace whisker {
    namespace utils {

        /**
         * @brief Parse a path string into its individual components.
         *
         * The input @p path is split into its logical components. Components may
         * encode selectors (for example property selectors, type selectors,
         * quoted values or property=value pairs) and are returned as strings
         * in the order they appear in the path.
         *
         * Example:
         * - Input: "Root/Child/.enabled/#Button/(id=submit)/\"Hello\""
         * - Output: { "Root", "Child", ".enabled", "#Button", "(id=submit)", "\"Hello\"" }
         *
         * The exact parsing rules mirror the component semantics used by the
         * WISKR path component utilities; callers should pass each returned
         * component to the `path::Component` constructor when building an
         * `ItemPath`.
         *
         * @param path The textual path to parse.
         * @return A vector of component strings in order. Returns an empty vector
         *         if @p path is empty or contains no components.
         */
        std::vector<std::string> ParsePathFrom(const std::string& path);

        /**
         * @brief Format a sequence of path component strings into a path string.
         *
         * The inverse of `ParsePathFrom`. Takes the vector of component strings
         * (each matching the serialized form used by `path::Component::string()`)
         * and joins them into a single path string suitable for display or
         * re-parsing.
         *
         * Example:
         * - Input: { "Root", "Child", ".enabled" }
         * - Output: "Root/Child/.enabled"
         *
         * @param components The ordered components to join.
         * @return A single path string representing the joined components. If
         *         @p components is empty an empty string is returned.
         */
        std::string FormatPath(const std::vector<std::string>& components);

    } // namespace utils
} // namespace whisker
#endif //WISKR_PATH_PARSER_H
