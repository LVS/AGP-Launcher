//
//  LVSDownloader.m
//  ABP Launcher
//
//  Created by Andy Jeffries (andy@andyjeffries.co.uk) on 17/02/2010.
//  Copyright 2010 LVS Ltd. All rights reserved.
//

#import "LVSDownloader.h"
#import <unistd.h>

@implementation LVSDownloader

-(NSString *)downloadFileAndReturnContent:(NSString *)url
{
	NSLog(@"Downloading '%@'", url);
	NSMutableURLRequest *request = [NSMutableURLRequest 
                                    requestWithURL:[NSURL URLWithString:url]];
	[request setValue:@"ABP-Launcher (Mac)" forHTTPHeaderField:@"User-Agent"];
	NSData *data = [ NSURLConnection sendSynchronousRequest:request returningResponse: nil error: nil ];
	NSString *content = [[NSString alloc] initWithBytes: [data bytes] length:[data length] encoding: NSUTF8StringEncoding];
	return content;
}

-(void)downloadFile:(NSString *)url to:(NSString *)file_name
{
	NSFileManager *fm = [NSFileManager defaultManager];
    BOOL fileExists = [fm fileExistsAtPath:file_name];
	
	if (!fileExists) {
		NSLog(@"Downloading '%@'", url);
		NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL: 
										[NSURL URLWithString:url]];
		[request setValue:@"ABP-Launcher (Mac)" forHTTPHeaderField:@"User-Agent"];
		NSData *data = [ NSURLConnection sendSynchronousRequest:request returningResponse: nil error: nil ];
		NSLog(@"Writing to %@", file_name);
        
		BOOL written = [data writeToFile:file_name 
				  atomically:NO];
		NSLog(@"Written? %@", (written ? @"YES" : @"NO"));		
	}
}

-(NSString *)cacheFolder:(NSString *)server
{
    NSString *server_path = [server stringByReplacingOccurrencesOfRegex:@"[^a-z0-9.]"
                                                             withString:@"_"];
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *path = [paths objectAtIndex:0];
	path = [path stringByAppendingPathComponent:@"ABP Launcher"];
	path = [path stringByAppendingPathComponent:server_path];
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	[fileManager createDirectoryAtPath:path 
		   withIntermediateDirectories:YES 
							attributes:NULL
								 error:NULL];
	return path;
}

-(NSMutableString *)downloadJNLPs:(NSString *)server jnlpTag:(NSInteger)jnlp_tag
{
    NSString *trayItemJNLPURL = nil;
    if (jnlp_tag == 0) {
        trayItemJNLPURL = [NSString stringWithFormat:@"%@%@",
                                     server, @"apps/trayItem.jnlp"];
    } else if (jnlp_tag == 1) {
        trayItemJNLPURL = [NSString stringWithFormat:@"%@%@",
                           server, @"apps/tms_local.jnlp"];
    } else if (jnlp_tag == 2) {
        trayItemJNLPURL = [NSString stringWithFormat:@"%@%@",
                           server, @"apps/retailMonitor.jnlp"];
    } else if (jnlp_tag == 3) {
        trayItemJNLPURL = [NSString stringWithFormat:@"%@%@",
                           server, @"apps/trniviewer.jnlp"];
    }
	NSString *thirdPartyJNLPURL = [NSString stringWithFormat:@"%@%@",
								 server, @"apps/thirdParty.jnlp"];
	NSMutableString *jnlp = [[NSMutableString alloc] init];
	[jnlp appendString:[self downloadFileAndReturnContent:trayItemJNLPURL]];
	[jnlp appendString:[self downloadFileAndReturnContent:thirdPartyJNLPURL]];
	return jnlp;
}

-(void)extractPropertyFiles:(NSString *)jnlp language:(NSString *)language
{
	NSString *regex   = @"<property name=\"(.*?)\" value=\"(.*?)\"";
	NSString *name;
	NSString *value;
	NSString *param;
	
    NSArray *matches = [jnlp arrayOfCaptureComponentsMatchedByRegex:regex];
	NSEnumerator *enumerator = [matches objectEnumerator];
    
    NSArray * object;
    while ((object = [enumerator nextObject])) {
		name = [object objectAtIndex:1];
		value = [object objectAtIndex:2];
		param = [NSString stringWithFormat:@"-D%@=%@", name, value];
		[cmd_args addObject:param];
	}
	if ([language caseInsensitiveCompare:@"1"] == NSOrderedSame) {
		value = @"en_GB";
	}
	else if ([language caseInsensitiveCompare:@"2"] == NSOrderedSame) {
		value = @"fr_FR";
	}
	param = [NSString stringWithFormat:@"-D%@=%@", @"locale", value];
	[cmd_args addObject:param];
}

-(NSArray *)extractJarFiles:(NSString *)jnlp
{
	NSMutableArray *jar_files = [[NSMutableArray alloc] init];
	NSString *regex   = @"<jar href=\"(.*?)\"";
	NSArray *matches = [jnlp arrayOfCaptureComponentsMatchedByRegex:regex];
	NSEnumerator *enumerator = [matches objectEnumerator];

    NSArray * object;
    while ((object = [enumerator nextObject])) {
		NSString *file_name = [object objectAtIndex:1];
		if (![file_name isMatchedByRegex:@"(win32|swt_cocoa64_x86)"]) {
			[jar_files addObject:file_name];
		}
	}
	return jar_files;
}

-(void)downloadJarFiles:(NSArray *)files server:(NSString *)server
{
	NSString *cacheFolder = [self cacheFolder:server];
	int arrayCount = [files count];
	for (int i = 0; i < arrayCount; i++) {
		NSString *file_name = [files objectAtIndex:i];
		NSString *baseName = [file_name lastPathComponent];
		NSString *target = [cacheFolder stringByAppendingPathComponent:baseName];
//        [actionLabel setStringValue:[NSString stringWithFormat:@"Downloading %@", baseName]];
        [actionLabel setStringValue:baseName];
        [actionLabel displayIfNeeded];
		[self downloadFile:[NSString stringWithFormat:@"%@%@", server, file_name] 
						to:target];
		[class_path appendString:[NSString stringWithFormat:@"%@:", target]];
		[progressBar incrementBy:1];
        [progressBar displayIfNeeded];
//        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
	}
}

-(void)copyCocoaJarToDir:(NSString *)server
{
	NSString *fullPath = [[self cacheFolder:server] stringByAppendingPathComponent:@"swt_cocoa64_x86-3.6.2.jar"];
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:fullPath];
	
	if (!fileExists) {
		NSString *jarPath = [[NSBundle mainBundle] pathForResource:@"swt_cocoa64_x86-3.6.2" 
													   ofType:@"jar"];
		NSLog(@"Copying Jar from %@ to %@", jarPath, fullPath);
		[[NSFileManager defaultManager] copyItemAtPath:jarPath toPath:fullPath error:NULL];
	}
	[class_path appendString:fullPath];
//    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
}

-(NSString *)extractMainClass:(NSString *)jnlp
{
    NSString *mainClass = [jnlp stringByMatching:@"<application-desc main-class=\"(.*?)\"" capture:1];
	return mainClass;
}

-(NSMutableString *)fullServerAddress:(NSString *)server_address
{
	NSMutableString *server = [[NSMutableString alloc] initWithString:server_address];
	if (![server isMatchedByRegex:@"^http"]) {
		[server insertString:@"http://" atIndex:0];
		[server appendString:@":8080/abp/"];
	}
	if (![server isMatchedByRegex:@"/$"]) {
		[server appendString:@"/"];
	}
	return server;
}

-(void)pullFromServer:(NSString *)server_address language:(NSString *)language memory:(NSString *)memory debug:(BOOL)debug jnlpTag:(NSInteger)jnlp_tag className:(NSString *)className
{
	NSString *param;
	
	[launchButton setEnabled:NO];
	[launchButton setNeedsDisplay];
	
	NSMutableString *server = [self fullServerAddress:server_address];
	class_path = [[NSMutableString alloc] init];
    cmd_args = [[NSMutableArray alloc] init];
	[cmd_args addObject:@"-XstartOnFirstThread"];
	
	param = [NSString stringWithFormat:@"-Xmx%@m", memory];
	[cmd_args addObject:param];
	
	NSMutableString *jnlp = [self downloadJNLPs:server jnlpTag:jnlp_tag];
	[self extractPropertyFiles:jnlp language:language];
	
	[cmd_args addObject:@"-classpath"];
	NSArray *jar_files = [self extractJarFiles:jnlp];
	[progressBar setMaxValue:[jar_files count]];
	[progressBar incrementBy:2];
	[progressBar displayIfNeeded];
	[launchButton setNeedsDisplay];
	
	[self downloadJarFiles:jar_files server:server];
	[self copyCocoaJarToDir:server];
	// class_path = [NSString stringWithFormat:@"\"%@\"", class_path];
	[cmd_args addObject:class_path];
	
	if (debug) {
		param = [NSString stringWithFormat:@"-Ddebug=true"];
		[cmd_args addObject:param];
	}
	
    if ([className length] != 0) {
        [cmd_args addObject:className];        
    } else {
        [cmd_args addObject:[self extractMainClass:jnlp]];
    }

    [actionLabel setStringValue:@"Launching ABP, please wait..."];
    [actionLabel displayIfNeeded];
    
	OSStatus status = noErr;
	FSRef ref;
	status = FSPathMakeRef((const UInt8 *)[@"/usr/bin/java" fileSystemRepresentation], &ref, NULL);
	LSApplicationParameters params = {0, NSWorkspaceLaunchAsync || kLSLaunchDefaults, &ref, NULL, NULL, (CFArrayRef)cmd_args, NULL};
	status = LSOpenApplication(&params, NULL);
	
	NSLog(@"%@", cmd_args);
	
	sleep(5);
}

@end
