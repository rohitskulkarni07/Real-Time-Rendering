//
//  MyView.m
//  window
//
//  Created by Rohit Kulkarni on 15/02/22.
//

#import "MyView.h"

@implementation MyView {
    @private
    NSString *centralText;
}

- (id)initWithFrame:(CGRect)frame {
    
    // code
    self = [super initWithFrame:frame];
    
    if(self) {
        
        centralText = @"Hello World!";
        
        // single tap
        UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onSingleTap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        // double tap
        UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        // fail double tap if single tap
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(onSwipe:)];
        [swipeGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(onLongPress:)];
        [longPressGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    
    return (self);
}

- (void)drawRect:(CGRect)rect {
    
    // Drawing code
    UIColor *backgroundColor = [UIColor blackColor];
    [backgroundColor set];
    UIRectFill(rect);
    
    NSDictionary* dictionaryWithTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys:[UIFont fontWithName:@"Helvetica" size:32],
                                                  NSFontAttributeName,[UIColor greenColor], NSForegroundColorAttributeName, nil];
    CGSize textSize = [centralText sizeWithAttributes:dictionaryWithTextAttributes];
    CGPoint point;
    point.x = (rect.size.width/2) - (textSize.width/2);
    point.y = (rect.size.height/2) - (textSize.height/2);
    
    [centralText drawAtPoint:point withAttributes:dictionaryWithTextAttributes];
}

- (void)onSingleTap:(UITapGestureRecognizer *)gr {
    
    // code
    centralText = @"Single Tap";
    [self setNeedsDisplay];
}


- (void)onDoubleTap:(UITapGestureRecognizer *)gr {
    
    // code
    centralText = @"Double Tap";
    [self setNeedsDisplay];
}

- (void)onSwipe:(UISwipeGestureRecognizer *)gr {
    
    // code
    centralText = @"Swipe";
    [self setNeedsDisplay];
}

- (void)onLongPress:(UILongPressGestureRecognizer *)gr {
    
    // code
    centralText = @"Long Press";
    [self setNeedsDisplay];
    
}

- (void)dealloc {
    // code
    [super dealloc];
}

@end
