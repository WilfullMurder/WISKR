#include <wiskr/test_server.h>

#include <wiskr/dispatch/dispatcher.h>

#include <wiskr/commands/click_on_item.h>
#include <wiskr/commands/custom_command.h>
#include <wiskr/commands/drag_begin.h>
#include <wiskr/commands/drag_end.h>
#include <wiskr/commands/drop_from_ext.h>
#include <wiskr/commands/enter_key.h>
#include <wiskr/commands/exists_and_visible.h>
#include <wiskr/commands/get_bounding_box.h>
#include <wiskr/commands/get_property.h>
#include <wiskr/commands/get_test_status.h>
#include <wiskr/commands/input_text.h>
#include <wiskr/commands/invoke_method.h>
#include <wiskr/commands/quit.h>
#include <wiskr/commands/screenshot.h>
#include <wiskr/commands/screenshot_base64.h>
#include <wiskr/commands/set_property.h>
#include <wiskr/commands/wait.h>
#include <wiskr/commands/wait_for_item.h>

#include <wiskr/events/identifiers.h>
#include <iostream>

namespace whisker {

    TestServer::~TestServer() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    void TestServer::start(){
            if(!dispatcher_) {
                throw std::runtime_error("Cannot start TestServer: Dispatcher not set");
            }
            thread_ = std::thread([this] {
                try {
                    executeTest();
                } catch (const std::exception& ex) {
                    std::cerr << "Test execution error: " << ex.what() << std::endl;
                }
            });
    }

    void TestServer::setDispatcher(Dispatcher* dispatcher) {
        dispatcher_ = dispatcher;
    }

    void TestServer::setGenericCommandHandler(std::function<void(std::string, std::string)> handler) {
        handler_ = std::move(handler);
    }

    void TestServer::wait(std::chrono::milliseconds duration) {
        dispatcher_->enqueueCommand<cmd::Wait>(duration);
    }

    void TestServer::mouseClick(const ItemPath& path) {
        dispatcher_->enqueueCommand<cmd::ClickOnItem>(path, whisker::MouseButtons::Left, whisker::KeyModifiers::None);
    }

    void TestServer::mouseClick(const ItemPath& path, const Point& position) {
        auto pathWithPos = ItemPosition(path, Point(0, 0), position);
        dispatcher_->enqueueCommand<cmd::ClickOnItem>(pathWithPos, whisker::MouseButtons::Left, whisker::KeyModifiers::None);
    }

    void TestServer::mouseClick(const ItemPath& path, const Point& position, const Point& offset) {
        Point combined(position.x + offset.x, position.y + offset.y);
        auto pathWithOffset = ItemPosition(path, Point(0, 0), combined);
        dispatcher_->enqueueCommand<cmd::ClickOnItem>(pathWithOffset, whisker::MouseButtons::Left, whisker::KeyModifiers::None);
    }

    void TestServer::mouseClick(const ItemPath& path, MouseButton button, KeyModifier mod){
        dispatcher_->enqueueCommand<cmd::ClickOnItem>(path, button, mod);
    }

    void TestServer::mouseBeginDrag(const ItemPath& path) {
        dispatcher_->enqueueCommand<cmd::DragBegin>(path);
    }

    void TestServer::mouseEndDrag(const ItemPath& path) {
        dispatcher_->enqueueCommand<cmd::DragEnd>(path);
    }

    void TestServer::mouseDropUrls(const ItemPath& path, const std::vector<std::string>& urls) {
        dispatcher_->enqueueCommand<cmd::DropFromExt>(path, makePasteboardContentWithUrls(urls));
    }

    void TestServer::genericCommand(const std::string& command, const std::string& payload) {
        dispatcher_->enqueueCommand<cmd::CustomCommand>([=](whisker::Environment&) {
            handler_(command, payload);
        }, []() {return true;});
    }

    void TestServer::inputText(const ItemPath& path, const std::string& text) {
        dispatcher_->enqueueCommand<cmd::InputText>(path, text);
    }

    void TestServer::enterKey(const ItemPath& path, int keycode, unsigned modifiers) {
        dispatcher_->enqueueCommand<cmd::EnterKey>(path, keycode, modifiers);
    }

    std::string TestServer::getStringProperty(const ItemPath& path, const std::string& property) {
        std::promise<std::string> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::GetProperty>(path, property, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    void TestServer::setStringProperty(const ItemPath& path, const std::string& property, const std::string& value) {
        dispatcher_->enqueueCommand<cmd::SetProperty>(path, property, value);
    }

    skitter::core::Value TestServer::invokeMethod(const ItemPath& path, const std::string& method, const std::vector<skitter::core::Value>& args) {
        std::promise<skitter::core::Value> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::InvokeMethod>(path, method, args, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    Rect TestServer::getBoundingBox(const ItemPath& path) {
        std::promise<Rect> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::GetBoundingBox>(path, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    bool TestServer::existsAndVisible(const ItemPath& path) {
        std::promise<bool> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::ExistsAndVisible>(path, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    std::vector<std::string> TestServer::getErrors() {
        std::promise<std::vector<std::string>> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::GetTestStatus>(true, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    bool TestServer::waitForItem(const ItemPath& path, std::chrono::milliseconds timeout) {
        std::promise<bool> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::WaitForItem>(path, timeout, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    void TestServer::takeScreenshot(const ItemPath& path, const std::string& filename) {
        dispatcher_->enqueueCommand<cmd::Screenshot>(path, filename);
    }

    std::string TestServer::takeScreenshotBase64(const ItemPath& path) {
        std::promise<std::string> promise;
        auto result = promise.get_future();
        auto cmd = std::make_unique<cmd::ScreenshotBase64>(path, std::move(promise));
        dispatcher_->enqueueCommand(std::move(cmd));
        return result.get();
    }

    void TestServer::quit() {
        dispatcher_->enqueueCommand<cmd::Quit>();
    }
} // namespace whisker