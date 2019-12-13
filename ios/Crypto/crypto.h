// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2014-2018, The Monero Project
// Copyright (c) 2016-2018, The Karbowanec developers
// Copyright (c) 2018, The TurtleCoin Developers
// 
// Please see the included LICENSE file for more information.

#pragma once

#include <vector>

#include "CryptoTypes.h"
#include "crypto-ops.h"

namespace Crypto
{
    struct EllipticCurvePoint
    {
        uint8_t data[32];
    };

    struct EllipticCurveScalar
    {
        uint8_t data[32];
    };

    bool generateRingSignatures(
        const Hash prefixHash,
        const KeyImage keyImage,
        const std::vector<PublicKey> publicKeys,
        const Crypto::SecretKey transactionSecretKey,
        uint64_t realOutput,
        std::vector<Signature> &signatures); 

    bool checkRingSignature(
        const Hash &prefix_hash,
        const KeyImage &image,
        const std::vector<PublicKey> pubs,
        const std::vector<Signature> signatures);
    
    void generateViewFromSpend(
        const Crypto::SecretKey &spend,
        Crypto::SecretKey &viewSecret);

    void generateViewFromSpend(
        const Crypto::SecretKey &spend,
        Crypto::SecretKey &viewSecret,
        Crypto::PublicKey &viewPublic);
    
    bool underive_public_key(
        const KeyDerivation &derivation,
        const size_t output_index,
        const PublicKey &derived_key,
        PublicKey &base);

    bool generate_key_derivation(
        const PublicKey &key1,
        const SecretKey &key2,
        KeyDerivation &derivation);

    bool derive_public_key(
        const KeyDerivation &derivation,
        const size_t output_index,
        const PublicKey &base,
        PublicKey &derived_key);

    void derive_secret_key(
        const KeyDerivation &derivation,
        const size_t output_index,
        const SecretKey &base,
        SecretKey &derived_key);

    void generate_key_image(
        const PublicKey &pub,
        const SecretKey &sec,
        KeyImage &image);

    void derivation_to_scalar(
        const KeyDerivation &derivation,
        size_t output_index,
        EllipticCurveScalar &res);

    void hash_to_scalar(
        const void *data,
        const size_t length,
        EllipticCurveScalar &res);

    void hash_to_ec(
        const PublicKey &key,
        ge_p3 &res);

    void hash_data_to_ec(
        const uint8_t *,
        std::size_t,
        PublicKey &);
    
    KeyImage scalarmultKey(
        const KeyImage &P,
        const KeyImage &a);
    
    bool check_signature(
        const Hash &,
        const PublicKey &,
        const Signature &);
    
    void generate_signature(
        const Hash &,
        const PublicKey &,
        const SecretKey &,
        Signature &);
    
    bool underive_public_key_and_get_scalar(
        const KeyDerivation &,
        std::size_t,
        const PublicKey &,
        PublicKey &,
        EllipticCurveScalar &);

    bool secret_key_to_public_key(
        const SecretKey &,
        PublicKey &);
    
    bool check_key(const PublicKey &);

    SecretKey generate_m_keys(
        PublicKey &pub,
        SecretKey &sec,
        const SecretKey &recovery_key = SecretKey(),
        bool recover = false);
    
    void generate_deterministic_keys(
        PublicKey &pub,
        SecretKey &sec,
        SecretKey &second);
    
    void generate_keys(
        PublicKey &,
        SecretKey &);
}
