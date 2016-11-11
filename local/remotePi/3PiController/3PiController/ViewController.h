//
//  MainConsoleController.m
//  3PiController
//
//  Created by Yan & Colten on 15/2/23.
//  Copyright (c) 2015 3piControllerIOS. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
//useful for the motion control robot
#import <CoreMotion/CoreMotion.h>
#import "AppDelegates.h"

@interface ViewController : UIViewController <NSStreamDelegate> {
    NSString *hostName;
    NSString *portNumber;

}

@property(nonatomic, strong) IBOutlet UIButton *buttonConnect;
@property(nonatomic, strong) IBOutlet UIButton *buttonMainConsole;
@property(nonatomic, strong) IBOutlet UIButton *buttonSetting;
@property(nonatomic, strong) IBOutlet UILabel *connStatus;
@property(nonatomic, strong) NSString *hostName;
@property(nonatomic, strong) NSString *portNumber;

- (IBAction)doSetting:(id)sender;
- (IBAction)doConnect:(id)sender;
- (IBAction)linktoMainConsole:(id)sender;
- (IBAction)doDisconnect: (id)sender;

@end

