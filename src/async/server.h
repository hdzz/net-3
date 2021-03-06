#ifndef RJ_NET_SERVER_ASYNC_H
#define RJ_NET_SERVER_ASYNC_H

#include "../socket_server.h"

namespace rj
{
    namespace net
    {
        namespace async
        {
            class server : public rj::net::socket_server
            {
               public:
                /*!
                 * default constructor
                 * @factory the factory to create sockets with
                 */
                server(const factory_type &factory = default_socket_factory);

                /*!
                 * non-copyable constructor
                 */
                server(const server &other) = delete;

                /*!
                 * move constructor
                 */
                server(server &&other);

                virtual ~server();

                /*!
                 * Non-copyable assignment operator
                 */
                server &operator=(const server &other) = delete;

                /*!
                 * move assignment operator
                 */
                server &operator=(server &&other);

               private:
                void set_non_blocking(bool);
                void on_start();

                void run();
            };
        }
    }
}


#endif
