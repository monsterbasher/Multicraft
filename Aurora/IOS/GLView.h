#import <QuartzCore/QuartzCore.h>

@interface GLView : UIView
{
@private
    EAGLContext* m_context;
    float m_timestamp;
}

- (void) drawView: (CADisplayLink*) displayLink;
- (void) didRotate: (NSNotification*) notification;

@end
