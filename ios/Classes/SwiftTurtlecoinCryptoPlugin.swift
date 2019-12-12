import Flutter
import UIKit

public class SwiftTurtlecoinCryptoPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "turtlecoin_crypto", binaryMessenger: registrar.messenger())
    let instance = SwiftTurtlecoinCryptoPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    result("iOS " + UIDevice.current.systemVersion)
  }
}
