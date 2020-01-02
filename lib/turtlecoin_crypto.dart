import 'dart:async';
import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart';
import 'package:turtlecoin_crypto/src/crypto.dart';
import 'package:turtlecoin_crypto/src/native_types.dart';
import 'package:turtlecoin_crypto/src/types.dart';

export 'src/types.dart';

class TurtlecoinCrypto {
  static const MethodChannel _channel =
      const MethodChannel('turtlecoin_crypto');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}

int hello() {
  return nativeHello();
}

KeyPair generateKeys() {
  final Pointer<NKeyPair> res = nativeGenerateKeys();
  return KeyPair(
    publicKey: PublicKey.fromNative(res.ref.publicKey.ref),
    secretKey: SecretKey.fromNative(res.ref.secretKey.ref),
  );
}

KeyDerivation generateKeyDerivation(
    PublicKey txPublicKey, SecretKey privateViewKey) {
  Pointer<NPublicKey> nPublicKey =
      allocate<NPublicKey>(count: sizeOf<NPublicKey>());
  nPublicKey.ref.data = txPublicKey.toNative().data;
  Pointer<NSecretKey> nSecretKey =
      allocate<NSecretKey>(count: sizeOf<NSecretKey>());
  nSecretKey.ref.data = privateViewKey.toNative().data;

  final Pointer<NKeyDerivation> res =
      nativeGenerateKeyDerivation(nPublicKey, nSecretKey);

  return KeyDerivation.fromNative(res.ref);
}

SecretKey generateViewFromSpend(SecretKey spend) {
  Pointer<NSecretKey> nSpend =
      allocate<NSecretKey>(count: sizeOf<NSecretKey>());
  nSpend.ref.data = spend.toNative().data;

  final Pointer<NSecretKey> res = nativeGenerateViewFromSpend(nSpend);

  return SecretKey.fromNative(res.ref);
}
// TODO: Add Remaining methods (Its late night)
