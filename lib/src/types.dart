import 'dart:ffi';
import 'dart:typed_data';

import 'package:turtlecoin_crypto/src/native_types.dart';

class PublicKey {
  Uint8List data;

  PublicKey({this.data}) {
    if (data == null) {
      data = Uint8List(32);
    }
  }

  factory PublicKey.fromNative(NPublicKey native) {
    return PublicKey(data: native.data.asTypedList(32));
  }
}

class SecretKey {
  Uint8List data;

  SecretKey({this.data}) {
    if (data == null) {
      data = Uint8List(32);
    }
  }

  factory SecretKey.fromNative(NSecretKey native) {
    return SecretKey(data: native.data.asTypedList(32));
  }
}

class KeyDerivation {
  Uint8List data;

  KeyDerivation({this.data}) {
    if (data == null) {
      data = Uint8List(32);
    }
  }

  factory KeyDerivation.fromNative(NKeyDerivation native) {
    return KeyDerivation(data: native.data.asTypedList(32));
  }
}

class KeyPair {
  PublicKey publicKey;
  SecretKey secretKey;

  KeyPair({this.publicKey, this.secretKey});

  factory KeyPair.fromNative(NKeyPair native) {
    return KeyPair(
        publicKey: PublicKey.fromNative(native.publicKey.ref),
        secretKey: SecretKey.fromNative(native.secretKey.ref));
  }
}
