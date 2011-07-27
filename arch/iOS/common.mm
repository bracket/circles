#include <arch/common.hpp>

std::string get_file_contents(char const * name, char const * ext) {
  	NSString * n = [ NSString stringWithCString:name encoding:[NSString defaultCStringEncoding] ];
  	NSString * e = [ NSString stringWithCString:ext encoding:[NSString defaultCStringEncoding] ];
	NSString * path = [ [ NSBundle mainBundle ] pathForResource:n ofType:e ];

    return (char const *)[[NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil] UTF8String];
}
