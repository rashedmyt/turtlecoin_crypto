import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:turtlecoin_crypto/turtlecoin_crypto.dart';

void main() {
  const MethodChannel channel = MethodChannel('turtlecoin_crypto');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await TurtlecoinCrypto.platformVersion, '42');
  });
}
