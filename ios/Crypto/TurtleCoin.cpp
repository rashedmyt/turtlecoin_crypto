#include "crypto.h"
#include "TurtleCoin.h"

#include <stdint.h>

extern "C" __attribute__((visibility("default"))) __attribute__((used))
int32_t hello() {
    return 7;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
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

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Crypto::KeyDerivation generateKeyDerivation(
    const Crypto::PublicKey transactionPublicKey,
    const Crypto::SecretKey privateViewKey)
{
    Crypto::KeyDerivation derivation;
    Crypto::generate_key_derivation(transactionPublicKey, privateViewKey, derivation);

    return derivation;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Crypto::KeyImage generateKeyImage(
    const Crypto::PublicKey publicEphemeral,
    const Crypto::SecretKey privateEphemeral)
{
    Crypto::KeyImage keyImage;
    Crypto::generate_key_image(publicEphemeral, privateEphemeral, keyImage);

    return keyImage;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Crypto::SecretKey deriveSecretKey(
    const Crypto::KeyDerivation derivation,
    const size_t outputIndex,
    const Crypto::SecretKey privateSpendKey)
{
    Crypto::SecretKey secretKey;
    Crypto::derive_secret_key(derivation, outputIndex, privateSpendKey, secretKey);

    return secretKey;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Crypto::PublicKey derivePublicKey(
    const Crypto::KeyDerivation derivation,
    const size_t outputIndex,
    const Crypto::PublicKey publicSpendKey)
{
    Crypto::PublicKey derivedKey;
    Crypto::derive_public_key(derivation, outputIndex, publicSpendKey, derivedKey);

    return derivedKey;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used))
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
