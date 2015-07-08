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

  @override
  List<AL.Constraint> _constructImplicitConstraints() {
    return [
      // The left edge must be positive
      _leftEdge >= AL.CM(0.0),

      // Width must be positive
      _rightEdge >= _leftEdge,
    ];
  }
}

class RenderAutoLayout extends RenderBox
    with ContainerRenderObjectMixin<RenderBox, AutoLayoutParentData>,
         RenderBoxContainerDefaultsMixin<RenderBox, AutoLayoutParentData>,
         _AutoLayoutParamMixin {

  final AL.Solver _solver = new AL.Solver();
  List<AL.Constraint> _explicitConstraints = new List<AL.Constraint>();

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
      _explicitConstraints.addAll(constraints);
    }

    return res;
  }

  /// Add the given constraint to the solver.
  AL.Result addConstraint(AL.Constraint constraint) {
    AL.Result res = _solver.addConstraint(constraint);

    if (res == AL.Result.success) {
      markNeedsLayout();
      _explicitConstraints.add(constraint);
    }

    return res;
  }

  /// Removes all explicitly added constraints.
  AL.Result clearAllConstraints() {
    AL.Result res = _solver.removeConstraints(_explicitConstraints);

    if (res == AL.Result.success) {
      markNeedsLayout();
      _explicitConstraints = new List<AL.Constraint>();
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
    // are the _AutoLayoutParamMixin instances.
    for (_AutoLayoutParamMixin update in _solver.flushUpdates()) {
      update._applyAutolayoutParameterUpdates();
    }
  }

  @override
  void _applyAutolayoutParameterUpdates() {
    // Nothing to do since the size update has already been presented to the
    // solver as an edit variable modification. The invokation of this method
    // only indicates that the value has been flushed to the variable.
  }

  @override
  void hitTestChildren(HitTestResult result, {Point position}) =>
      defaultHitTestChildren(result, position: position);

  @override
  void paint(PaintingCanvas canvas, Offset offset) =>
      defaultPaint(canvas, offset);

  @override
  void adoptChild(RenderObject child) {
    // Make sure to call super first to setup the parent data
    super.adoptChild(child);
    child.parentData._setupImplicitConstraints(_solver);
  }

  @override
  void dropChild(RenderObject child) {
    child.parentData._collectImplicitConstraints(_solver);

    // Call super last as this collects parent data
    super.dropChild(child);
  }

  @override
  List<AL.Constraint> _constructImplicitConstraints() {
    // Only edits are present on layout containers
    return null;
  }
}

abstract class _AutoLayoutParamMixin {
  // Ideally, the edges would all be final, but then they would have to be
  // initialized before the constructor. Not sure how to do that using a Mixin
  AL.Param _leftEdge;
  AL.Param _rightEdge;
  AL.Param _topEdge;
  AL.Param _bottomEdge;

  List<AL.Constraint> _implicitConstraints;

  AL.Param get leftEdge => _leftEdge;
  AL.Param get rightEdge => _rightEdge;
  AL.Param get topEdge => _topEdge;
  AL.Param get bottomEdge => _bottomEdge;

  AL.Expression get width => _rightEdge - _leftEdge;
  AL.Expression get height => _bottomEdge - _topEdge;

  AL.Expression get horizontalCenter => (_leftEdge + _rightEdge) / AL.CM(2.0);
  AL.Expression get verticalCenter => (_topEdge + _bottomEdge) / AL.CM(2.0);

  void _setupLayoutParameters(dynamic context) {
    _leftEdge = new AL.Param.withContext(context);
    _rightEdge = new AL.Param.withContext(context);
    _topEdge = new AL.Param.withContext(context);
    _bottomEdge = new AL.Param.withContext(context);
  }

  void _setupEditVariablesInSolver(AL.Solver solver, double priority) {
    solver.addEditVariables([
        _leftEdge.variable,
        _rightEdge.variable,
        _topEdge.variable,
        _bottomEdge.variable], priority);
  }

  void _applyEditsAtSize(AL.Solver solver, Size size) {
    solver.suggestValueForVariable(_leftEdge.variable, 0.0);
    solver.suggestValueForVariable(_topEdge.variable, 0.0);
    solver.suggestValueForVariable(_bottomEdge.variable, size.height);
    solver.suggestValueForVariable(_rightEdge.variable, size.width);
  }

  void _applyAutolayoutParameterUpdates();
  List<AL.Constraint> _constructImplicitConstraints();

  void _setupImplicitConstraints(AL.Solver solver) {
    List<AL.Constraint> implicit = _constructImplicitConstraints();

    if (implicit == null || implicit.length == 0) {
      return;
    }

    AL.Result res = solver.addConstraints(implicit);
    assert(res == AL.Result.success);

    _implicitConstraints = implicit;
  }

  void _collectImplicitConstraints(AL.Solver solver) {
    if (_implicitConstraints == null || _implicitConstraints.length == 0) {
      return;
    }

    AL.Result res = solver.removeConstraints(_implicitConstraints);
    assert(res == AL.Result.success);

    _implicitConstraints = null;
  }
}
