#include <wiskr/dispatch/dispatcher.h>
#include <wiskr/dispatch/environment.h>


#include <cassert>

namespace whisker {

    Dispatcher::Dispatcher() : mainThreadId_(std::this_thread::get_id()), commands_() {}

    State& Dispatcher::state() {
        assert(mainThreadId_ == std::this_thread::get_id());
        return state_;
    }

    void Dispatcher::enqueueCommand(std::unique_ptr<cmd::Command> command) {
        std::lock_guard<std::mutex> lock(mutex_);
        commands_.push(std::move(command));
    }

    void Dispatcher::processCommands(Scene& scene) {
        assert(mainThreadId_ == std::this_thread::get_id());

        std::unique_lock<std::mutex> lock(mutex_, std::try_to_lock);
        if (!lock) return;

        Environment env(scene, state_);

        while (!commands_.empty()) {
            auto& queuedCmd = commands_.front();

            if (queuedCmd->canExecute(env)) break;

            std::unique_ptr<cmd::Command> localCmd = std::move(queuedCmd);
            commands_.pop();

            lock.unlock();

            localCmd->execute(env);
            if(!lock.try_lock()) return;
        }
    }
} // namespace whisker