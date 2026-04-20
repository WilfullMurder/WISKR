#ifndef WISKR_FIND_QT_ITEM_H
#define WISKR_FIND_QT_ITEM_H

#include <wiskr/wiskr_qtquick_export.h>

#include <wiskr/data/item_path.h>

#include <QQuickItem>
#include <QQuickWindow>

/**
 * @file find_qt_item.h
 * @brief Utility functions to locate QQuickItem and QQuickWindow instances based on ItemPath.
 *
 * This header declares functions that take an ItemPath and attempt to locate the corresponding
 * QQuickItem or QQuickWindow in the scene. The implementation will traverse the Qt object hierarchy
 * according to the selectors defined in the ItemPath components, matching items by name, type, or
 * other properties as specified.
 *
 * Note: The actual matching logic and traversal are defined in the corresponding source file.
 */

namespace whisker {
    namespace qt{

        /**
         * @brief Locate a QQuickItem in the scene that matches the given ItemPath.
         * @param path ItemPath describing the hierarchical selectors to match.
         * @return Pointer to the matching QQuickItem, or nullptr if no match is found.
         */
        QQuickItem* getQQuickItemAt(const whisker::ItemPath& path);

        /**
         * @brief Locate a QQuickWindow in the scene that matches the given ItemPath.
         * @param path ItemPath describing the hierarchical selectors to match.
         * @return Pointer to the matching QQuickWindow, or nullptr if no match is found.
         */
        QQuickWindow* getQQuickWindowAt(const whisker::ItemPath& path);

    } // namespace qt
} // namespace whisker

#endif //WISKR_FIND_QT_ITEM_H
