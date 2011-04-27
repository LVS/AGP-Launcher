//
//  ApplicationController.m
//  ABP Launcher
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
	path = [path stringByAppendingPathComponent:@"ABP Launcher"];
	
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
    [sender setHidden:YES];
    [progressLabel setHidden:NO];
    [formLabel setHidden:YES];
    [progressBar setHidden:NO];
    [actionLabel setHidden:NO];
    [actionLabel setStringValue:@"Downloading ABP..."];
    
    NSWindow *window = [sender window];
    NSRect frame = [window frame];
    frame.origin.y += frame.size.height - 150;
    frame.size.height = 150;
    [window setFrame:frame display:NO animate:YES];
    
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
    
	NSString *server_address = [[serverComboBox cell] stringValue];
	NSString *language = [[languagePopUpButton cell] stringValue];
	NSString *memory = [[memoryPopUpButton cell] titleOfSelectedItem];
	NSInteger debugMode = [debugCheckbox state];
    
	[downloader pullFromServer:server_address language:language memory:memory debug:(debugMode == 1)];
	[self addServer:server_address];
	
	[[NSApplication sharedApplication] terminate:nil];
}

- (IBAction)clickClearCache:(id)sender {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *path = [paths objectAtIndex:0];
	path = [path stringByAppendingPathComponent:@"ABP Launcher"];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtPath:path error:NULL];
    [sender setEnabled:NO];
    NSRunAlertPanel(@"ABP Launcher", @"Your cache has been successfully cleared.", @"OK", nil,nil);
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
