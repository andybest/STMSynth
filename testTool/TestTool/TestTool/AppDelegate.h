//
//  AppDelegate.h
//  TestTool
//
//  Created by Andy Best on 23/02/2015.
//  Copyright (c) 2015 Andy Best. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ABAudioManager.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>


@property (nonatomic, strong) ABAudioManager *audioManager;

@end

