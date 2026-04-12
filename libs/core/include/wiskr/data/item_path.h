#ifndef WISKR_ITEM_PATH_H
#define WISKR_ITEM_PATH_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/data/item_path_component.h>
#include <string>
#include <vector>

namespace  whisker{

    /**
     * @file item_path.h
     * @brief Represents a hierarchical path of item selectors used to locate items within a scene or document.
     *
     * The ItemPath class stores a sequence of path::Component values that describe
     * how to traverse or match items. It provides convenience constructors from
     * textual representations and accessors to inspect or extract subpaths.
     */


    /**
     * @class ItemPath
     * @brief Encapsulates a sequence of path components that identify items.
     *
     * An ItemPath is an ordered collection of `path::Component` elements. Each
     * component describes a single selection step (for example, by name, type or property).
     */
    class WISKRCORE_EXPORT ItemPath {
    public:

        /**
         * @brief Default-constructs an empty ItemPath.
         *
         * The resulting path has no components and its length() is zero.
         */
        ItemPath();

        /**
         * @brief Constructs an ItemPath by parsing a C-style string representation.
         * @param path Null-terminated string containing the path representation.
         *
         * Parsing rules are implementation-defined and handled in the corresponding source file.
         */
        ItemPath(const char* path);

        /**
         * @brief Constructs an ItemPath by parsing a std::string representation.
         * @param path String containing the path representation.
         *
         * Parsing rules are implementation-defined and handled in the corresponding source file.
         */
        ItemPath(const std::string& path);

        /**
         * @brief Constructs an ItemPath from an explicit list of components.
         * @param components A vector of pre-constructed path components.
         *
         * The provided components are copied into the new ItemPath.
         */
        ItemPath(const std::vector<path::Component>& components);


        /**
         * @brief Returns a const reference to the stored components.
         * @return const std::vector<path::Component>& The sequence of components in this path.
         */
        const std::vector<path::Component>& components() const;

        /**
         * @brief Returns the number of components in the path.
         * @return size_t Number of components.
         */
        size_t length() const {return components_.size();}

        /**
         * @brief Returns the root component of the path.
         * @return const path::Component& Reference to the first component.
         *
         * Caller must ensure the path is not empty before calling this method.
         */
        const path::Component& root() const;

        /**
         * @brief Produces a string representation of the path.
         * @return std::string A textual representation suitable for debugging or serialization.
         *
         * The exact formatting is defined by the implementation.
         */
        std::string string() const;


        /**
         * @brief Returns a subpath starting at the given offset.
         * @param offset The index of the first component to include in the returned subpath.
         * @return ItemPah A new ItemPath containing the components from \p offset to the end.
         *
         * Note: The declaration below is the original from the selection; the intended
         * return type is the ItemPath class above.
         */
        ItemPath subPath(size_t offset) const;

    private:
        std::vector<path::Component> components_; ///< Internal storage for the path components.
    };

} // namespace whisker

#endif //WISKR_ITEM_PATH_H
