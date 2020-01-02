import 'dart:ffi';
import 'dart:typed_data';

import 'package:ffi/ffi.dart';
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

  NPublicKey toNative() {
    Pointer<Uint8> res = allocate<Uint8>(count: 33);
    Uint8List nativeList = res.asTypedList(33);
    nativeList.setAll(0, data);
    nativeList[32] = 0;

    NPublicKey native = NPublicKey.allocate(res.cast());

    return native;
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

  NSecretKey toNative() {
    Pointer<Uint8> res = allocate<Uint8>(count: 33);
    Uint8List nativeList = res.asTypedList(33);
    nativeList.setAll(0, data);
    nativeList[32] = 0;

    NSecretKey native = NSecretKey.allocate(res.cast());

    return native;
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

  NKeyDerivation toNative() {
    Pointer<Uint8> res = allocate<Uint8>(count: 33);
    Uint8List nativeList = res.asTypedList(33);
    nativeList.setAll(0, data);
    nativeList[32] = 0;

    NKeyDerivation native = NKeyDerivation.allocate(res.cast());

    return native;
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
