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
#import "AppDelegates.h"

@interface MainConsoleController: UIViewController <NSStreamDelegate>{
}

@property(nonatomic, strong) IBOutlet UIButton *doUp;
@property(nonatomic, strong) IBOutlet UIButton *doDown;
@property(nonatomic, strong) IBOutlet UIButton *doLeft;
@property(nonatomic, strong) IBOutlet UIButton *doRight;
@property(nonatomic, strong) IBOutlet UIButton *doStop;
@property(nonatomic, strong) IBOutlet UIImageView *image;
-(IBAction)goUp:(id)sender;
-(IBAction)goDown:(id)sender;
-(IBAction)goLeft:(id)sender;
-(IBAction)goRight:(id)sender;
-(IBAction)stop:(id)sender;

@end