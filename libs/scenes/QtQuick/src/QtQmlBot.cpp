#include <wiskr/QtQmlBot.h>

#include <wiskr/qt_scene.h>
#include <wiskr/dispatch/dispatcher.h>
#include <wiskr/test_server.h>
#include <QtDebug>

namespace whisker {

    QtQmlBot::QtQmlBot(QObject* parent) : QObject(parent), scene_(std::make_unique<QtScene>()), dispatcher_(std::make_unique<Dispatcher>()) {
        startTimer(10); // start a timer to process events every 10ms
    }

    QtQmlBot::~QtQmlBot() = default;

    void QtQmlBot::runTestServer(TestServer& server) {
        qInfo() << "Wiskr server is enabled. Only use in a test environment!";
        server.setDispatcher(dispatcher_.get());
        server.start();
    }

    void QtQmlBot::timerEvent(QTimerEvent* /*event*/) {
        dispatcher_->processCommands(*scene_);
    }

} // namespace whisker