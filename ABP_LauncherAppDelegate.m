//
//  ABP_LauncherAppDelegate.m
//  ABP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import "ABP_LauncherAppDelegate.h"

@implementation ABP_LauncherAppDelegate

@synthesize window;

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) theApplication
{
	return YES;
}

@end
