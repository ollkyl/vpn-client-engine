#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <QByteArray>

// Thin wrapper around OpenSSL EVP used for local settings encryption.
// All functions report failures by returning an empty QByteArray and logging, without throwing.
//
// Wire compatibility note: encryptAes256Cbc/decryptAes256Cbc reproduce the exact
// behaviour of QSimpleCrypto::QBlockCipher (AES-256-CBC, PKCS7 padding, first
// 16 bytes of the IV buffer), so settings written by Amnezia Client stay readable.
namespace CryptoUtils
{
    // Cryptographically secure random bytes of the requested size.
    QByteArray generateRandomBytes(int size);

    // AES-256-CBC with PKCS7 padding. key must be 32 bytes, iv must be at least
    // 16 bytes (only the first 16 are used, matching the legacy behaviour).
    QByteArray encryptAes256Cbc(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
    QByteArray decryptAes256Cbc(const QByteArray &data, const QByteArray &key, const QByteArray &iv);
}

#endif // CRYPTOUTILS_H
