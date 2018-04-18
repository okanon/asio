#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace inc {
	class client : public std::enable_shared_from_this<client>
	{
	public:
		client(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

		void get(const char* host, const char* path);

		void run();

		void handle_connect(const boost::system::error_code& error);

		void handle_handshake(const boost::system::error_code& error);

		void handle_write_request(const boost::system::error_code& err);

		void handle_read_status_line(const boost::system::error_code& err);

		void handle_read_headers(const boost::system::error_code& err);

		void handle_read_content(const boost::system::error_code& err);



	private:
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator_;
		unsigned int status_code_ = 0;
		std::ostringstream message_header_;
		std::ostringstream message_body_;
		boost::asio::streambuf request_;
		boost::asio::streambuf response_;
		const char* host_;
		const char* path_;
		const char* method_;
	};
}