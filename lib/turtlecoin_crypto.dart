import 'dart:async';

import 'package:flutter/services.dart';

class TurtlecoinCrypto {
  static const MethodChannel _channel =
      const MethodChannel('turtlecoin_crypto');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
