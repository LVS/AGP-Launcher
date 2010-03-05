//
//  ApplicationController.m
//  AGP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import "ApplicationController.h"


@implementation ApplicationController

-(NSString *)serverListFilename
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *path = [paths objectAtIndex:0];
	path = [path stringByAppendingPathComponent:@"AGP Launcher"];
	
	[[NSFileManager defaultManager] createDirectoryAtPath:path
							  withIntermediateDirectories:YES 
											   attributes:nil
													error:nil];
	return [path stringByAppendingPathComponent:@"servers.plist"];
}

-(void)saveServerList
{
	NSString *path = [self serverListFilename];
	[serverList writeToFile:path atomically:YES];
}

-(void)addServer:(NSString *)server
{
	NSString *obj = nil;
	char *cs;

	serverList = [NSMutableArray arrayWithContentsOfFile:[self serverListFilename]];
	if (!serverList) {
		serverList = [[NSMutableArray alloc] init];
	}
	unsigned count = [serverList count];
	while (count--) {
		cs = (char *)[serverList objectAtIndex:count];
		NSString *s = [NSString stringWithCString:cs encoding:NSUTF8StringEncoding];
		if ([s caseInsensitiveCompare:server] == NSOrderedSame) {
			obj = s;
		}
	}
	if (obj) {
		[serverList removeObject:obj];
	}
	[serverList insertObject:server atIndex:0];
	[self saveServerList];
}

-(IBAction)clickLaunch:(id)sender
{
	NSString *server_address = [[serverComboBox cell] stringValue];
	NSString *language = [[languagePopUpButton cell] stringValue];
	NSInteger debugMode = [debugCheckbox state];
	[downloader pullFromServer:server_address language:language debug:(debugMode == 1)];
	[self addServer:server_address];
	
	[[NSApplication sharedApplication] terminate:nil];
}

-(void) awakeFromNib 
{
	NSString *path = [self serverListFilename];
	serverList = [NSMutableArray arrayWithContentsOfFile:path];
	if (!serverList) {
		serverList = [[NSMutableArray alloc] init];
	}
	else {
		[serverComboBox addItemsWithObjectValues:serverList];
		if ([serverComboBox numberOfItems] > 0) {
			[serverComboBox selectItemAtIndex:0]; 
		}
	}
}

@end
