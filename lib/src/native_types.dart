import 'dart:ffi';

import 'package:ffi/ffi.dart';

class NPublicKey extends Struct {
  Pointer<Uint8> data;

  factory NPublicKey.allocate(Pointer<Uint8> data) =>
      allocate<NPublicKey>().ref..data = data;
}

class NSecretKey extends Struct {
  Pointer<Uint8> data;

  factory NSecretKey.allocate(Pointer<Uint8> data) =>
      allocate<NSecretKey>().ref..data = data;
}

class NKeyDerivation extends Struct {
  Pointer<Uint8> data;

  factory NKeyDerivation.allocate(Pointer<Uint8> data) =>
      allocate<NKeyDerivation>().ref..data = data;
}

class NKeyImage extends Struct {
  Pointer<Uint8> data;

  factory NKeyImage.allocate(Pointer<Uint8> data) =>
      allocate<NKeyImage>().ref..data = data;
}

class NSignature extends Struct {
  Pointer<Uint8> data;

  factory NSignature.allocate(Pointer<Uint8> data) =>
      allocate<NSignature>().ref..data = data;
}

class NKeyPair extends Struct {
  Pointer<NPublicKey> publicKey;
  Pointer<NSecretKey> secretKey;

  factory NKeyPair.allocate(
          Pointer<NPublicKey> publicKey, Pointer<NSecretKey> secretKey) =>
      allocate<NKeyPair>().ref
        ..publicKey
        ..secretKey;
}
