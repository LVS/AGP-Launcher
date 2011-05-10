//
//  LVSDownloader.h
//  ABP Launcher
//
//  Created by Andy Jeffries (andy@andyjeffries.co.uk) on 17/02/2010.
//  Copyright 2010 LVS Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RegexKitLite.h"

@interface LVSDownloader : NSObject {
	IBOutlet NSButton *launchButton;
	IBOutlet NSProgressIndicator *progressBar;
    IBOutlet NSTextField *actionLabel;
	
	@private
	NSMutableString *class_path;
	NSMutableArray *cmd_args;
}
-(void)pullFromServer:(NSString *)server_address language:(NSString *)language memory:(NSString *)memory debug:(BOOL)debug jnlpTag:(NSInteger)jnlp_tag className:(NSString *)className;

@end
