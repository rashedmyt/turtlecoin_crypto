import 'dart:async';
import 'dart:ffi';

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
