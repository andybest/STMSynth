//
//  AppDelegate.m
//  TestTool
//
//  Created by Andy Best on 23/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#import "AppDelegate.h"

void setHeadphoneVolume(int vol)
{
    
}

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    self.audioManager = [[ABAudioManager alloc] init];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

@end
