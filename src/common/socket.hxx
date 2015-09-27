#ifndef SVH_COMMON_SOCKET_SERVER_H__
#define SVH_COMMON_SOCKET_SERVER_H__
#pragma once

#include <string>
#include <memory>

#include "codedocs.h"

#include "daemon.hxx"
#include "constraint.hxx"

struct uv_tcp_s;
typedef uv_tcp_s uv_tcp_t;

namespace sevenhearts {
	namespace net {

		class Message {
		public:
			Message(size_t length);
			Message(consumes char *buffer, size_t length);
			Message(const Message &message) = delete;
			~Message();

			owns char * const buffer;
			const size_t length;
		};

		class SocketClient {
			friend class SocketServerBase;
		public:
			SocketClient();
			virtual ~SocketClient();

			// does not assume ownership
			void write(uses char *buffer, size_t len);
			void write(std::shared_ptr<Message> message);
			virtual void onData(std::shared_ptr<Message> message) = 0;
			virtual void onDisconnect(int status) = 0;

			void start();
			void stop();

			void close();
		private:
			owns uv_tcp_t *handle;
		};

		class SocketServerBase : public Daemon {
		protected:
			SocketServerBase(std::string bindHost, unsigned short bindPort);
			void onError(int status);

			uses uv_tcp_t* getUVHandle() const;

		private:
			std::string host;
			unsigned short port;

			owns uv_tcp_t *handle;
		};

		template <typename Client>
		class SocketServer : public SocketServerBase {
		public:
			SocketServer(std::string bindHost, unsigned short bindPort)
					: SocketServerBase(bindHost, bindPort) {
				derived_from<Client, SocketClient>();
			}

		protected:
			virtual void onConnect(std::shared_ptr<Client> client) = 0;
		};
	}
}

#endif
