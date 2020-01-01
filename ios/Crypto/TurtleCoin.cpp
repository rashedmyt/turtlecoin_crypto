#include "crypto.h"
#include "TurtleCoin.h"

#include <cstdint>

extern "C" __attribute__((visibility("default"))) __attribute__((used))
int32_t hello() {
    return 7;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::KeyDerivation* GenerateKeyDerivation(
        const Native::PublicKey* nativeTxPublicKey,
        const Native::SecretKey* nativePrivateViewKey)
{
    Crypto::PublicKey  transactionPublicKey;
    Crypto::SecretKey privateViewKey;

    std::copy(nativeTxPublicKey->data,nativeTxPublicKey->data+32,transactionPublicKey.data);
    std::copy(nativePrivateViewKey->data,nativePrivateViewKey->data+32,privateViewKey.data);

    Crypto::KeyDerivation derivation;
    Crypto::generate_key_derivation(transactionPublicKey, privateViewKey, derivation);

    auto nativeDerivation = new Native::KeyDerivation();
    std::copy(std::begin(derivation.data),std::end(derivation.data),nativeDerivation->data);

    return nativeDerivation;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::PublicKey* UnDerivePublicKey(
        const Native::KeyDerivation* nativeDerivation,
        const uint64_t outputIndex,
        const Native::PublicKey* nativeDerivedKey)
{
    Crypto::KeyDerivation derivation;
    Crypto::PublicKey derivedKey;

    std::copy(nativeDerivation->data,nativeDerivation->data+32,derivation.data);
    std::copy(nativeDerivedKey->data,nativeDerivedKey->data+32,derivedKey.data);

    Crypto::PublicKey base;
    Crypto::underive_public_key(derivation,outputIndex,derivedKey,base);

    auto nativeBase = new Native::PublicKey();
    std::copy(std::begin(base.data),std::end(base.data),nativeBase->data);

    return nativeBase;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::SecretKey* DeriveSecretKey(
        const Native::KeyDerivation* nativeDerivation,
        const uint64_t outputIndex,
        const Native::SecretKey* nativePrivateSpendKey)
{
    Crypto::KeyDerivation derivation;
    Crypto::SecretKey base;

    std::copy(nativeDerivation->data,nativeDerivation->data+32,derivation.data);
    std::copy(nativePrivateSpendKey->data,nativePrivateSpendKey->data+32,base.data);

    Crypto::SecretKey derivedKey;
    Crypto::derive_secret_key(derivation,outputIndex,base,derivedKey);

    auto nativeDerivedKey = new Native::SecretKey();
    std::copy(std::begin(base.data),std::end(base.data),nativeDerivedKey->data);

    return nativeDerivedKey;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::PublicKey* DerivePublicKey(
        const Native::KeyDerivation* nativeDerivation,
        const uint64_t outputIndex,
        const Native::PublicKey* nativeBase)
{
    Crypto::KeyDerivation derivation;
    Crypto::PublicKey base;

    std::copy(nativeDerivation->data,nativeDerivation->data+32,derivation.data);
    std::copy(nativeBase->data,nativeBase->data+32,base.data);

    Crypto::PublicKey derivedKey;
    Crypto::underive_public_key(derivation,outputIndex,base,derivedKey);

    auto nativeDerivedKey = new Native::PublicKey();
    std::copy(std::begin(derivedKey.data),std::end(derivedKey.data),nativeDerivedKey->data);

    return nativeDerivedKey;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::KeyPair* GenerateKeys() {
    Crypto::PublicKey publicKey;
    Crypto::SecretKey secretKey;

    Crypto::generate_keys(publicKey,secretKey);

    auto nativeKeyPair = new Native::KeyPair();
    std::copy(std::begin(publicKey.data),std::end(publicKey.data),nativeKeyPair->publicKey->data);
    std::copy(std::begin(secretKey.data),std::end(secretKey.data),nativeKeyPair->secretKey->data);

    return nativeKeyPair;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Native::SecretKey* GenerateViewFromSpend(Native::SecretKey* nativeSpend) {
    Crypto::SecretKey spend;
    std::copy(nativeSpend->data,nativeSpend->data+32,spend.data);

    Crypto::SecretKey secretViewKey;

    Crypto::generateViewFromSpend(spend,secretViewKey);

    auto nativeSecretViewKey = new Native::SecretKey();
    std::copy(std::begin(secretViewKey.data),std::end(secretViewKey.data),nativeSecretViewKey->data);

    return nativeSecretViewKey;
}

// TODO: Wrapper for generateRingSignatures
// TODO: Wrapper for checkRingSignature

// TODO: Remove Leftovers
bool GenerateRingSignatures(
    const Crypto::Hash prefixHash,
    const Crypto::KeyImage keyImage,
    const std::vector<Crypto::PublicKey> publicKeys,
    const Crypto::SecretKey transactionSecretKey,
    uint64_t realOutput)
{
    std::vector<Crypto::Signature> signatures;
    auto success = Crypto::generateRingSignatures(
        prefixHash, keyImage, publicKeys, transactionSecretKey, realOutput, signatures);

    return success;
}

Crypto::KeyImage generateKeyImage(
    const Crypto::PublicKey publicEphemeral,
    const Crypto::SecretKey privateEphemeral)
{
    Crypto::KeyImage keyImage;
    Crypto::generate_key_image(publicEphemeral, privateEphemeral, keyImage);

    return keyImage;
}

std::tuple<Crypto::PublicKey, TransactionInput>* processBlockOutputs(
    const WalletBlockInfo &block,
    const Crypto::SecretKey &privateViewKey,
    const std::unordered_map<Crypto::PublicKey, Crypto::SecretKey> &spendKeys,
    const bool isViewWallet,
    const bool processCoinbaseTransactions)
{

    std::vector<std::tuple<Crypto::PublicKey, TransactionInput>> inputs;

    /* Process the coinbase tx if we're not skipping them for speed */
    if (processCoinbaseTransactions && block.coinbaseTransaction)
    {
        processTransactionOutputs(
            *block.coinbaseTransaction, privateViewKey, spendKeys, isViewWallet, inputs);
    }

    /* Process the normal txs */
    for (const auto &tx : block.transactions)
    {
        processTransactionOutputs(
            tx, privateViewKey, spendKeys, isViewWallet, inputs);
    }

    return inputs.data();
}

void processTransactionOutputs(
    const RawTransaction &tx,
    const Crypto::SecretKey &privateViewKey,
    const std::unordered_map<Crypto::PublicKey, Crypto::SecretKey> &spendKeys,
    const bool isViewWallet,
    std::vector<std::tuple<Crypto::PublicKey, TransactionInput>> &inputs)
{
    Crypto::KeyDerivation derivation;

    /* Generate the key derivation from the random tx public key, and our private
       view key */
    Crypto::generate_key_derivation(
        tx.transactionPublicKey, privateViewKey, derivation);

    uint32_t outputIndex = 0;

    for (const auto &output : tx.keyOutputs)
    {
        Crypto::PublicKey derivedSpendKey;

        /* Derive the public spend key from the transaction, using the previous
           derivation */
        Crypto::underive_public_key(
            derivation, outputIndex, output.key, derivedSpendKey);

        /* See if the derived spend key matches any of our spend keys */
        const auto ourPrivateSpendKey = spendKeys.find(derivedSpendKey);

        /* If it does, the transaction belongs to us */
        if (ourPrivateSpendKey != spendKeys.end())
        {
            TransactionInput input;

            input.amount = output.amount;
            input.transactionIndex = outputIndex;
            input.globalOutputIndex = output.globalIndex;
            input.key = output.key;
            input.parentTransactionHash = tx.hash;

            if (!isViewWallet)
            {
                /* Make a temporary key pair */
                Crypto::PublicKey tmpPublicKey;
                Crypto::SecretKey tmpSecretKey;

                /* Get the tmp public key from the derivation, the index,
                   and our public spend key */
                Crypto::derive_public_key(
                    derivation, outputIndex, derivedSpendKey, tmpPublicKey);

                /* Get the tmp private key from the derivation, the index,
                   and our private spend key */
                Crypto::derive_secret_key(
                    derivation, outputIndex, ourPrivateSpendKey->second, tmpSecretKey);

                /* Get the key image from the tmp public and private key */
                Crypto::generate_key_image(
                    tmpPublicKey, tmpSecretKey, input.keyImage);
            }

            inputs.emplace_back(derivedSpendKey, input);
        }

        outputIndex++;
    }
}

