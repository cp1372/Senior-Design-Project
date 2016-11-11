//
//  MainConsoleController.m
//  3PiController
//
//  Created by Yan & Colten on 15/2/23.
//  Copyright (c) 2015 3piControllerIOS. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SettingViewController.h" 

@class ViewController;

@implementation SettingViewController

- (void) viewDidLoad{
    [super viewDidLoad];
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.ipAddress"] != nil) {
        [self.ip setText:[[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.ipAddress"]];
    }
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.portNumber"] != nil) {
        [self.port setText:[[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.portNumber"]];
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

// Release any retained subviews of the main view.
- (void)viewDidUnload {
    [super viewDidUnload];
}


- (IBAction)View_TouchDown:(id)sender {
    //resignFirstResponder. hide the keyboard when touch down
    [[UIApplication sharedApplication] sendAction:@selector(resignFirstResponder) to:nil from:nil forEvent:nil];
}

-(IBAction)doSave:(id)sender {
    //save the user input host name and port number
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[self.ip text] forKey:@"3pi.ipAddress"];
    [defaults setObject:[self.port text] forKey:@"3pi.portNumber"];
    [defaults synchronize];
}

-(IBAction)doClear:(id)sender {

    //@Action todo
}

-(IBAction)doDefault:(id)sender{

    //@Action todo
}

@end

