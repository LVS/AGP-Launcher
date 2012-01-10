//
//  ApplicationController.h
//  ABP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LVSDownloader.h"

@interface ApplicationController : NSObject {
    IBOutlet NSWindow *window;
	IBOutlet NSComboBox *serverComboBox;
	IBOutlet NSPopUpButton *languagePopUpButton;
	IBOutlet NSPopUpButton *memoryPopUpButton;
	IBOutlet LVSDownloader *downloader;
	IBOutlet NSButton *debugCheckbox;
	NSMutableArray *serverList;
    IBOutlet NSTextField *progressLabel;
    IBOutlet NSTextField *formLabel;
    IBOutlet NSProgressIndicator *progressBar;
    IBOutlet NSTextField *actionLabel;
    IBOutlet NSButtonCell *advancedOptionsDisclosure;
    IBOutlet NSBox *advancedOptionsBox;
    IBOutlet NSMenuItem *clearCacheMenuItem;
    IBOutlet NSTextField *classNameToRun;
    IBOutlet NSBox *launchButton;
    IBOutlet NSPopUpButton *applicationChoice;
    IBOutlet NSPopUpButton *widgetChoice;
}

-(IBAction)clickLaunch:(id)sender;
-(IBAction)clickClearCache:(id)sender;
-(IBAction)clickAdvancedDisclosure:(id)sender;

@end
