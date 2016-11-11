//
//  MainConsoleController.m
//  3PiController
//
//  Created by Yan & Colten on 15/2/23.
//  Copyright (c) 2015 3piControllerIOS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MainConsoleController.h"


@implementation MainConsoleController

static AppDelegates *appDelegate;

- (void)viewDidLoad {
    [super viewDidLoad];
     appDelegate = (AppDelegates *) [[UIApplication sharedApplication] delegate];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;

}


- (void) viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
}


-(IBAction)goUp:(id)sender{
    [self sendCommand:@"forward"];
}

-(IBAction)goDown:(id)sender{
    [self sendCommand:@"back"];
}

-(IBAction)goLeft:(id)sender{
    [self sendCommand:@"left"];
}

-(IBAction)goRight:(id)sender{
    [self sendCommand:@"right"];
}

-(IBAction)stop:(id)sender{
    [self sendCommand:@"stop"];
}

- (void) sendCommand: (NSString *) command{
    NSData *data = [[NSData alloc] initWithData:[command dataUsingEncoding:NSASCIIStringEncoding]];
    [appDelegate.outputStream write:[data bytes] maxLength:[data length]];
    
}
@end

