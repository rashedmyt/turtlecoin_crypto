#include "Types.h"

#include <unordered_map>

void processTransactionOutputs(
        const RawTransaction &tx,
        const Crypto::SecretKey &privateViewKey,
        const std::unordered_map<Crypto::PublicKey, Crypto::SecretKey> &spendKeys,
        const bool isViewWallet,
        std::vector<std::tuple<Crypto::PublicKey, TransactionInput>> &inputs);
