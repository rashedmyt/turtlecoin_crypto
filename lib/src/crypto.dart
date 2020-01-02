import 'dart:ffi';
import 'dart:io';

import 'package:turtlecoin_crypto/src/native_types.dart';

final DynamicLibrary cryptoLib = Platform.isAndroid
    ? DynamicLibrary.open("libcrypto.so")
    : DynamicLibrary.process();

final int Function() nativeHello =
    cryptoLib.lookup<NativeFunction<Int32 Function()>>("hello").asFunction();

final Pointer<NKeyPair> Function() nativeGenerateKeys = cryptoLib
    .lookup<NativeFunction<Pointer<NKeyPair> Function()>>("GenerateKeys")
    .asFunction();

final Pointer<NKeyDerivation> Function(Pointer<NPublicKey>, Pointer<NSecretKey>)
    nativeGenerateKeyDerivation = cryptoLib
        .lookup<
            NativeFunction<
                Pointer<NKeyDerivation> Function(Pointer<NPublicKey>,
                    Pointer<NSecretKey>)>>("GenerateKeyDerivation")
        .asFunction();

final Pointer<NPublicKey> Function(
        Pointer<NKeyDerivation>, int, Pointer<NPublicKey>)
    nativeUnDerivePublicKey = cryptoLib
        .lookup<
            NativeFunction<
                Pointer<NPublicKey> Function(Pointer<NKeyDerivation>, Uint64,
                    Pointer<NPublicKey>)>>("UnDerivePublicKey")
        .asFunction();

final Pointer<NSecretKey> Function(
        Pointer<NKeyDerivation>, int, Pointer<NSecretKey>)
    nativeDeriveSecretKey = cryptoLib
        .lookup<
            NativeFunction<
                Pointer<NSecretKey> Function(Pointer<NKeyDerivation>, Uint64,
                    Pointer<NSecretKey>)>>("DeriveSecretKey")
        .asFunction();

final Pointer<NPublicKey> Function(
        Pointer<NKeyDerivation>, int, Pointer<NPublicKey>)
    nativeDerivePublicKey = cryptoLib
        .lookup<
            NativeFunction<
                Pointer<NPublicKey> Function(Pointer<NKeyDerivation>, Uint64,
                    Pointer<NPublicKey>)>>("UnDerivePublicKey")
        .asFunction();

final Pointer<NSecretKey> Function(
    Pointer<
        NSecretKey>) nativeGenerateViewFromSpend = cryptoLib
    .lookup<NativeFunction<Pointer<NSecretKey> Function(Pointer<NSecretKey>)>>(
        "GenerateViewFromSpend")
    .asFunction();
