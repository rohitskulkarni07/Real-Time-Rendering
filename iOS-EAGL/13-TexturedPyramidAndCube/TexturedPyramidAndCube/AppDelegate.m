//
//  AppDelegate.m
//  window
//
//  Created by Rohit Kulkarni on 15/02/22.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import "GLESView.h"

@implementation AppDelegate {
    @private
    UIWindow* window;
    ViewController* viewController;
    GLESView* view;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    // Override point for customization after application launch.
    CGRect win_rect = [[UIScreen mainScreen]bounds];
    
    window = [[UIWindow alloc]initWithFrame:win_rect];
    
    viewController = [[ViewController alloc]init];
    
    [window setRootViewController:viewController];
    
    view = [[GLESView alloc]initWithFrame:win_rect];
    
    [viewController setView:view];
    
    [view release];
       
    [window makeKeyAndVisible];
    
    [view startAnimation];
       
    return YES;
}

- (void) applicationDidEnterBackground:(UIApplication *)application {
    // code
}

- (void) applicationWillEnterForeground:(UIApplication *)application {
    // code
}

- (void) applicationWillResignActive:(UIApplication *)application {
    
    // code
    [view stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application {
    
    // code
    [view startAnimation];
}

- (void) applicationWillTerminate:(UIApplication *)application {
    
    // code
    [view stopAnimation];
}

- (void) dealloc {
    
    // code
    [view release];
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end
