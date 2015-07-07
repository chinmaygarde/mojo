// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'box.dart';
import 'object.dart';
import 'package:cassowary/cassowary.dart' as AL;

class AutoLayoutParentData extends BoxParentData
    with ContainerParentDataMixin<RenderBox> {

  final AL.Param leftEdge;
  final AL.Param rightEdge;
  final AL.Param topEdge;
  final AL.Param bottomEdge;

  AL.Expression get width => rightEdge - leftEdge;
  AL.Expression get height => bottomEdge - topEdge;

  AL.Expression get horizontalCenter => (leftEdge + rightEdge) / AL.CM(2.0);
  AL.Expression get verticalCenter => (topEdge + bottomEdge) / AL.CM(2.0);

  final RenderAutoLayout _renderLayout;
  final RenderBox _renderBox;

  AutoLayoutParentData(this._renderLayout, this._renderBox)
      : leftEdge = new AL.Param(),
        rightEdge = new AL.Param(),
        topEdge = new AL.Param(),
        bottomEdge = new AL.Param() {
    leftEdge.context = this;
    rightEdge.context = this;
    topEdge.context = this;
    bottomEdge.context = this;
  }

  AL.Result addConstraints(List<AL.Constraint> constraints) =>
      _renderLayout.addConstraints(constraints);

  AL.Result addConstraint(AL.Constraint constraint) =>
      _renderLayout.addConstraint(constraint);

  void _applyParameterUpdates() {
    BoxConstraints box = new BoxConstraints.tightFor(
        width: rightEdge.value - leftEdge.value,
        height: bottomEdge.value - topEdge.value);

    _renderBox.layout(box, parentUsesSize: false);
    this.position = new Point(leftEdge.value, topEdge.value);
  }
}

class RenderAutoLayout extends RenderBox
    with ContainerRenderObjectMixin<RenderBox, AutoLayoutParentData>,
         RenderBoxContainerDefaultsMixin<RenderBox,
         AutoLayoutParentData> {

  final AL.Solver _solver = new AL.Solver();

  RenderAutoLayout({List<RenderBox> children}) {
    addAll(children);
  }

  /// Adds all the given constraints to the solver. Either all constraints are
  /// added or none
  AL.Result addConstraints(List<AL.Constraint> constraints) {
    AL.Result res = _solver.addConstraints(constraints);

    if (res == AL.Result.success) {
      markNeedsLayout();
    }

    return res;
  }

  /// Add the given constraint to the solver.
  AL.Result addConstraint(AL.Constraint constraint) {
    AL.Result res = _solver.addConstraint(constraint);

    if (res == AL.Result.success) {
      markNeedsLayout();
    }

    return res;
  }

  @override
  void setupParentData(RenderObject child) {
    if (child.parentData is! AutoLayoutParentData) {
      child.parentData = new AutoLayoutParentData(this, child);
    }
  }

  @override
  void performLayout() {
    // Step 1: Resolve solver updates and flush parameters

    // We don't iterate over the children, instead, we ask the solver to tell
    // us the updated parameters. Attached to the parameters (via the context)
    // are the AutoLayoutParentData instances. We can fetch the updated children
    // from the same
    Set<AutoLayoutParentData> updates = _solver.flushUpdates();

    for (AutoLayoutParentData childData in updates) {
      // Assert that parameters active in other solvers are not being resolved
      // here
      assert(childData._renderLayout == this);
      childData._applyParameterUpdates();
    }

    // Step 2: Update dimensions of self
    size = constraints.smallest;
  }

  @override
  void hitTestChildren(HitTestResult result, {Point position}) =>
      defaultHitTestChildren(result, position: position);

  @override
  void paint(PaintingCanvas canvas, Offset offset) =>
      defaultPaint(canvas, offset);
}
