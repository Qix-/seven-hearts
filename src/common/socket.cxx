#include <stdlib.h>

#include "uv.h"

#include "socket.hxx"
#include "log.hxx"
#include "uv-error.hxx"

using namespace std;
using namespace sevenhearts;

static const size_t BUFFER_SIZE = 8192;

struct CloseStatus {
	CloseStatus(uses net::SocketClient *socket, int status)
			: socket(socket),
			  status(status) {
	}

	uses net::SocketClient *socket;
	int status;
};

net::Message::Message(size_t length)
		: buffer((char *) malloc(length)),
		  length(length) {
	/*
		why malloc() and not new[]?

		because this class is intended to be used with lower level write
		operations. while this particular constructor overload doesn't
		matter (memory is memory, regardless of malloc() or new[]),
		the ~destructor() is what we care about. the other overload
		takes in an assumed malloc()'d buffer from a low level library;
		those libraries are usually written in C, hence we will have to
		ultimately free() it.
	*/
}

net::Message::Message(consumes char *buffer, size_t length)
		: buffer(buffer),
		  length(length) {
}

net::Message::~Message() {
	/*
		see comment in Message(size_t) about why free() is used here.
	*/
	if (this->buffer) {
		free(consumes this->buffer);
		*(const_cast<char**>(&this->buffer)) = 0;
	}
}

net::SocketClient::SocketClient()
		: handle(new uv_tcp_t()) {
	/*
		initialization of the handle is actually done in the main
		server object that created this.
	*/
}

net::SocketClient::~SocketClient() {
	uv_close((uv_handle_t *) this->handle, [](uv_handle_t *handle) {
		delete handle;
	});
}

void net::SocketClient::write(uses char *buffer, size_t len) {
	uv_write_t request;
	const uv_buf_t buffers[] = {
		{ buffer, len }
	};
	int result = uv_write(&request, uses (uv_stream_t *) this->handle, buffers, 1, [](uses uv_write_t *, int status) {
		if (status < 0) {
			out::warn() << "write failed on socket (" << uv_get_error_name(status) << ")";
		}
	});;
	if (result < 0) {
		out::warn() << "write call failed on socket (" << uv_get_error_name(result) << ")";
	}
}

void net::SocketClient::write(std::shared_ptr<Message> message) {
	this->write(uses message->buffer, message->length);
}

void net::SocketClient::start() {
	this->handle->data = (void *) this;

	uv_read_start(
		uses (uv_stream_t *) this->handle,
		[](uses uv_handle_t *, size_t, uses uv_buf_t *buf) {
			/*
				see above comment about malloc() in Message(size_t)

				we're going to keep this bumped down a bit; there's no way
				we need 64kbs of data in a single packet.
			*/
			buf->base = (char *) malloc(BUFFER_SIZE);
			buf->len = BUFFER_SIZE; },
		[](uses uv_stream_t *handle, ssize_t nread, consumes const uv_buf_t *buf) {
			net::SocketClient *self = (net::SocketClient *) handle->data;

			if (nread == 0) {
				/*
					as per the docs, this doesn't mean an error.
					it simple means the socket didn't receive anything.
					just go on with our day.
				*/
			} else if (nread == UV_EOF) {
				/*
					client disconnected
				*/
				self->onDisconnect(0);
			} else if (nread < 0) {
				/*
					error; we'll just disconnect

					this beautiful pile of crap code here is because
					C libraries don't seem to understand their code
					might be used in a re-usable way, thus making passing
					state-related values a pain in the dick.
				*/
				handle->data = (void *) new CloseStatus(self, nread);
				uv_close(uses (uv_handle_t *) handle, [](uses uv_handle_t *handle) {
					net::SocketClient *self = ((CloseStatus *) handle->data)->socket;
					int status = ((CloseStatus *) handle->data)->status;
					handle->data = (void *) self;

					self->onDisconnect(status);
				});
			} else {
				/*
					hey, we received a message.
				*/
				shared_ptr<Message> message(new Message(consumes buf->base, nread));
				self->onData(message);
			}

			free((void *) buf);
		});
}

void net::SocketClient::stop() {
	int result = uv_read_stop((uv_stream_t *) this->handle);

	if (result < 0) {
		out::warn() << "could not stop reading on a client socket (" << uv_get_error_name(result) << ")";
	}
}

void net::SocketClient::close() {
	uv_close(uses (uv_handle_t *) this->handle, 0);
}
