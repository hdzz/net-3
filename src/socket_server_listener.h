#ifndef RJ_NET_SOCKET_SERVER_LISTENER_H
#define RJ_NET_SOCKET_SERVER_LISTENER_H


namespace rj
{
    namespace net
    {
        class socket_server;

        /*!
         * Defines an interface to listen to a socket server
         */
        class socket_server_listener
        {
           public:
            typedef socket_server *server_type;

            /*!
             * called when the server starts
             */
            virtual void on_start(const server_type &server) = 0;

            /*!
             * called when the server stops
             */
            virtual void on_stop(const server_type &server) = 0;
        };
    }
}

#endif
