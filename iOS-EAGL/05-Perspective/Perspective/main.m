//
//  main.m
//  window
//
//  Created by Rohit Kulkarni on 15/02/22.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    
    appDelegateClassName = NSStringFromClass([AppDelegate class]);
    
    int ret = UIApplicationMain(argc, argv, nil, appDelegateClassName);
    
    [pool release];
    
    return (ret);
}
