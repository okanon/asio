#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

template <class Verifier> class verbose_verification {
public:
	verbose_verification(Verifier verifier);

	bool operator()(
		bool preverified,
		boost::asio::ssl::verify_context& ctx
	);
	
private:
	Verifier verifier_;
};

template <typename Verifier>
verbose_verification<Verifier>
make_verbose_verification(Verifier verifier);
