
// Definition of the socket class

#ifndef RJ_SOCKET_H
#define RJ_SOCKET_H

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace rj
{
    namespace net
    {
#ifdef _WIN32
        typedef int socklen_t;
#else
        typedef int SOCKET;
        int closesocket(SOCKET socket);
#endif

        class secure_layer;

        /*!
         * A wrapper for a raw socket.  Includes both client and server functionality
         */
        class socket
        {
           public:
            static const int INVALID = -1;

            /*!
             * the base data type for sockets
             */
            typedef unsigned char data_type;

            /*!
             * data type for a data buffer
             */
            typedef std::vector<data_type> data_buffer;

            /*!
             * default constructor
             */
            socket();

            /*!
             * constructor to take a raw socket and its address
             */
            socket(SOCKET sock, const sockaddr_storage &addr);

            /*!
             * Constructor to open a socket given the host and port
             */
            socket(const std::string &host, const int port, bool secure = false);

            /*!
             * Non copyable
             */
            socket(const socket &) = delete;

            /*!
             * move constructor
             */
            socket(socket &&other);

            /*!
             * destructor will close the socket RAII style
             */
            virtual ~socket();

            /*!
             * Non-copyable assignment operator
             */
            socket &operator=(const socket &) = delete;

            /*!
             * Move assignment operator
             */
            socket &operator=(socket &&);

            bool operator==(const socket &other) const;

            // Data Transimission

            /*!
             * Will write a block of data to the socket
             * @return the number of bytes written
             */
            virtual int send(const data_buffer &, int flags = 0);

            /*!
             * Will write a block of data to the socket
             * @return the number of bytes written
             */
            virtual int send(const void *, size_t, int flags = 0);

            /*!
             * Recieves a block of input
             * @returns the number of bytes read
             */
            virtual int recv(data_buffer &, int flags = 0);

            /*!
             * @returns true if the socket is alive and connected
             */
            bool is_valid() const;

            /*!
             * Writes a data buffer to the socket
             */
            socket &operator<<(const data_buffer &);

            /*!
             * Reads data from the socket into a data buffer
             */
            socket &operator>>(data_buffer &);

            /*!
             * @returns the raw socket
             */
            SOCKET raw_socket() const;


            /*!
             * @retuns the ip address of the socket
             */
            const char *ip() const;

            /*!
             * @returns the port of the socket
             */
            int port() const;

            /*!
             * closes the socket
             */
            virtual void close();

            /*!
             * Client initialization, connects to a host and port
             */
            virtual bool connect(const std::string &host, const int port);

            /*!
             * Accepts a socket
             * @param addr the address structure to populate
             * @returns the connected socket
             */
            SOCKET accept(sockaddr_storage &addr) const;

            /*!
             * puts the socket in listen mode
             */
            virtual bool listen(const int port, const int backlogSize = BACKLOG_SIZE);

            /*!
             * sets the socket in blocking or non blocking mode
             */
            void set_non_blocking(const bool);

            bool is_non_blocking() const;

            bool is_secure() const;

            void set_secure(bool value);

           protected:
            static const int MAXHOSTNAME = 200;
            static const int MAXRECV = 500;
            static const int BACKLOG_SIZE = 10;

            virtual void on_recv(data_buffer &s);

            // the raw socket
            SOCKET sock_;

            // the socket address
            struct sockaddr_storage addr_;

           private:
            bool non_blocking_;
            std::shared_ptr<secure_layer> ssl_;
        };
    }
}

#endif
