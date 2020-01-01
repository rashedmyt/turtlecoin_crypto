#include "Types.h"

#include <unordered_map>

namespace Native
{
    struct PublicKey
    {
        uint8_t* data;

        PublicKey() {
            data = new uint8_t[32];
        }
    };

    struct SecretKey
    {
        uint8_t* data;

        SecretKey() {
            data = new uint8_t[32];
        }
    };

    struct KeyDerivation
    {
        uint8_t* data;

        KeyDerivation() {
            data = new uint8_t[32];
        }
    };

    struct KeyImage
    {
        uint8_t* data;

        KeyImage() {
            data = new uint8_t[32];
        }
    };

    struct Signature
    {
        uint8_t* data;

        Signature() {
            data = new uint8_t[64];
        }
    };

    struct KeyPair
    {
        PublicKey* publicKey;
        SecretKey* secretKey;

        KeyPair() {
            publicKey = new PublicKey();
            secretKey = new SecretKey();
        }
    };
}

void processTransactionOutputs(
        const RawTransaction &tx,
        const Crypto::SecretKey &privateViewKey,
        const std::unordered_map<Crypto::PublicKey, Crypto::SecretKey> &spendKeys,
        const bool isViewWallet,
        std::vector<std::tuple<Crypto::PublicKey, TransactionInput>> &inputs);
