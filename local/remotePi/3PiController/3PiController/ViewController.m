//
//  MainConsoleController.m
//  3PiController
//
//  Created by Yan & Colten on 15/2/23.
//  Copyright (c) 2015 3piControllerIOS. All rights reserved.
//


#import "ViewController.h"
#import "SettingViewController.h"
#import "MainConsoleController.h"


@implementation ViewController

@synthesize hostName;
@synthesize portNumber;

static AppDelegates *appDelegate;

// Do any additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"3Pi Robot Controller";
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithTitle:@"Main Page" style:UIBarButtonItemStylePlain target:nil action:nil];
    self.navigationItem.backBarButtonItem = backButton;
    [self.connStatus setText:@"Not Connected"];
    appDelegate = (AppDelegates *) [[UIApplication sharedApplication] delegate];
}

- (void) viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    //load the ip address and port number which is saved by user
    
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.ipAddress"] != nil) {
        hostName = [[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.ipAddress"];
       
        //NSLog(@"instance assigned");
    }
    if ([[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.portNumber"] != nil) {
        portNumber = [[NSUserDefaults standardUserDefaults] objectForKey:@"3pi.portNumber"];
    }
}

//release any cached data, release the view
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void) viewDidUnload{
    [super viewDidUnload];
}

- (IBAction)doSetting:(id)sender{
    SettingViewController *settingViewController = [[SettingViewController alloc] initWithNibName:@"SettingView" bundle:nil ];
    settingViewController.title = @"3Pi Robot Settings";
    [self.navigationController pushViewController:settingViewController animated:YES];
}

- (IBAction)doConnect:(id)sender {
    [self initializeNetworkComm];
    [self.connStatus setText:@"Connecting...."];
    NSString *response  = [NSString stringWithFormat:@"conndone"];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [appDelegate.outputStream write:[data bytes] maxLength:[data length]];
    NSLog(@"%@, %@", hostName, portNumber);
    
    while (([appDelegate.outputStream streamStatus] != NSStreamStatusOpen && [appDelegate.outputStream streamStatus] != NSStreamStatusError)) {
        [self.connStatus performSelectorOnMainThread:@selector(setText:) withObject:@"Connection in progress…" waitUntilDone:YES];
    }
    if ([appDelegate.outputStream streamStatus] == NSStreamStatusOpen) {
        [self.connStatus performSelectorOnMainThread:@selector(setText:) withObject:[NSString stringWithFormat:@"Connected to %@:%@", hostName, portNumber] waitUntilDone:YES];
    } else if ([appDelegate.outputStream streamStatus] == NSStreamStatusError) {
        [self.connStatus performSelectorOnMainThread:@selector(setText:) withObject:@"Could not connect to 3pi" waitUntilDone:YES];
    } else {
        [self.connStatus performSelectorOnMainThread:@selector(setText:) withObject:@"Not connected to 3pi" waitUntilDone:YES];
    }
}

- (IBAction)doDisconnect: (id)sender{
    [appDelegate.inputStream close];
    [appDelegate.outputStream close];
    if ([appDelegate.outputStream streamStatus] == NSStreamStatusClosed) {
        [self.connStatus performSelectorOnMainThread:@selector(setText:) withObject:[NSString stringWithFormat:@"Disconnected"] waitUntilDone:YES];
    }
}

- (IBAction)linktoMainConsole:(id)sender{
    MainConsoleController *mainConsoleController = [[MainConsoleController alloc] initWithNibName:@"mainConsole" bundle:nil];
    mainConsoleController.title = @"Robot & Rasp Pi Main Console";
    [self.navigationController pushViewController:mainConsoleController animated:YES];
}


- (void)initializeNetworkComm {
    CFReadStreamRef readS;
    CFWriteStreamRef writeS;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef) CFBridgingRetain(hostName), [portNumber intValue], &readS, &writeS);
    appDelegate.inputStream = (__bridge NSInputStream *) readS;
    appDelegate.outputStream = (__bridge NSOutputStream *) writeS;
    [appDelegate.inputStream setDelegate:self];
    [appDelegate.outputStream setDelegate:self];
    [appDelegate.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [appDelegate.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [appDelegate.inputStream open];
    [appDelegate.outputStream open];
}

@end
