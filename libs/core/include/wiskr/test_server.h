#ifndef WISKR_TEST_SERVER_H
#define WISKR_TEST_SERVER_H

#include <wiskr/wiskr_core_export.h>

#include <wiskr/data/geometry.h>
#include <wiskr/data/item_path.h>
#include <wiskr/events/identifiers.h>
#include <skitr/core/value.h>

#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <string>

namespace whisker {
    class Dispatcher;

    /**
     * @brief High-level test helper that runs a test scenario on a background thread.
     *
     * TestServer provides a set of convenience methods that post commands to a
     * Dispatcher to interact with the Scene on the main thread. Derived classes
     * must implement executeTest() which is invoked on a worker thread created
     * by start(). Many methods use promises internally so the caller can block
     * until the corresponding scene-thread action completes.
     *
     * @note The Dispatcher pointer must be set via setDispatcher() before calling start()
     *       or invoking operations that enqueue commands.
     */
    class WISKRCORE_EXPORT TestServer {
    public:

        /**
         * @brief Virtual destructor ensures derived cleanup on shutdown.
         *
         * If the internal worker thread is running it will be joined during destruction.
         */
        virtual ~TestServer();

        /**
         * @brief Start the test execution thread.
         *
         * This spawns a worker thread that calls the protected executeTest()
         * implementation. Exceptions thrown from executeTest() are caught and
         * reported to stderr.
         *
         * @throws std::runtime_error if a Dispatcher has not been set.
         */
        void start();

        /**
         * @brief Provide the Dispatcher used to enqueue scene commands.
         *
         * @param dispatcher Pointer to the Dispatcher instance that will receive commands.
         */
        void setDispatcher(Dispatcher* dispatcher);

        /**
         * @brief Install a handler for generic custom commands.
         *
         * The handler will be invoked when genericCommand(...) is used to post a
         * CustomCommand to the Dispatcher.
         *
         * @param handler Callable that receives (command, payload).
         */
        void setGenericCommandHandler(std::function<void(std::string, std::string)> handler);

        /**
         * @brief Enqueue a timed wait on the Dispatcher.
         *
         * Posts a cmd::Wait to the Dispatcher which delays progress of the command
         * queue for the given duration.
         *
         * @param duration Duration to wait (in milliseconds).
         */
        void wait(std::chrono::milliseconds duration);

        /**
         * @brief Simulate a mouse click on the item at the given path (center).
         *
         * Enqueues a cmd::ClickOnItem using the left mouse button and no modifiers.
         *
         * @param path ItemPath identifying the target item.
         */
        void mouseClick(const ItemPath& path);

        /**
         * @brief Simulate a mouse click at a relative position within the item.
         *
         * @param path ItemPath identifying the target item.
         * @param position Position expressed as a proportion and/or offset for the item.
         */
        void mouseClick(const ItemPath& path, const Point& position);

        /**
         * @brief Simulate a mouse click at a relative position plus an extra offset.
         *
         * The provided offset is added to the base position before enqueueing the click.
         *
         * @param path ItemPath identifying the target item.
         * @param position Base position for the click.
         * @param offset Additional offset to apply to the click position.
         */
        void mouseClick(const ItemPath& path, const Point& position, const Point& offset);

        /**
         * @brief Simulate a mouse click specifying button and modifiers.
         *
         * @param path ItemPath identifying the target item.
         * @param button Mouse button to use.
         * @param mod Key modifier flags (default: none).
         */
        void mouseClick(const ItemPath& path, MouseButton button, KeyModifier mod = whisker::KeyModifiers::None);


        /**
         * @brief Begin a drag gesture on the item at the given path.
         *
         * Enqueues a cmd::DragBegin which issues mouseDown/mouseMove events on the scene thread.
         *
         * @param path ItemPath identifying the target item.
         */
        void mouseBeginDrag(const ItemPath& path);

        /**
         * @brief End a drag gesture on the item at the given path.
         *
         * Enqueues a cmd::DragEnd which issues mouseMove/mouseUp on the scene thread.
         *
         * @param path ItemPath identifying the target item.
         */
        void mouseEndDrag(const ItemPath& path);

        /**
         * @brief Simulate dropping external URLs onto the target item.
         *
         * Constructs a PasteboardContent containing the provided URLs and enqueues
         * a cmd::DropFromExt command.
         *
         * @param path ItemPath identifying the target item.
         * @param urls Vector of URL strings to drop.
         */
        void mouseDropUrls(const ItemPath& path, const std::vector<std::string>& urls);

        /**
         * @brief Post a custom generic command that invokes the installed handler.
         *
         * This enqueues a cmd::CustomCommand which will call the handler provided
         * via setGenericCommandHandler().
         *
         * @param command Arbitrary command identifier.
         * @param payload Optional payload string.
         */
        void genericCommand(const std::string& command, const std::string& payload);

        /**
         * @brief Send text input to the specified item.
         *
         * Enqueues a cmd::InputText that inputs the given string into the item.
         *
         * @param path ItemPath identifying the target item.
         * @param text Text to input.
         */
        void inputText(const ItemPath& path, const std::string& text);

        /**
         * @brief Simulate pressing and releasing a key on the specified item.
         *
         * Enqueues a cmd::EnterKey that sends key press/release events.
         *
         * @param path ItemPath identifying the target item.
         * @param keycode Platform-specific key code.
         * @param modifiers Bitmask of modifier keys.
         */
        void enterKey(const ItemPath& path, int keycode, unsigned modifiers);

        /**
         * @brief Retrieve a string property from an item synchronously.
         *
         * Posts a cmd::GetProperty with a moved promise and blocks until the result
         * is available, returning the property's string value (or empty string on error).
         *
         * @param path ItemPath identifying the target item.
         * @param property Name of the property to query.
         * @return std::string Property value or empty string if not found.
         */
        std::string getStringProperty(const ItemPath& path, const std::string& property);

        /**
         * @brief Set a string property on an item.
         *
         * Enqueues a cmd::SetProperty to set the property's value on the scene thread.
         *
         * @param path ItemPath identifying the target item.
         * @param property Name of the property to set.
         * @param value New string value for the property.
         */
        void setStringProperty(const ItemPath& path, const std::string& property, const std::string& value);

        /**
         * @brief Invoke a method on an item and return the result synchronously.
         *
         * Posts a cmd::InvokeMethod and waits on an internal promise for the returned Value.
         *
         * @param path ItemPath identifying the target item.
         * @param method Method name to invoke.
         * @param args Vector of skitter::core::Value arguments.
         * @return skitter::core::Value Return value of the invoked method.
         */
        skitter::core::Value invokeMethod(const ItemPath& path, const std::string& method, const std::vector<skitter::core::Value>& args);

        /**
         * @brief Retrieve the bounding box of an item synchronously.
         *
         * Posts a cmd::GetBoundingBox with a promise and returns the resulting Rect.
         *
         * @param path ItemPath identifying the target item.
         * @return Rect Bounding rectangle of the item (or zero rect on error).
         */
        Rect getBoundingBox(const ItemPath& path);

        /**
         * @brief Check whether an item exists and is visible synchronously.
         *
         * Posts a cmd::ExistsAndVisible and returns the boolean result.
         *
         * @param path ItemPath identifying the target item.
         * @return bool True if item exists and is visible, false otherwise.
         */
        bool existsAndVisible(const ItemPath& path);

        /**
         * @brief Retrieve error messages recorded in the Dispatcher State.
         *
         * Posts a cmd::GetTestStatus that returns a vector of error strings.
         *
         * @return std::vector<std::string> Collected error messages.
         */
        std::vector<std::string> getErrors();

        /**
         * @brief Wait for an item to appear within a timeout synchronously.
         *
         * Posts a cmd::WaitForItem with the requested timeout and blocks until the
         * promise is satisfied or the timeout elapses.
         *
         * @param path ItemPath to wait for.
         * @param timeout Maximum duration to wait.
         * @return bool True if the item appeared before timeout, false otherwise.
         */
        bool waitForItem(const ItemPath& path, std::chrono::milliseconds timeout);

        /**
         * @brief Request the scene to write a screenshot image to disk.
         *
         * Enqueues a cmd::Screenshot which instructs the Scene to persist an image file.
         *
         * @param path ItemPath identifying the target item to capture.
         * @param filename Destination filename for the screenshot.
         */
        void takeScreenshot(const ItemPath& path, const std::string& filename);

        /**
         * @brief Request a Base64-encoded screenshot string synchronously.
         *
         * Posts a cmd::ScreenshotBase64 and blocks until a Base64 image string is returned.
         *
         * @param path ItemPath identifying the target item to capture.
         * @return std::string Base64-encoded image data (empty on error).
         */
        std::string takeScreenshotBase64(const ItemPath& path);

        /**
         * @brief Post a quit event to the Scene.
         *
         * Enqueues a cmd::Quit to request the application to quit via the scene events.
         */
        void quit();


    protected:

        /**
         * @brief Execute the test scenario.
         *
         * Derived classes implement this method; it will run on a worker thread
         * started by start(). Use the TestServer helper methods to interact with
         * the Scene via the Dispatcher.
         */
        virtual void executeTest() = 0;

    private:
        Dispatcher* dispatcher_; /**< Dispatcher used to enqueue commands; must be set before use. */
        std::thread thread_; /**< Worker thread used to execute the test scenario. */
        std::function<void(std::string, std::string)> handler_; /**< Handler for generic custom commands. */
    };

} // namespace whisker

#endif //WISKR_TEST_SERVER_H
