#include <https.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

int main() {
	try
	{
		boost::asio::io_service io_service;

		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query("www.google.co.jp", "https");
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

		boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
		ctx.load_verify_file("YOUR_ROOT_CA_CERTIFICATE");

		std::make_shared<inc::client>(io_service, ctx, iterator)->get("www.google.co.jp", "/");

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	getchar();

	return 0;
}