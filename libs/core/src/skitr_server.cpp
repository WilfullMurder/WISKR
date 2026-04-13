#include <wiskr/skitr_server.h>
#include <skitr/core/value.h>

#include <atomic>



namespace whisker{
        using Value = skitter::core::Value;

        SkitrServer::SkitrServer(uint16_t port): registry_(), dispatcher_(registry_) {

            registry_.registerMethod("mouseClick", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                mouseClick(path);
                return {};
            });

            registry_.registerMethod("mouseClickWithPosition", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                Point position{static_cast<Real>(params.at(1).asInt()), static_cast<Real>(params.at(2).asInt())};
                mouseClick(path, position);
                return {};
            });

            registry_.registerMethod("mouseClickWithOffset", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                Point position{static_cast<Real>(params.at(1).asInt()), static_cast<Real>(params.at(2).asInt())};
                Point offset{static_cast<Real>(params.at(3).asInt()), static_cast<Real>(params.at(4).asInt())};
                mouseClick(path, position, offset);
                return {};
            });

            registry_.registerMethod("mouseClickWithButton", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                MouseButton button = static_cast<MouseButton>(params.at(1).asInt());
                mouseClick(path, button, KeyModifiers::None);
                return {};
            });

            registry_.registerMethod("mouseBeginDrag", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                mouseBeginDrag(path);
                return {};
            });

            registry_.registerMethod("mouseEndDrag", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                mouseEndDrag(path);
                return {};
            });

            registry_.registerMethod("mouseDropUrls", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::vector<std::string> urls;
                for(size_t i = 1; i < params.size(); ++i) {
                    urls.push_back(params.at(i).asString());
                }
                mouseDropUrls(path, urls);
                return {};
            });

            registry_.registerMethod("inputText", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::string text = params.at(1).asString();
                inputText(path, text);
                return {};
            });

            registry_.registerMethod("enterKey", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                int keycode = params.at(1).asInt();
                unsigned modifiers = params.at(2).asInt();
                enterKey(path, keycode, modifiers);
                return {};
            });

            registry_.registerMethod("getStringProperty", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::string property = params.at(1).asString();
                return getStringProperty(path, property);
            });

            registry_.registerMethod("setStringProperty", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::string property = params.at(1).asString();
                std::string value = params.at(2).asString();
                setStringProperty(path, property, value);
                return {};
            });

            registry_.registerMethod("invokeMethod", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::string method = params.at(1).asString();
                std::vector<Value> args(params.begin() + 2, params.end());
                return invokeMethod(path, method, args);
            });

            registry_.registerMethod("getBoundingBox", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                auto bounds = getBoundingBox(path);
                std::vector<Value> vals;
                vals.emplace_back(static_cast<long long>(bounds.topLeft.x));
                vals.emplace_back(static_cast<long long>(bounds.topLeft.y));
                vals.emplace_back(static_cast<long long>(bounds.size.width));
                vals.emplace_back(static_cast<long long>(bounds.size.height));
                return Value(vals);
            });

            registry_.registerMethod("getErrors", [this](const std::vector<Value>&) -> Value {
                auto errors = getErrors();
                std::vector<Value> result;
                for(const auto& err : errors) {
                    result.emplace_back(err);
                }
                return Value(result);
            });

            registry_.registerMethod("existsAndVisible", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                return existsAndVisible(path);
            });

            registry_.registerMethod("takeScreenshot", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                std::string filename = params.at(1).asString();
                takeScreenshot(path, filename);
                return {};
            });

            registry_.registerMethod("takeScreenshotBase64", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                return takeScreenshotBase64(path);
            });

            registry_.registerMethod("quit", [this](const std::vector<Value>&) -> Value {
                quit();
                return {};
            });

            registry_.registerMethod("waitForItem", [this](const std::vector<Value>& params) -> Value {
                ItemPath path(params.at(0).asString());
                int ms = params.at(1).asInt();
                return waitForItem(path, std::chrono::milliseconds(ms));
            });

            registry_.registerMethod("wait", [this](const std::vector<Value>& params) -> Value {
                int ms = params.at(0).asInt();
                wait(std::chrono::milliseconds(ms));
                return {};
            });

            registry_.registerMethod("genericCommand", [this](const std::vector<Value>& params) -> Value {
                std::string command = params.at(0).asString();
                std::string payload = params.at(1).asString();
                genericCommand(command, payload);
                return {};
            });


            server_ = std::make_unique<skitter::transport::Server>(port, dispatcher_);
            server_->start();
        }

    SkitrServer::~SkitrServer(){
            thread_local static std::atomic<bool> isShuttingDown{false};
            if(isShuttingDown) return; // prevent reentrancy if destructor is called during stack unwinding
            isShuttingDown = true;
            if(server_) {
                server_->stop();
            }

        };

    void SkitrServer::executeTest() {
            // No test scenario implemented here; the server runs indefinitely until shutdown.
            // Derived classes can override this method to implement specific test sequences using the provided helper methods.
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
    }
} // namespace whisker