#import "TurtlecoinCryptoPlugin.h"
#if __has_include(<turtlecoin_crypto/turtlecoin_crypto-Swift.h>)
#import <turtlecoin_crypto/turtlecoin_crypto-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "turtlecoin_crypto-Swift.h"
#endif

@implementation TurtlecoinCryptoPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftTurtlecoinCryptoPlugin registerWithRegistrar:registrar];
}
@end
