#ifndef WISKR_QT_SCENE_H
#define WISKR_QT_SCENE_H

#include <wiskr/wiskr_qtquick_export.h>

#include <wiskr/qt_events.h>
#include <wiskr/data/item_path.h>
#include <wiskr/scene/scene.h>

#include <QByteArray>

class QQuickWindow;

namespace whisker {
    class ItemPath;

    class WISKRQTQUICK_EXPORT QtScene : public Scene {
    public:
        std::unique_ptr<Item> itemAt(const ItemPath& path) const override;

        Events& events() override;

        void takeScreenshot(const ItemPath& targetItem, const std::string& filename) override;
        std::string takeScreenshotBase64(const ItemPath& targetItem) override;

    private:
        QtEvents events_;
    };
}


#endif //WISKR_QT_SCENE_H
