#ifndef ARG3_NET_SOCKETSERVER_H
#define ARG3_NET_SOCKETSERVER_H

#include "socket_factory.h"
#include <vector>
#include <thread>
#include <mutex>
#include <sys/time.h>

using namespace std;

namespace arg3
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
            /*!
             * called when the server has polled its connections
             */
            virtual void on_poll(socket_server *server) = 0;

            /*!
             * called when the server starts
             */
            virtual void on_start(socket_server *server) = 0;

            /*!
             * called when the server stops
             */
            virtual void on_stop(socket_server *server) = 0;
        };


        /*!
         * Defines a network server that accepts incomming connections, processes their input and writes their output.
         */
        class socket_server : public socket
        {
           public:
            /*!
             * default constructor
             * @factory the factory to create sockets with
             */
            socket_server(socket_factory *factory = &default_socket_factory);

            /*!
             * non-copyable constructor
             */
            socket_server(const socket_server &other) = delete;

            /*!
             * move constructor
             */
            socket_server(socket_server &&other);

            /*!
             * destuctor will stop the server
             */
            virtual ~socket_server();

            /*!
             * Non-copyable assignment operator
             */
            socket_server &operator=(const socket_server &other) = delete;

            /*!
             * move assignment operator
             */
            socket_server &operator=(socket_server &&other);

            bool is_valid() const;

            /*!
             * starts the server
             */
            void start(int port, int backlogSize = BACKLOG_SIZE);

            void start_in_background(int port, int backlogSize = BACKLOG_SIZE);

            /*!
             * Sets the frequency of connection updates (used when looping)
             * @param value the number of cycles per secon
             */
            void set_poll_frequency(unsigned cyclesPerSecond);

            /*!
             * Adds a l istener to the server
             */
            void add_listener(socket_server_listener *listener);

            /*!
             * equality operator compares the port
             */
            bool operator==(const socket_server &other);

            /*!
             * inequality operator compares the port
             */
            bool operator!=(const socket_server &other);

            /*!
             * updates the servers connections (performs read/writes)
             * - will do nothing if the server is not alive
             * - is called by the server based on the poll frequency
             */
            void poll();

            void stop();

            bool listen(const int port, const int backlogSize);

            void set_socket_factory(socket_factory *factory);

           protected:
            /*!
             * starts a syncronous loop of updating connections
             */
            void run();

            /*!
             * can override these without adding a listener
             */
            virtual void on_poll();
            virtual void on_start();
            virtual void on_stop();

           private:
            /*!
             * Will loop each connection and if the delegate returns false, will remove the connection
             */
            void check_connections(std::function<bool(const std::shared_ptr<buffered_socket> &)> delegate);

            void wait_for_poll(struct timeval *);

            void notify_poll();

            void notify_start();

            void notify_stop();

            unsigned pollFrequency_;

            socket_factory *factory_;

            shared_ptr<thread> backgroundThread_;

            vector<std::shared_ptr<buffered_socket>> sockets_;

            vector<socket_server_listener *> listeners_;

            mutex sockets_mutex_;
            mutex listeners_mutex_;
        };
    }
}

#endif
