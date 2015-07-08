// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'box.dart';
import 'object.dart';
import 'package:cassowary/cassowary.dart' as AL;

class AutoLayoutParentData extends BoxParentData
    with ContainerParentDataMixin<RenderBox>, _AutoLayoutParamMixin {

  final RenderBox _renderBox;

  AutoLayoutParentData(this._renderBox) {
    _setupLayoutParameters(this);
  }

  @override
  void _applyAutolayoutParameterUpdates() {
    BoxConstraints box = new BoxConstraints.tightFor(
        width: _rightEdge.value - _leftEdge.value,
        height: _bottomEdge.value - _topEdge.value);

    _renderBox.layout(box, parentUsesSize: false);
    this.position = new Point(_leftEdge.value, _topEdge.value);
  }
}

class RenderAutoLayout extends RenderBox
    with ContainerRenderObjectMixin<RenderBox, AutoLayoutParentData>,
         RenderBoxContainerDefaultsMixin<RenderBox, AutoLayoutParentData>,
         _AutoLayoutParamMixin {

  final AL.Solver _solver = new AL.Solver();

  RenderAutoLayout({List<RenderBox> children}) {
    _setupLayoutParameters(this);
    _setupEditVariablesInSolver(_solver, AL.Priority.required - 1);

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
      child.parentData = new AutoLayoutParentData(child);
    }
  }

  @override
  void set size(Size value) {
    super.size = value;
    _applyEditsAtSize(_solver, value);
  }

  @override
  void performLayout() {
    // Step 1: Update dimensions of self
    size = constraints.smallest;

    // Step 2: Resolve solver updates and flush parameters

    // We don't iterate over the children, instead, we ask the solver to tell
    // us the updated parameters. Attached to the parameters (via the context)
    // are the AutoLayoutParentData instances. We can fetch the updated children
    // from the same
    Set<_AutoLayoutParamMixin> updates = _solver.flushUpdates();
    for (_AutoLayoutParamMixin update in updates) {
      update._applyAutolayoutParameterUpdates();
    }
  }

  @override
  void _applyAutolayoutParameterUpdates() {
    // Nothing to do since the size update has already been presented to the
    // solver are edit variable modification.
  }

  @override
  void hitTestChildren(HitTestResult result, {Point position}) =>
      defaultHitTestChildren(result, position: position);

  @override
  void paint(PaintingCanvas canvas, Offset offset) =>
      defaultPaint(canvas, offset);
}

abstract class _AutoLayoutParamMixin {
  // Ideally, the edges would all be final, but then they would have to be
  // initialized before the constructor. Not sure how to do that using a Mixin
  AL.Param _leftEdge;
  AL.Param _rightEdge;
  AL.Param _topEdge;
  AL.Param _bottomEdge;

  void _setupLayoutParameters(dynamic context) {
    _leftEdge = new AL.Param.withContext(context);
    _rightEdge = new AL.Param.withContext(context);
    _topEdge = new AL.Param.withContext(context);
    _bottomEdge = new AL.Param.withContext(context);
  }

  void _setupEditVariablesInSolver(AL.Solver solver, double priority) {
    solver.addEditVariable(_leftEdge.variable, priority);
    solver.addEditVariable(_rightEdge.variable, priority);
    solver.addEditVariable(_topEdge.variable, priority);
    solver.addEditVariable(_bottomEdge.variable, priority);
  }

  void _applyEditsAtSize(AL.Solver solver, Size size) {
    solver.suggestValueForVariable(_leftEdge.variable, 0.0);
    solver.suggestValueForVariable(_topEdge.variable, 0.0);
    solver.suggestValueForVariable(_bottomEdge.variable, size.height);
    solver.suggestValueForVariable(_rightEdge.variable, size.width);
  }

  void _applyAutolayoutParameterUpdates();

  AL.Param get leftEdge => _leftEdge;
  AL.Param get rightEdge => _rightEdge;
  AL.Param get topEdge => _topEdge;
  AL.Param get bottomEdge => _bottomEdge;

  AL.Expression get width => _rightEdge - _leftEdge;
  AL.Expression get height => _bottomEdge - _topEdge;

  AL.Expression get horizontalCenter => (_leftEdge + _rightEdge) / AL.CM(2.0);
  AL.Expression get verticalCenter => (_topEdge + _bottomEdge) / AL.CM(2.0);
}
