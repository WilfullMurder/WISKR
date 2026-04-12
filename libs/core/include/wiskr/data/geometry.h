#ifndef WISKR_GEOMETRY_H
#define WISKR_GEOMETRY_H

#include <wiskr/wiskr_core_export.h>

namespace whisker {

    /**
     * @brief Floating point type used throughout the geometry utilities.
     *
     * Defined as an alias to allow easy changes to precision across the codebase.
     */
    using Real = double;

    /**
     * @brief Represents a 2D point with X and Y coordinates.
     *
     * Provides simple constructors for default initialization and explicit coordinate
     * initialization. Instances are small value objects intended for stack allocation.
     */
    struct WISKRCORE_EXPORT Point {
        Point();
        Point(Real x, Real y);

        Real x;
        Real y;
    };

    /**
     * @brief Represents width and height dimensions.
     *
     * Useful for sizing UI elements or geometry primitives.
     */
    struct WISKRCORE_EXPORT Size {
        Size();
        Size(Real width, Real height);

        Real width;
        Real height;
    };

    /**
     * @brief Axis-aligned rectangle defined by a top-left point and a size.
     * Commonly used for bounding boxes, layout rectangles, and geometric shapes.
     */
    struct WISKRCORE_EXPORT Rect {
        Rect();
        Rect(Real x, Real y, Real width, Real height);

        Point topLeft;
        Size size;
    };

} // namespace whisker

#endif //WISKR_GEOMETRY_H
