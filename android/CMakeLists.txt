set(CRYPTO
        ../ios/Crypto/TurtleCoin.cpp
        ../ios/Crypto/crypto.cpp
        ../ios/Crypto/crypto-ops.cpp
        ../ios/Crypto/crypto-ops-data.cpp
        ../ios/Crypto/hash.cpp
        ../ios/Crypto/keccak.cpp
)

add_library(crypto SHARED ${CRYPTO})