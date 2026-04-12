#ifndef WISKR_SCENE_H
#define WISKR_SCENE_H

#include <wiskr/wiskr_core_export.h>

#include <wiskr/data/geometry.h>
#include <wiskr/scene/events.h>
#include <wiskr/scene/item.h>
#include <wiskr/data/item_path.h>

#include <memory>
#include <string>


namespace whisker {
    class PasteboardContent;

    /**
     * The Scene class represents the current visual and interactive context of the application.
     */
    class WISKRCORE_EXPORT Scene {

    public:
        virtual ~Scene() = default;

        /**
         * Retrieves the item located at the specified path within the scene.
         * @param path The path to the item within the scene, which can be used to identify and access specific items in a hierarchical structure.
         * @return A unique pointer to the item at the specified path.
         */
        virtual std::unique_ptr<Item> itemAt(const ItemPath& path) const = 0;

        virtual Events& events() = 0;

        /**
         * Takes a screenshot of the specified item in the scene and saves it to a file with the given filename.
         * @param targetItem The path to the item in the scene that should be captured in the screenshot.
         * @param filename The name of the file where the screenshot will be saved. This should include the desired file extension (e.g., "screenshot.png").
         */
        virtual void takeScreenshot(const ItemPath& targetItem, const std::string& filename) = 0;

        /**
         * Takes a screenshot of the specified item in the scene and returns it as a base64-encoded string.
         * @param targetItem The path to the item in the scene that should be captured in the screenshot.
         * @return A base64-encoded string representing the screenshot of the specified item. This can be used for embedding the image in web pages or for other purposes where a file is not desired.
         */
        virtual std::string takeScreenshotBase64(const ItemPath& targetItem) = 0;
    };
} // namespace whisker

#endif //WISKR_SCENE_H
