#ifndef WISKR_ITEM_POSITION_H
#define WISKR_ITEM_POSITION_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/data/geometry.h>
#include <wiskr/data/item_path.h>

#include <string>

namespace whisker {

    /**
     * @file item_position.h
     * @brief Helper for specifying a position of an item using a path, relative proportion and pixel offset.
     *
     * An ItemPosition couples an ItemPath (identifying an item) with a relative position
     * inside that item expressed as a proportion of the item's size and an additional
     * absolute offset. This is useful to refer to anchor points on items in a resolution-
     * independent way (via proportion) while allowing fine-grained pixel adjustments
     * (via offset).
     */
   class WISKRCORE_EXPORT ItemPosition {
    public:

       /**
         * @brief Construct an ItemPosition.
         *
         * @param path The path identifying the target item. Taken by value and stored internally.
         * @param proportion Relative position inside the item, given as a Point where
         *                   x and y are in the range [0.0, 1.0]. Defaults to the center {0.5, 0.5}.
         * @param offset Absolute pixel offset applied after computing the proportional position.
         *               Defaults to {0.0, 0.0}.
         */
        ItemPosition(ItemPath path, Point proportion = {0.5, 0.5}, Point offset = {0., 0.});

       /**
        * @brief Return the ItemPath that identifies the referenced item.
        *
        * @return const ItemPath& Reference to the stored path.
        */
        const ItemPath& itemPath() const;

       /**
        * @brief Compute a concrete point inside an item for a given item size.
        *
        * The returned point is computed as:
        *   result = (proportion_ * size) + offset_
        *
        * Where proportion_ is interpreted relative to the item's width and height,
        * and offset_ is added in the same coordinate space (typically pixels).
        *
        * @param size The size of the item to compute the absolute position for.
        * @return Point The absolute position within the item, after applying proportion and offset.
        */
        Point positionForItemSize(const Size& size) const;

    private:
        ItemPath path_; ///< The path identifying the target item.
        Point proportion_; ///< Relative position inside the item (0..1 for x and y).
        Point offset_; ///< Absolute offset applied after proportional positioning.
    };

} // namespace whisker


#endif //WISKR_ITEM_POSITION_H
