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
}

-(IBAction)clickLaunch:(id)sender;
- (IBAction)clickClearCache:(id)sender;

@end
