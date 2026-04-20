#include <wiskr/qt_scene.h>
#include <wiskr/find_qt_item.h>
#include <wiskr/qt_item.h>
#include <wiskr/qt_item_tools.h>
#include <wiskr/data/item_path.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

namespace whisker {


    std::unique_ptr<Item> QtScene::itemAt(const ItemPath &path) const {
        auto window = whisker::qt::getQQuickWindowAt(path);

        if(!window || !window->contentItem()) return {};
        if(path.length() == 1) return std::make_unique<QtItem>(window);

        auto item = whisker::qt::getQQuickItemAt(path);
        if(!item) return {};
        return std::make_unique<QtItem>(item);
    }

    Events &QtScene::events() {
        return events_;
    }

    void QtScene::takeScreenshot(const ItemPath &targetItem, const std::string &filename) {
        auto item = whisker::qt::getQQuickItemAt(targetItem);
        if(!item) return;

        auto windowImage = item->window()->grabWindow();

        QRectF imageCropRectItemSpace (0, 0, item->width(), item->height());
        auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
        QRect imageCropRect(
                imageCropRectF.x() * windowImage.devicePixelRatio(),
                imageCropRectF.y() * windowImage.devicePixelRatio(),
                imageCropRectF.width() * windowImage.devicePixelRatio(),
                imageCropRectF.height() * windowImage.devicePixelRatio()
        );

        auto image = windowImage.copy(imageCropRect);
        image.save(QString::fromStdString(filename));
    }

    std::string QtScene::takeScreenshotBase64(const ItemPath &targetItem) {
        auto item = whisker::qt::getQQuickItemAt(targetItem);
        if(!item) return "";

        auto windowImage = item->window()->grabWindow();
        QRectF imageCropRectItemSpace (0, 0, item->width(), item->height());
        auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
        QRect imageCropRect(
                imageCropRectF.x() * windowImage.devicePixelRatio(),
                imageCropRectF.y() * windowImage.devicePixelRatio(),
                imageCropRectF.width() * windowImage.devicePixelRatio(),
                imageCropRectF.height() * windowImage.devicePixelRatio()
        );
        auto image = windowImage.copy(imageCropRect);
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        buffer.close();

        return byteArray.toBase64().toStdString();
    }
} // namespace whisker
