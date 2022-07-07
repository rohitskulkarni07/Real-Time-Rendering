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
    //code
    self = [super initWithFrame:frame];
    
    if(self) {
        centralText = @"Hello World!";
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


- (void)dealloc {
    [super dealloc];
}

@end
