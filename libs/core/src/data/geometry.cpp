#include <wiskr/data/geometry.h>

namespace whisker {

    Point::Point() : x(0), y(0) {}

    Point::Point(Real x, Real y) : x(x), y(y) {}

    Size::Size() : width(0), height(0) {}

    Size::Size(Real width, Real height) : width(width), height(height) {}

    Rect::Rect(){}

    Rect::Rect(Real x, Real y, Real w, Real h) : topLeft(x, y), size(w, h) {}

} // namespace whisker