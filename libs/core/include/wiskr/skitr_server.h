#ifndef WISKR_SKITR_SERVER_H
#define WISKR_SKITR_SERVER_H

#include <wiskr/wiskr_core_export.h>
#include <wiskr/test_server.h>
#include <skitr/registry/method_registry.h>
#include <skitr/runtime/dispatcher.h>
#include <skitr/transport/server.h>

namespace whisker {

    /**
     * @file `libs/core/include/wiskr/skitr_server.h`
     * @brief Test server exposing a skitr RPC transport and binding remote methods to test helpers.
     *
     * The SkitrServer adapts the generic TestServer to the skitr transport/registry.
     * It registers RPC methods that map to the various `TestServer` helper operations
     * (mouse events, property access, screenshots, etc.) and runs a transport `Server`
     * listening on a TCP port. The server runs alongside the application and enqueues
     * commands to the `Dispatcher` so that operations execute on the scene thread.
     */
    class WISKRCORE_EXPORT SkitrServer: public TestServer {
    public:

        /**
         * @brief Construct a SkitrServer.
         *
         * Creates a server bound to the given TCP port (default 9000). The constructor
         * typically registers remote callable methods with the internal `registry_` and
         * prepares the transport `server_`.
         *
         * @param port TCP port on which the skitr transport server will listen.
         */
        SkitrServer(uint16_t port = 9000);

        /**
         * @brief Virtual destructor.
         *
         * Cleans up owned transport resources. If a transport server is running it will
         * be stopped and any background resources released.
         */
        ~SkitrServer() override;

    protected:

        /**
         * @brief Execute the test scenario on the TestServer worker thread.
         *
         * This override is invoked by `TestServer::start()` on a background thread.
         * For `SkitrServer` this may drive any server-side logic required while the
         * transport server is active. Implementations must not touch UI/scene state
         * directly but should use the provided TestServer helper methods which enqueue
         * commands via the Dispatcher.
         */
        void executeTest() override;

    private:
        skitter::registry::MethodRegistry registry_; ///< Method registry for RPC handlers.
        skitter::runtime::Dispatcher dispatcher_; ///< Dispatcher that routes RPC calls to the registry.
        std::unique_ptr<skitter::transport::Server> server_; ///< Transport server that listens for RPC calls.
    };
}

#endif //WISKR_SKITR_SERVER_H
