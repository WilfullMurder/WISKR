#ifndef WISKR_DISPATCHER_H
#define WISKR_DISPATCHER_H

#include <wiskr/dispatch/state.h>
#include <wiskr/commands/command.h>

#include <wiskr/wiskr_core_export.h>

#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace whisker {
    class Scene;

    /**
     * The Dispatcher is responsible for managing and executing commands that modify the state of the application.
     */
    class WISKRCORE_EXPORT Dispatcher{
            public:
            Dispatcher();

            State& state();

            /**
             * Enqueues a command to be executed. This method is thread-safe and can be called from any thread. The command will be executed on the main thread when processCommands is called.
             * @param command The command to enqueue. The Dispatcher takes ownership of the command and will manage its lifetime.
             */
            void enqueueCommand(std::unique_ptr<cmd::Command> command);

            /**
             * Processes all enqueued commands in FIFO order. This method should be called on the main thread, typically once per frame. It will execute commands until it encounters a command whose canExecute returns true, at which point it will stop processing further commands until the next call.
             * @param scene The scene to pass to the command environment when executing commands.
             */
            void processCommands(Scene& scene);

            /**
             * Convenience template method to enqueue a command by constructing it in-place with the provided arguments.
             * @tparam CommandType The type of the command to enqueue. Must be a subclass of cmd::Command.
             * @tparam Args The types of the arguments to forward to the command's constructor.
             * @param args The arguments to forward to the command's constructor. Perfectly forwarded to allow for move semantics.
             */
            template<typename CommandType, typename... Args>
            void enqueueCommand(Args&&... args) {
                enqueueCommand(std::make_unique<CommandType>(std::forward<Args>(args)...));
            }

            private:
            std::thread::id mainThreadId_; ///< The ID of the main thread, used to ensure that certain operations are only performed on the main thread.
            std::mutex mutex_; ///< Mutex to protect access to the command queue, allowing for thread-safe enqueuing of commands from multiple threads.

            std::queue<std::unique_ptr<cmd::Command>> commands_; ///< Queue of commands waiting to be executed. Commands are processed in FIFO order.
            State state_; ///< The current state of the application, which can be modified by commands and queried for errors.
    };
} // namespace whisker

#endif //WISKR_DISPATCHER_H
