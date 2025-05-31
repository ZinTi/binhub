#include "hash_utils.h"
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>

HashUtils::HashUtils() = default;

// 将 const std::vector<unsigned char> 类型的 key 转换为十六进制字符串
std::string HashUtils::keyToHexString(const std::vector<unsigned char>& key) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    // 遍历 key 中的每个字节
    for (const unsigned char c : key) {
        // 将每个字节转换为两位十六进制字符串
        oss << std::setw(2) << static_cast<int>(c);
    }

    // 返回拼接好的十六进制字符串
    return oss.str();
}

// 将十六进制字符串转换为 const std::vector<unsigned char> 类型
std::vector<unsigned char> HashUtils::hexStringToKey(const std::string& hexString) {
    std::vector<unsigned char> key;
    if (hexString.length() % 2 != 0) {
        // "Invalid hex string"
        return key;
    }

    for (size_t i = 0; i < hexString.length(); i += 2) {
        std::string byteString = hexString.substr(i, 2);
        unsigned int byte;
        std::istringstream iss(byteString);
        iss >> std::hex >> byte;
        key.push_back(static_cast<unsigned char>(byte));
    }

    return key;
}

std::string HashUtils::calculateMD5(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_md5());
}

std::string HashUtils::calculateSHA1(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_sha1());
}

std::string HashUtils::calculateSHA256(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_sha256());
}

std::string HashUtils::calculateSHA384(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_sha384());
}

std::string HashUtils::calculateSHA512(const std::string& plaintext) {
    return calculateHash(plaintext, EVP_sha512());
}

std::string HashUtils::calculateHash(const std::string& plaintext, const EVP_MD* hashType) {
    // 初始化 OpenSSL 库（线程安全，多次调用无害）
    OPENSSL_init_crypto(OPENSSL_INIT_ADD_ALL_DIGESTS, nullptr);

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) return {};

    // 初始化摘要上下文
    if (1 != EVP_DigestInit_ex(context, hashType, nullptr)) {
        EVP_MD_CTX_free(context);
        return {};
    }

    // 更新摘要计算
    if (1 != EVP_DigestUpdate(context, plaintext.data(), plaintext.size())) {
        EVP_MD_CTX_free(context);
        return {};
    }

    // 获取摘要结果
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength;
    if (1 != EVP_DigestFinal_ex(context, hash, &hashLength)) {
        EVP_MD_CTX_free(context);
        return {};
    }

    EVP_MD_CTX_free(context);

    // 将二进制哈希值转换为十六进制字符串
    std::string result;
    result.reserve(hashLength * 2);
    for (unsigned int i = 0; i < hashLength; ++i) {
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", hash[i]);
        result.append(hex);
    }

    return result;
}
