// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'animated_value.dart';
import 'curves.dart';

// TODO(mpcomplete): merge this stuff with AnimatedValue somehow. We shouldn't
// have 2 different ways to animate values.
abstract class AnimatedVariable {
  void setFraction(double t);
}

class AnimatedType<T extends dynamic> extends AnimatedVariable {
  T value;
  final T begin, end;
  final Curve curve;

  AnimatedType(this.begin, this.end, {this.curve: linear}) {
    value = begin;
  }

  void setFraction(double t) {
    // TODO(mpcomplete): Reverse the timeline and curve.
    value = begin + (end - begin) * curve.transform(t);
  }
}

// This class manages a "performance" - a collection of values that change
// based on a timeline. For example, a performance may handle an animation
// of a menu opening by sliding and fading in (changing Y value and opacity)
// over .5 seconds. The performance can move forwards (present) or backwards
// (dismiss). A consumer may also take direct control of the timeline by
// manipulating |progress|, or |fling| the timeline causing a physics-based
// simulation to take over the progression.
class AnimationPerformance {
  // TODO(mpcomplete): make this a list, or composable somehow.
  AnimatedVariable variable;
  // Advances from 0 to 1. On each tick, we'll update our variable's values.
  AnimatedValue timeline = new AnimatedValue(0.0);
  // TODO(mpcomplete): duration should be on a director.
  Duration duration;

  AnimationPerformance() {
    timeline.onValueChanged.listen((double t) {
      variable.setFraction(t);
    });
  }

  double get progress => timeline.value;
  void set progress(double t) {
    stop();
    timeline.value = t.clamp(0.0, 1.0);
  }

  bool get isDismissed => progress == 0.0;
  bool get isCompleted => progress == 1.0;
  bool get isAnimating => timeline.isAnimating;

  void play() {
    _animateTo(1.0);
  }
  void reverse() {
    _animateTo(0.0);
  }

  void _animateTo(double target) {
    double remainingDistance = (target - timeline.value).abs();
    timeline.stop();
    if (remainingDistance != 0.0)
      timeline.animateTo(target, remainingDistance * duration.inMilliseconds);
  }

  void stop() {
    timeline.stop();
  }

  // Resume animating in a direction, with the given velocity.
  // TODO(mpcomplete): this should be a force with friction so it slows over
  // time.
  void fling({double velocity: 1.0}) {
    double target = velocity.sign < 0.0 ? 0.0 : 1.0;
    double distance = (target - timeline.value).abs();
    double duration = distance / velocity.abs();

    if (distance > 0.0)
      timeline.animateTo(target, duration, curve: linear);
  }
}
