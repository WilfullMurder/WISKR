#ifndef WISKR_QTQMLBOT_H
#define WISKR_QTQMLBOT_H

#include <wiskr/wiskr_qtquick_export.h>

#include <QObject>
#include <wiskr/test_server.h>
#include <memory>

namespace whisker {

    class Dispatcher;
    class QtScene;

/**
 * @class QtQmlBot
 * @brief Qt-side integration helper that runs a TestServer and drives a Scene.
 *
 * The QtQmlBot owns a Scene and a Dispatcher and provides a bridge for running
 * a `TestServer` implementation on a background thread while ensuring commands
 * are enqueued to the scene's `Dispatcher` on the Qt main thread.
 *
 * Instances are QObject-derived so they can be instantiated and managed from
 * Qt code (for example test harnesses that run within a Qt event loop).
 */
    class WISKRQTQUICK_EXPORT QtQmlBot : public QObject{
    Q_OBJECT

    public:
    /**
     * @brief Construct a QtQmlBot.
     * @param parent Optional QObject parent for Qt ownership semantics.
     *
     * The constructor prepares internal resources but does not start any test
     * server thread. Call `runTestServer()` to run a supplied `TestServer`.
     */
    explicit QtQmlBot(QObject* parent = nullptr);

    /**
     * @brief Destructor.
     *
     * Cleans up owned resources (scene, dispatcher). Any running test threads
     * created by a provided `TestServer` should be stopped by the server's
     * own shutdown logic prior to destruction.
     */
    ~QtQmlBot();

    /**
     * @brief Start running the given TestServer.
     * @param server Reference to a `TestServer` instance to execute.
     *
     * This method connects the provided `TestServer` to the internal
     * `Dispatcher` and instructs it to start execution. The `TestServer` will
     * run its test scenario on a background thread and will use the Dispatcher's
     * enqueue API to interact with the `QtScene` on the main thread.
     *
     * The `server` reference must remain valid for the lifetime of the test
     * execution or until the server has cleanly stopped.
     */
    void runTestServer(TestServer& server);

    protected:

    /**
     * @brief Qt timer event handler.
     * @param event Pointer to the QTimerEvent delivered by the Qt event loop.
     *
     * The implementation uses timer events for any periodic internal tasks
     * required by the QtQmlBot (for example processing pending Dispatcher
     * operations or integrating with the Qt event loop). Override of
     * QObject::timerEvent ensures correct event dispatching within Qt.
     */
    void timerEvent(QTimerEvent* event) override;

    private:
    std::unique_ptr<QtScene> scene_; ///< Owned QtScene instance that represents the scene being tested.
    std::unique_ptr<Dispatcher> dispatcher_; ///< Owned Dispatcher that routes commands to the scene thread.

};

} // namespace whisker
#endif //WISKR_QTQMLBOT_H
