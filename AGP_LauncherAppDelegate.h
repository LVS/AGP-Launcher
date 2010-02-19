//
//  AGP_LauncherAppDelegate.h
//  AGP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AGP_LauncherAppDelegate : NSObject {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;
- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) theApplication;

@end
