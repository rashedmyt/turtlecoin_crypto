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
    .asFunction<Pointer<NKeyPair> Function()>();
