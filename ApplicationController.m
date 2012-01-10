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

	serverList = [NSMutableArray arrayWithContentsOfFile:[self serverListFilename]];
	if (!serverList) {
		serverList = [[NSMutableArray alloc] init];
	}
	unsigned count = [serverList count];
	while (count--) {
		NSString *s = [serverList objectAtIndex:count];
		if ([s caseInsensitiveCompare:server] == NSOrderedSame) {
			obj = s;
            break;
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
    [clearCacheButton setHidden:YES];
    [launchButton setHidden:YES];
    [actionLabel setStringValue:@"Downloading ABP..."];
    
    NSRect frame = [window frame];
    frame.origin.y += frame.size.height - 150;
    frame.size.height = 150;
    [window setFrame:frame display:NO animate:YES];
    
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
    
	NSString *server_address = [[serverComboBox cell] stringValue];
	NSString *language = [[languagePopUpButton selectedItem] title];
	NSString *memory = [[memoryPopUpButton cell] titleOfSelectedItem];
	NSInteger debugMode = [debugCheckbox state];
	NSString *className = [classNameToRun stringValue];
	NSInteger jnlp_tag = [applicationChoice selectedTag];
    NSInteger widget_tag = [widgetChoice selectedTag];
    
	[downloader pullFromServer:server_address language:language memory:memory debug:(debugMode == 1) jnlpTag:jnlp_tag className:className
                     widgetTag:widget_tag];
	[self addServer:server_address];
	
	[[NSApplication sharedApplication] terminate:nil];
}

- (IBAction)clickClearCache:(id)sender {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *path = [paths objectAtIndex:0];
	path = [path stringByAppendingPathComponent:@"ABP Launcher"];
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    [fileManager removeItemAtPath:path error:NULL];
    [clearCacheButton setEnabled:NO];
    [clearCacheMenuItem setEnabled:NO];
    NSAlert *alert = [NSAlert alertWithMessageText: @"ABP Launcher"
                    defaultButton: @"OK"
                    alternateButton: nil
                    otherButton: nil
        informativeTextWithFormat: @"Your cache has been successfully cleared."];
    [alert beginSheetModalForWindow: window
                      modalDelegate: nil
                     didEndSelector: nil
                        contextInfo: nil];
}

- (IBAction)clickAdvancedDisclosure:(id)sender {
    NSRect frame = [window frame];
    CGFloat sizeChange = [advancedOptionsBox frame].size.height;
    switch ([sender state]) {
        case NSOnState:
            // Show the extra box.
            [advancedOptionsBox setHidden:NO];
            // Make the window bigger.
            frame.size.height += sizeChange;
            // Move the origin.
            frame.origin.y -= sizeChange;
            break;
        case NSOffState:
            // Hide the extra box.
            [advancedOptionsBox setHidden:YES];
            // Make the window smaller.
            frame.size.height -= sizeChange;
            // Move the origin.
            frame.origin.y += sizeChange;
            break;
        default:
            break;
    }
    [window setFrame:frame display:YES animate:YES];
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
    
    [advancedOptionsBox setHidden:YES];
    NSRect frame = [window frame];
    CGFloat sizeChange = [advancedOptionsBox frame].size.height;
    frame.size.height -= sizeChange;
    frame.origin.y += sizeChange;
    [window setFrame:frame display:YES animate:YES];
    
    [applicationChoice selectItemAtIndex:0];
    [widgetChoice selectItemAtIndex:0];
}

@end
