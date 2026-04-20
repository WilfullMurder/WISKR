#include <wiskr/qt_item_tools.h>

#include <QDateTime>
#include <QQmlContext>
#include <QQuickItem>
#include <QRegularExpression>
#include <stdexcept>

namespace whisker {
    namespace qt {
        const QString repeater_class_name = QString("QQuickRepeater");
        const char* item_at_method_name = "itemAt";

        QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index) {
            QQuickItem* retVal = nullptr;
            bool success = QMetaObject::invokeMethod(repeater, item_at_method_name, Qt::DirectConnection, Q_RETURN_ARG(QQuickItem*, retVal), Q_ARG(int, index));
            if (success) return retVal;
            return nullptr;
        }

        QString GetObjectName(QObject* obj) {
            if(!obj) return {};
            if(!obj->objectName().isEmpty()) return obj->objectName();

            QQmlContext* context = qmlContext(obj);
            if(context) return context->nameForObject(obj);

            return obj->objectName();
        }

        QString TypeStringForObject(QObject* obj) {
            if(!obj) return "";

            auto typeName = QString(obj->metaObject()->className());
            typeName.replace(QRegularExpression("QQuick|_QML"), "");
            return typeName;
        }

        void ForEachChild(QObject* parent, const std::function<bool(QObject*)>& callback) {
            if(!parent) return;

            auto parentName = parent->metaObject()->className();
            if(parentName == repeater_class_name) {
                QQuickItem* repeaterItem = static_cast<QQuickItem*>(parent);

                int i = 0;
                QQuickItem* child = nullptr;
                while((child = RepeaterChildAtIndex(repeaterItem, i++)) != nullptr) {
                    if(!callback(child)) return;
                }
            }

            if (auto quickItem = qobject_cast<QQuickItem*>(parent)) {
                for(auto* child : quickItem->childItems()) {
                    if (!callback(child)) return;
                }
            } else{
                for(auto* child : parent->children()) {
                    if (!callback(child)) return;
                }
            }
        }

        QGenericReturnArgument GetReturnArgForQMetaType(int type, QMLReturnVariant& retVar) {
            switch (type) {
                case QMetaType::Type::Void:
                    retVar = nullptr;
                    return QGenericReturnArgument();
                case QMetaType::Bool:
                    retVar = bool();
                    return QReturnArgument<bool>("bool", std::get<bool>(retVar));
                case QMetaType::Int:
                    retVar = int();
                    return QReturnArgument<int>("int", std::get<int>(retVar));
                case QMetaType::Float:
                    retVar = float();
                    return QReturnArgument<float>("float", std::get<float>(retVar));
                case QMetaType::Double:
                    retVar = double();
                    return QReturnArgument<double>("double", std::get<double>(retVar));
                case QMetaType::QString:
                    retVar = QString();
                    return QReturnArgument<QString>("QString", std::get<QString>(retVar));
                case QMetaType::QDateTime:
                    retVar = QDateTime();
                    return QReturnArgument<QDateTime>("QDateTime", std::get<QDateTime>(retVar));
                default:
                    retVar = QVariant();
                    return QReturnArgument<QVariant>("QVariant", std::get<QVariant>(retVar));
            }
        }

        QVariant ValueToQVariant(const skitter::core::Value& val) {
            using skitter::core::Value;
            using skitter::core::Type;

            switch (val.type()) {
                case Type::Null:
                    return QVariant();
                case Type::Bool:
                    return QVariant(val.asBool());
                case Type::Int:
                    return QVariant(static_cast<qlonglong>(val.asInt()));
                case Type::Double:
                    return QVariant(val.asDouble());
                case Type::String:
                    return QVariant(QString::fromStdString(val.asString()));
                case Type::DateTime: {
                    auto tp = val.asDateTime();
                    std::time_t t = std::chrono::system_clock::to_time_t(tp);
                    return QVariant(QDateTime::fromSecsSinceEpoch(static_cast<qint64>(t)));
                }
                case Type::Array: {
                    QVariantList list;
                    for (const auto &item: val.asArray()) {
                        list.append(ValueToQVariant(item));
                    }
                    return QVariant(list);
                }
                case Type::Object: {
                    QVariantMap map;
                    for (const auto &kv: val.asObject()) {
                        map.insert(QString::fromStdString(kv.first), ValueToQVariant(kv.second));
                    }
                    return QVariant(map);
                }
                case Type::Binary: {
                    const auto &binaryData = val.asBinary();
                    QByteArray byteArray(reinterpret_cast<const char *>(binaryData.data()),
                                         static_cast<int>(binaryData.size()));
                    return QVariant(byteArray);
                }
                default:
                    throw std::runtime_error("Unsupported Value type for conversion to QVariant");
            }
        }

        skitter::core::Value QVariantToValue(const QVariant& var) {
            using Value = skitter::core::Value;
            using array_t = Value::array_t;
            using object_t = Value::object_t;

            int varType = var.userType();
            switch(static_cast<QMetaType::Type>(varType)) {
                case QMetaType::Type::Bool:
                    return Value(var.toBool());
                case QMetaType::Type::Char:
                case QMetaType::Type::SChar:
                case QMetaType::Type::Short:
                case QMetaType::Type::Int:
                case QMetaType::Type::Long:
                case QMetaType::Type::LongLong:
                    return Value(static_cast<int64_t>(var.toLongLong()));
                case QMetaType::Type::UChar:
                case QMetaType::Type::UShort:
                case QMetaType::Type::UInt:
                case QMetaType::Type::ULong:
                case QMetaType::Type::ULongLong:
                    return Value(static_cast<int64_t>(var.toULongLong()));
                case QMetaType::Type::Float:
                case QMetaType::Type::Double:
                    return Value(var.toDouble());
                case QMetaType::Type::QString:
                    return Value(var.toString().toStdString());
                case QMetaType::Type::QDateTime: {
                    qint64 secs = var.toDateTime().toSecsSinceEpoch();
                    return Value(std::chrono::system_clock::from_time_t(static_cast<std::time_t>(secs)));
                }
                case QMetaType::Type::Nullptr:
                case QMetaType::Type::Void:
                case QMetaType::Type::UnknownType:
                    return Value();
                default:
                    break;
            }

            if(varType == qMetaTypeId<QJSValue>()) {
                QJSValue jsVal = var.value<QJSValue>();
                return QVariantToValue(jsVal.toVariant());
            }

            // Prefer treating raw byte arrays as binary before checking for list/map conversions
            if(var.canConvert<QByteArray>()) {
                QByteArray byteArray = var.toByteArray();
                Value::binary_t bin;
                bin.reserve(static_cast<size_t>(byteArray.size()));
                for (auto c: byteArray) {
                    bin.push_back(static_cast<uint8_t>(c));
                }
                return Value(std::move(bin));
            }

            if(var.canConvert<QVariantList>()) {
                const QVariantList& list = var.toList();
                array_t out;
                out.reserve(static_cast<size_t>(list.size()));
                for(const auto& item : list) {
                    out.push_back(QVariantToValue(item));
                }
                return Value(std::move(out));
            }

            if(var.canConvert<QVariantMap>()) {
                const QVariantMap& map = var.toMap();
                object_t out;
                for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
                    out[it.key().toStdString()] = QVariantToValue(it.value());
                }
                return Value(std::move(out));
            }
            throw std::runtime_error(std::string("QVariantToValue: Received unknown type: ") + var.typeName());
        }

        skitter::core::Value QMLReturnVariantToValue(const QMLReturnVariant& var) {
            using Value = skitter::core::Value;

            return std::visit([](auto&& arg) -> Value {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, bool>) {
                    return Value(arg);
                } else if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
                    return Value(static_cast<int64_t>(arg));
                } else if constexpr (std::is_unsigned_v<T> && !std::is_same_v<T, bool>) {
                    // promote unsigned to signed storage (Value only holds int64_t)
                    return Value(static_cast<int64_t>(arg));
                } else if constexpr (std::is_floating_point_v<T>) {
                    return Value(static_cast<double>(arg));
                } else if constexpr (std::is_same_v<T, QString>) {
                    return Value(arg.toStdString());
                } else if constexpr (std::is_same_v<T, QDateTime>) {
                    qint64 secs = arg.toSecsSinceEpoch();
                    return Value(std::chrono::system_clock::from_time_t(static_cast<std::time_t>(secs)));
                } else if constexpr (std::is_same_v<T, QVariant>) {
                    return QVariantToValue(arg);
                } else {
                    return QVariantToValue(QVariant::fromValue(arg));
                }
            }, var);
        }

        bool CanConvertArgTypes(const QMetaMethod& metaMethod, const std::vector<QVariant>& varargs) {
            if(metaMethod.parameterCount() != varargs.size()) return false;

            for(size_t i = 0; i < metaMethod.parameterCount(); ++i) {
                int targetType = metaMethod.parameterType(i);
                if (targetType != QMetaType::Type::QVariant && !varargs[i].canConvert(QMetaType(targetType))) {
                    return false;
                }
            }
            return true;
        }

        bool GetMethodMetaForArgs(const QObject& obj, const std::string& method, const std::vector<QVariant>& varargs, QMetaMethod& ret) {
            const QMetaObject *itemMeta = obj.metaObject();
            for (size_t i = 0; i < itemMeta->methodCount(); ++i) {
                const QMetaMethod methodMeta = itemMeta->method(i);
                if (methodMeta.name().compare(method.data()) == 0 && CanConvertArgTypes(methodMeta, varargs)) {
                    ret = methodMeta;
                    return true;
                }
            }
            return false;
        }

        std::vector<QGenericArgument> ConvertAndCCreateQArgumentsForMethod(
                const QMetaMethod& method,
                std::vector<QVariant>& varargs,
                bool& ok)
        {
            std::vector<QGenericArgument> qargs;
            ok = false;

            const int paramCount = method.parameterCount();

            for (int i = 0; i < paramCount && i < static_cast<int>(varargs.size()); ++i) {
                    int targetType = method.parameterType(i);

                   if(targetType == QMetaType::QVariant) continue;

                   if(!varargs[i].canConvert(QMetaType(targetType))) {
                       qWarning() << "Invoke pre-check failed: "
                                    << "arg" << i
                                    << "| method:" << method.name()
                                    << "| from:" << varargs[i].typeName()
                                    << "| to:" << QMetaType(targetType).name();
                          return {};
                   }
            }

            for(int i = 0; i < 10; ++i) {
                if(i < static_cast<int>(varargs.size())){
                    int targetType = method.parameterType(i);

                    if(targetType != QMetaType::QVariant) {
                        if(!ConvertArg(varargs[i], targetType, i, method)) {
                            return {};
                        }
                        QMetaType meta(targetType);
                        qargs.emplace_back(QGenericArgument(meta.name(), varargs[i].data()));
                    }else{
                        qargs.emplace_back("QVariant", varargs[i].data());
                    }
                }else{
                    qargs.emplace_back();
                }
            }
            ok = true;
            return qargs;
        }


        bool ConvertArg(QVariant& v, int targetType, int index, const QMetaMethod& method) {
            QMetaType meta(targetType);

            if (v.convert(meta)) {
                return true;
            }

            qWarning() << "Invoke failed:"
                       << "arg" << index
                       << "| method:" << method.name()
                       << "| from:" << v.typeName()
                       << "| to:" << meta.name();

            return false;
        }
    } // namespace qt
} // namespace whisker