all:
	clang++ -Os -target arm64-apple-ios8.0 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -o unicornvm_apitest -I../include -L../dylib -lunicornvm -framework Foundation -std=gnu++14 -stdlib=libc++ ./apitest.mm
