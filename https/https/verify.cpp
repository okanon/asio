#include "verify.h"

template<class Verifier>
verbose_verification<Verifier>::verbose_verification(Verifier verifier)
	: verifier_(verifier) {
}

template<class Verifier>
bool verbose_verification<Verifier>::operator()(
	bool preverified, 
	boost::asio::ssl::verify_context & ctx) {
	char subject_name[256];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	bool verified = verifier_(preverified, ctx);
	std::cout << "Verifying: " << subject_name << "\n"
		"Verified: " << verified << std::endl;
	return verified;

}

template<typename Verifier>
verbose_verification<Verifier> make_verbose_verification(Verifier verifier) {
	return verbose_verification<Verifier>(verifier);
}