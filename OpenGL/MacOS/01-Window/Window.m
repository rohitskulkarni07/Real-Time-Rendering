//
//  Window.m
//  
//
//  Created by Rohit Kulkarni on 04/01/22.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface AppDelegate: NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

int main(int argc, char* argv[]) {
    
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSApp = [NSApplication sharedApplication];
    [NSApp setDelegate: [[AppDelegate alloc] init]];
    
    // run loop
    [NSApp run];
    
    [pool release];
    
    return 0;
}

@interface MyView: NSView
@end

@implementation AppDelegate {
    @private
    NSWindow *window;
    MyView *view;
}

-(void) applicationDidFinishLaunching: (NSNotification *)aNotification {
    
    // code
    NSRect win_rect = NSMakeRect(0.0, 0.0, 800, 600); // internally c library function (Core Graphics)
    window = [
        [NSWindow alloc] initWithContentRect: win_rect
                                         styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                           backing: NSBackingStoreBuffered
                                            defer: NO
    ];
    [window setTitle: @"RSK: macOS Window"];
    [window center];
    
    view = [[MyView alloc] initWithFrame: win_rect];
    [window setContentView: view];
    [window setDelegate: self];
    [window makeKeyAndOrderFront: self];
}

-(void) applicationWillTerminate: (NSNotification *)aNotification {
    
    // code
    
}

-(void) windowWillClose: (NSNotification *)aNotification {
    
    // code
    [NSApp terminate: self];
    
}

-(void) dealloc {
    
    // code
    [view release];
    [window release];
    [super dealloc];
}
@end

@implementation MyView {
    
    @private
    NSString *centralText;
}

-(id) initWithFrame: (NSRect)frame {
    
    // code
    self = [super initWithFrame: frame];
    
    if(self) {
        //
        centralText = @"Hello World";
    }
    
    return self;
}

-(void) drawRect: (NSRect)dirtyRect {
    
    // code
    NSColor *backgroundColor = [NSColor blackColor];
    [backgroundColor set];
    NSRectFill(dirtyRect);
    
    NSDictionary *dictionaryForTextAttribute = [NSDictionary dictionaryWithObjectsAndKeys:
                                              [NSFont fontWithName: @"Helvetica" size: 32], NSFontAttributeName,
                                              [NSColor greenColor], NSForegroundColorAttributeName, nil];
    NSSize textSize = [centralText sizeWithAttributes: dictionaryForTextAttribute];
    NSPoint point;
    
    point.x = (dirtyRect.size.width/2 - textSize.width/2);
    point.y = (dirtyRect.size.height/2 - textSize.height/2);
    
    [centralText drawAtPoint: point withAttributes: dictionaryForTextAttribute];
    
}

-(BOOL) acceptsFirstResponder{
    
    // code
    [[self window] makeFirstResponder: self];
    return YES;
}

-(void) keyDown: (NSEvent*) theEvent {
    // code
    int key = [[theEvent characters] characterAtIndex: 0];
    
    switch(key) {
        case 27:
            [self release];
            [NSApp terminate: self];
            break;
            
        case 'f':
        case 'F':
            [[self window] toggleFullScreen: self];
            break;
        default:
            centralText = @"Left Mouse Button Clicked!";
            [self setNeedsDisplay: YES];
            break;
    }
}

-(void) mouseDown: (NSEvent*) theEvent {
    // code
    centralText = @"Left Mouse Button Clicked!";
    [self setNeedsDisplay: YES];
    
}

-(void) rightMouseDown: (NSEvent*) theEvent {
    // code
    centralText = @"Right Mouse Button Clicked!";
    [self setNeedsDisplay: YES];
}

-(void) otherMouseDown: (NSEvent*) theEvent {
    // code
    centralText = @"Hello World!";
    [self setNeedsDisplay: YES];
}

-(void)dealloc{
    [super dealloc];
}
@end
