#include "https.h"
#include "verify.h"

namespace inc {
	client::client(boost::asio::io_service& io_service,
		boost::asio::ssl::context& context,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		: socket_(io_service, context), endpoint_iterator_(endpoint_iterator) {
	}

	void client::get(const char* host, const char* path) {
		host_ = host;
		path_ = path;
		method_ = "GET";

		boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator_,
			boost::bind(&client::handle_connect, shared_from_this(),
				boost::asio::placeholders::error));
	}

	void client::run() {

		boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator_,
			boost::bind(&client::handle_connect, shared_from_this(),
				boost::asio::placeholders::error));
	}

	void client::handle_connect(const boost::system::error_code& error) {
		socket_.set_verify_callback(make_verbose_verification(
			boost::asio::ssl::rfc2818_verification(host_)));

		if (!error)
		{
			socket_.async_handshake(boost::asio::ssl::stream_base::client,
				boost::bind(&client::handle_handshake, shared_from_this(),
					boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Connect failed: " << error.message() << "\n";
		}
	}

	void client::handle_handshake(const boost::system::error_code& error) {
		if (!error)
		{
			std::ostream request_stream(&request_);
			{
				//std::stringbuf str;
				//request_stream.rdbuf(&str);

				request_stream << method_ << " " << path_ << " HTTP/1.0\r\n";
				request_stream << "Host: " << host_ << "\r\n";
				request_stream << "Accept: */*\r\n";

				request_stream << "Connection: close\r\n\r\n";

				//std::cout << "str = '" << str.str() << "'\n" << std::endl;
			}
			boost::asio::async_write(socket_, request_,
				boost::bind(&client::handle_write_request, shared_from_this(),
					boost::asio::placeholders::error));

		}
		else
		{
			std::cout << "Handshake failed: " << error.message() << "\n";
		}
	}

	void client::handle_write_request(const boost::system::error_code& err) {
		if (!err)
		{
			boost::asio::async_read_until(socket_, response_, "\r\n",
				boost::bind(&client::handle_read_status_line, shared_from_this(),
					boost::asio::placeholders::error));
		}
		else
		{
			std::cerr << "Error: " << err.message() << "\n";
		}
	}

	void client::handle_read_status_line(const boost::system::error_code& err) {
		if (!err)
		{
			std::istream response_stream(&response_);
			std::string http_version;
			response_stream >> http_version;
			response_stream >> status_code_;
			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				std::cerr << "Invalid response\n";
				return;
			}
			if (status_code_ != 200)
			{
				std::cout << "Response returned with status code ";
				std::cout << status_code_ << "\n";
				return;
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&client::handle_read_headers, shared_from_this(),
					boost::asio::placeholders::error));
		}
		else
		{
			std::cerr << "Error: " << err << "\n";
		}
	}


	void client::handle_read_headers(const boost::system::error_code& err) {
		if (!err)
		{
			// 
			std::istream response_stream(&response_);
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				message_header_ << header << "\n";
			}

			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&client::handle_read_content, shared_from_this(),
					boost::asio::placeholders::error));
		}
		else
		{
			std::cerr << "Error: " << err << "\n";
		}
	}

	void client::handle_read_content(const boost::system::error_code& err) {
		//      if (err == boost::asio::error::eof)
		if (err)
		{
			auto  self(shared_from_this());

			message_body_ << &response_;


			std::cout << status_code_ << std::endl << message_header_.str() << std::endl << message_body_.str() << std::endl;

		}
		else if (!err)
		{
			message_body_ << &response_;

			response_.consume(response_.size());

			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&client::handle_read_content, shared_from_this(),
					boost::asio::placeholders::error));

		}
	}
}