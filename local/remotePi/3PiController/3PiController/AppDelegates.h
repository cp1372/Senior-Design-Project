//
//  MainConsoleController.m
//  3PiController
//
//  Created by Yan & Colten on 15/2/23.
//  Copyright (c) 2015 3piControllerIOS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ViewController.h"
#import "MainConsoleController.h"

@interface AppDelegates : NSObject <NSStreamDelegate, UIApplicationDelegate> {
    UIWindow *window;
    UINavigationController *navigationController;
    NSInputStream *inputStream;
    NSOutputStream *outputStream;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) UINavigationController *navigationController;
@property (nonatomic, retain) NSInputStream *inputStream;
@property (nonatomic, retain) NSOutputStream *outputStream;

@end

