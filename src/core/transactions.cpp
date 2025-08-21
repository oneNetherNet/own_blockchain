#include "core/transactions.h"
#include <openssl/sha.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <iomanip>
#include <sstream>

std::string sha256(std::string str) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	std::stringstream ss;
	for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);

	return ss.str();
}

std::vector<unsigned char> sha256_raw(const std::string& data) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, data.data(), data.size());
	SHA256_Final(hash, &ctx);
	return std::vector<unsigned char>(hash, hash + SHA256_DIGEST_LENGTH);
}

EC_KEY* Transaction::keypair_gen() const {
	EC_KEY* key = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (!key) throw std::runtime_error("EC_KEY_new_by_curve_name failed");

	if (EC_KEY_generate_key(key) != 1)
		throw std::runtime_error("EC_KEY_generate_key failed");
	return key;
}

void Transaction::sign_input(int index, EC_KEY* prv_key) {
	std::vector<unsigned char> msg = sha256_raw(serialize_for_sign(index));
	std::string signature = sign(msg, prv_key);
	
	vin[index].sign = signature;
	vin[index].pub_key = get_pub_hex(prv_key);
}


bool Transaction::verify_sign_input(int index, EC_KEY* pub_key) const {
	std::vector<unsigned char> msg = sha256_raw(serialize_for_sign(index));
	return verify_sign(msg, vin[index].sign, pub_key);
}

std::string Transaction::get_pub_hex(EC_KEY* key) const {
	const EC_POINT* pub = EC_KEY_get0_public_key(key);
	const EC_GROUP* group = EC_KEY_get0_group(key);

	unsigned char* buf = nullptr;
	size_t len = EC_POINT_point2buf(group, pub, POINT_CONVERSION_UNCOMPRESSED, &buf, NULL);

	std::stringstream ss;
	for (size_t i = 0; i < len; i++)
		ss << std::setw(2) << std::setfill('0') << static_cast<int>(buf[i]);

	OPENSSL_free(buf);
	return ss.str();
}

std::string Transaction::get_prv_hex(EC_KEY* key) const {
	const BIGNUM* priv_bn = EC_KEY_get0_private_key(key);
	char* hex = BN_bn2hex(priv_bn);
	std::string privKey(hex);
	OPENSSL_free(hex);
	return privKey;
}

std::string Transaction::get_txid() const {
	std::string str = serialize();
	return sha256(sha256(str));
}

std::string Transaction::sign(const std::vector<unsigned char>& msg, EC_KEY* prv_key) const {
	unsigned char uc_msg[SHA256_DIGEST_LENGTH];
	for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) uc_msg[i] = msg[i];

	ECDSA_SIG* signature = ECDSA_do_sign(uc_msg, SHA256_DIGEST_LENGTH, prv_key);
	if (!signature) throw::std::runtime_error("Sign failed");

	unsigned char* der = nullptr;
	int derlen = i2d_ECDSA_SIG(signature, &der);
	std::string res;
	for (size_t i = 0; i < derlen; i++) res.push_back(der[i]);

	OPENSSL_free(der);
	ECDSA_SIG_free(signature);

	return res;
}

bool Transaction::verify_sign(const std::vector<unsigned char>& msg, const std::string& sign, EC_KEY* pub_key) const {
	unsigned char uc_msg[SHA256_DIGEST_LENGTH];
	unsigned char* uc_sgn = new unsigned char[sign.size()];
	for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) uc_msg[i] = msg[i];
	for (size_t i = 0; i < sign.size(); i++) uc_sgn[i] = sign[i];

	const unsigned char* const_uc_sgn = uc_sgn;


	ECDSA_SIG* signature = d2i_ECDSA_SIG(NULL, &const_uc_sgn, sign.size());
	int ok = ECDSA_do_verify(uc_msg, SHA256_DIGEST_LENGTH, signature, pub_key);

	delete[] uc_sgn;
	ECDSA_SIG_free(signature);

	return ok == 1;
}

std::string Transaction::serialize() const {
	std::stringstream ss;

	for (auto& input : vin)
		ss << input.prev_txid << input.pub_key << input.sign << std::to_string(input.out_index);

	for (auto& output : vout)
		ss << output.address << output.value;

	return ss.str();
}

std::string Transaction::serialize_for_sign(int index) const {
	std::stringstream ss;

	for (size_t i = 0; i < vin.size(); i++)
		ss << vin[i].prev_txid << std::to_string(vin[i].out_index);

	for (auto& output : vout)
		ss << output.address << output.value;

	return ss.str();
}