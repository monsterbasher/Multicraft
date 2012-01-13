//
//  main.m
//  IOSSupport
//
//  Created by MacBook on 1/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "AppDelegate.h"

int main(int argc, char *argv[])
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"AppDelegate");
    [pool release];
    return retVal;
    
    //{
    //    return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    //}
}
