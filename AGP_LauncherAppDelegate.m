//
//  AGP_LauncherAppDelegate.m
//  AGP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import "AGP_LauncherAppDelegate.h"

@implementation AGP_LauncherAppDelegate

@synthesize window;

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) theApplication
{
	return YES;
}

@end
