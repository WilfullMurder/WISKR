#ifndef WISKR_QT_ITEM_TOOLS_H
#define WISKR_QT_ITEM_TOOLS_H

#include <wiskr/wiskr_qtquick_export.h>
#include <skitr/core/value.h>

#include <QDateTime>
#include <QObject>
#include <QQuickItem>
#include <QVariant>

#include <functional>

/**
 * @file libs/scenes/QtQuick/include/wiskr/qt_item_tools.h
 * @brief Helper utilities for interacting with Qt Quick items and converting
 *        between engine value types and Qt QVariant/QMetaType types.
 *
 * This header declares utility functions used by the Wiskr QtQuick module to:
 *  - Inspect and traverse QQuickItem / QObject hierarchies (including Repeater support).
 *  - Convert between skitter::core::Value and QVariant.
 *  - Discover QMetaMethod overloads for invocation and prepare QGenericArgument
 *    lists for QMetaMethod::invoke.
 *
 * The implementation is found in `libs/scenes/QtQuick/src/qt_item_tools.cpp`.
 */

class QString;

namespace whisker {
    namespace qt {

        extern const QString repeater_class_name; ///< The class name used to identify QML Repeater items.
        extern const char* item_at_method_name; ///< The name of the method used to retrieve child items from a QQuickItem, typically "itemAt" or "childAt".

        /**
         * @brief Retrieve the child item created by a QML Repeater at the given index.
         *
         * Some QML container types create children via methods instead of QObject::children().
         * This helper invokes the configured item_at_method_name on the repeater and returns
         * the QQuickItem pointer for a valid index or nullptr on failure.
         *
         * @param repeater Pointer to the QQuickItem representing the repeater container.
         * @param index Zero-based index of the desired delegate instance.
         * @return QQuickItem* Pointer to the delegate instance or nullptr if not present.
         */
        WISKRQTQUICK_EXPORT QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index);

        /**
         * @brief Get a meaningful object name for debugging and selection.
         *
         * Returns:
         *  - the QObject::objectName() if set,
         *  - otherwise attempts to get a name from the QQmlContext (nameForObject),
         *  - otherwise returns the (possibly empty) objectName().
         *
         * @param obj QObject pointer (may be nullptr).
         * @return QString A non-null QString describing the object name or empty string.
         */
        WISKRQTQUICK_EXPORT QString GetObjectName(QObject* obj);

        /**
         * @brief Produce a simplified type name for a QObject intended for selector matching.
         *
         * Strips common Qt Quick prefixes such as "QQuick" and internal suffixes like "_QML"
         * so that QML types can be matched by their delegate type names used in tests or selectors.
         *
         * @param obj QObject pointer (may be nullptr).
         * @return QString Simplified type string, or empty string if obj is null.
         */
        WISKRQTQUICK_EXPORT QString TypeStringForObject(QObject* obj);

        /**
         * @brief Iterate over the logical children of @p parent, invoking @p callback for each.
         *
         * Special-cases QQuickRepeater-like containers by using RepeaterChildAtIndex to enumerate
         * delegate-created items. The callback should return true to continue iteration, or
         * false to stop early.
         *
         * @param parent The QObject whose children should be iterated (may be QQuickItem or generic QObject).
         * @param callback Function called for each child. Return false to stop iteration early.
         */
        WISKRQTQUICK_EXPORT void ForEachChild(QObject* parent, const std::function<bool(QObject*)>& callback);

        using QMLReturnVariant = std::variant<std::nullptr_t, bool, int, float, double, QString, QDateTime, QVariant>;
        /**
          * @brief Prepare a QGenericReturnArgument for the given QMetaType id.
          *
          * Fills @p retVar with a storage object of the appropriate type and returns a
          * QGenericReturnArgument pointing to that storage. The caller must keep @p retVar
          * alive until after QMetaMethod::invoke returns.
          *
          * @param type QMetaType id (e.g. QMetaType::Type::Int or QMetaType id from QMetaMethod::returnType()).
          * @param retVar Output variant that will hold the storage object for the return value.
          * @return QGenericReturnArgument A wrapper suitable for passing to QMetaMethod::invoke.
          */
        WISKRQTQUICK_EXPORT QGenericReturnArgument GetReturnArgForQMetaType(int type, QMLReturnVariant& retVar);

        /**
         * @brief Convert a skitter::core::Value into a QVariant.
         *
         * Supports primitive types, strings, datetime, arrays, objects (maps), and binary data.
         * Throws std::runtime_error for unsupported value types.
         *
         * @param val Input engine-level Value.
         * @return QVariant Converted QVariant representation.
         */
        WISKRQTQUICK_EXPORT QVariant ValueToQVariant(const skitter::core::Value& val);

        /**
         * @brief Convert a QVariant into a skitter::core::Value.
         *
         * Converts common Qt metatypes (bool/int/double/QString/QDateTime), QJSValue,
         * QByteArray (binary), QVariantList (array), and QVariantMap (object) into the
         * engine Value representation. Will throw std::runtime_error for unknown/unhandled types.
         *
         * @param var Input QVariant to convert.
         * @return skitter::core::Value Converted engine value.
         */
        WISKRQTQUICK_EXPORT skitter::core::Value QVariantToValue(const QVariant& var);

        /**
         * @brief Convert a QMLReturnVariant (the type used to receive return values from invoke)
         *        into a skitter::core::Value.
         *
         * This helper handles the small set of types stored in QMLReturnVariant and delegates
         * to QVariantToValue for the QVariant fallback case.
         *
         * @param var Input QMLReturnVariant holding the return value storage.
         * @return skitter::core::Value Engine value representing the return.
         */
        WISKRQTQUICK_EXPORT skitter::core::Value QMLReturnVariantToValue(const QMLReturnVariant& var);

        /**
         * @brief Check whether each QVariant in @p varargs can be converted to the corresponding
         *        parameter type of @p metaMethod.
         *
         * Note: This performs a "canConvert" check against the QMetaType for each parameter;
         * it does not perform the actual conversion.
         *
         * @param metaMethod Meta method whose parameter types are checked.
         * @param varargs Vector of candidate QVariant arguments.
         * @return bool true if the argument count matches and all args can be converted.
         */
        WISKRQTQUICK_EXPORT bool CanConvertArgTypes(const QMetaMethod& metaMethod, const std::vector<QVariant>& varargs);

        /**
         * @brief Find a QMetaMethod on @p obj with name @p method that can accept @p varargs.
         *
         * Iterates over methods declared on @p obj and selects an overload whose name matches
         * and for which CanConvertArgTypes returns true. If found, @p ret is set to the matching
         * QMetaMethod and the function returns true.
         *
         * @param obj QObject to inspect (const reference).
         * @param method Method name to search for.
         * @param varargs Candidate argument list as QVariants.
         * @param ret Output QMetaMethod that matches the signature, if any.
         * @return bool true if a matching method was found.
         */
        WISKRQTQUICK_EXPORT bool GetMethodMetaForArgs(const QObject& obj, const std::string& method, const std::vector<QVariant>& varargs, QMetaMethod& ret);

        /**
         * @brief Convert provided QVariants to the parameter types required by @p method and
         *        create a vector of QGenericArgument suitable for passing to QMetaMethod::invoke.
         *
         * The function:
         *  - Verifies convertible types for the method's parameter count.
         *  - Converts QVariants in-place where necessary.
         *  - Builds up to 10 QGenericArgument entries (matching QMetaMethod::invoke varargs limit)
         *    filling unused slots with default-constructed QGenericArgument.
         *
         * The function sets @p ok to true on success; on failure it returns an empty vector and
         * sets @p ok to false. The caller must ensure that the QVariant instances used as storage
         * (contained in the provided @p varargs vector) remain alive until after invoke completes.
         *
         * @param method QMetaMethod describing the target parameter types.
         * @param varargs Vector of QVariant arguments which may be converted in-place.
         * @param ok Output flag set to true when conversion and argument creation succeed.
         * @return std::vector<QGenericArgument> Vector of up to 10 QGenericArgument entries.
         */
        WISKRQTQUICK_EXPORT std::vector<QGenericArgument> ConvertAndCCreateQArgumentsForMethod(const QMetaMethod& method, std::vector<QVariant>& varargs, bool& ok);

        /**
         * @brief Convert a single QVariant @p v to the provided @p targetType using QMetaType conversion.
         *
         * If conversion succeeds returns true. On failure logs a warning (via qWarning) with details.
         *
         * @param v Variant to convert (modified in-place on success).
         * @param targetType QMetaType id to convert to.
         * @param index Argument index used for diagnostics.
         * @param method QMetaMethod used for diagnostics in log output.
         * @return bool true if conversion succeeded.
         */
        WISKRQTQUICK_EXPORT bool ConvertArg(QVariant& v, int targetType, int index, const QMetaMethod& method);
    } // namespace qt
} // namespace whisker

#endif //WISKR_QT_ITEM_TOOLS_H
