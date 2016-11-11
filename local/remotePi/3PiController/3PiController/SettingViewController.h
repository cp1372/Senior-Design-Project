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
@interface SettingViewController: UIViewController <UINavigationControllerDelegate>{
    
}


@property(nonatomic, strong) IBOutlet UIButton *btSave;
@property(nonatomic, strong) IBOutlet UIButton *btClear;
@property(nonatomic, strong) IBOutlet UIButton *btDefault;
@property(nonatomic, strong) IBOutlet UITextField *ip;
@property(nonatomic, strong) IBOutlet UITextField *port;
@property(nonatomic, strong) IBOutlet UISlider *slider;
@property(nonatomic, strong) IBOutlet UILabel *speed;
@property(nonatomic, strong) IBOutlet UISwitch *motionMode;
@property(nonatomic, strong) IBOutlet UILabel *motion;

- (IBAction)View_TouchDown:(id)sender;

- (IBAction)doSave:(id)sender;
- (IBAction)doClear:(id)sender;
- (IBAction)doDefault:(id)sender;

@end