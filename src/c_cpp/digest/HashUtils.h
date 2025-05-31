#ifndef HASHUTILS_H
#define HASHUTILS_H

#include <vector>
#include <string>
#include <openssl/evp.h>

class HashUtils
{
public:
    HashUtils();

    /**
     * @brief 将 const std::vector<unsigned char> 类型的 key 转换为十六进制字符串
     * @param key 密钥
     * @return 十六进制字符串
     */
    static std::string keyToHexString(const std::vector<unsigned char>& key);

    /**
     * @brief 将十六进制字符串转换为 const std::vector<unsigned char> 类型
     * @param hexString 十六进制字符串
     * @return 密钥
     */
    static std::vector<unsigned char> hexStringToKey(const std::string& hexString);

    /**
     * @brief 计算MD5摘要
     * @param plaintext 原文
     * @return MD5 digest
     */
    static std::string calculateMD5(const std::string& plaintext);

    /**
     * @brief 计算SHA1摘要
     * @param plaintext 原文
     * @return SHA1 digest
     */
    static std::string calculateSHA1(const std::string& plaintext);

    /**
     * @brief 计算SHA256摘要
     * @param plaintext 原文
     * @return SHA256 digest
     */
    static std::string calculateSHA256(const std::string& plaintext);

    /**
     * @brief 计算SHA384摘要
     * @param plaintext 原文
     * @return SHA384 digest
     */
    static std::string calculateSHA384(const std::string& plaintext);


   /**
    * @brief 计算SHA512摘要
    * @param plaintext 原文
    * @return SHA512 digest
    */
   static std::string calculateSHA512(const std::string& plaintext);

private:

    /**
     * @brief 计算摘要
     * @param plaintext 明文
     * @param hashType 散列类型
     * @return 计算结果
     */
    static std::string calculateHash(const std::string& plaintext, const EVP_MD* hashType);
};

#endif // HASHUTILS_H
