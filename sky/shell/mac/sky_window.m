// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "sky_window.h"

@interface SkyWindow () <NSWindowDelegate>

@property (assign) IBOutlet NSOpenGLView *renderSurface;
@property (getter=isSurfaceSetup) BOOL surfaceSetup;

@end

@implementation SkyWindow

@synthesize renderSurface=_renderSurface;
@synthesize surfaceSetup=_surfaceSetup;

-(void) awakeFromNib {
  [super awakeFromNib];

  self.delegate = self;

  [self windowDidResize:nil];
}

-(void) windowDidResize:(NSNotification *)notification {
  [self setupSurfaceIfNecessary];

  // Resize
}

-(void) setupSurfaceIfNecessary {
  if (self.isSurfaceSetup) {
    return;
  }

  self.surfaceSetup = YES;

  // Setup
}

#pragma mark - Responder overrides

- (void)dispatchEvent:(NSEvent *)event phase:(NSEventPhase) phase {
  NSPoint location = [_renderSurface convertPoint:event.locationInWindow
                                         fromView:nil];

  location.y = _renderSurface.frame.size.height - location.y;
}

- (void)mouseDown:(NSEvent *)event {
  [self dispatchEvent:event phase:NSEventPhaseBegan];
}

- (void)mouseDragged:(NSEvent *)event {
  [self dispatchEvent:event phase:NSEventPhaseChanged];
}

- (void)mouseUp:(NSEvent *)event {
  [self dispatchEvent:event phase:NSEventPhaseEnded];
}

@end
