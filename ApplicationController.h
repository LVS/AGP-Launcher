//
//  ApplicationController.h
//  AGP Launcher
//
//  Created by Andy on 12/02/2010.
//  Copyright 2010 NextGen Development Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <RegexKit/RegexKit.h>
#import "LVSDownloader.h"

@interface ApplicationController : NSObject {
	IBOutlet NSComboBox *serverComboBox;
	IBOutlet NSPopUpButton *languagePopUpButton;
	IBOutlet LVSDownloader *downloader;
	IBOutlet NSButton *debugCheckbox;
	NSMutableArray *serverList;
}

-(IBAction)clickLaunch:(id)sender;

@end
