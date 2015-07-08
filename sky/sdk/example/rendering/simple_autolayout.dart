// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import 'dart:sky';
import 'package:sky/rendering/box.dart';
import 'package:sky/rendering/object.dart';
import 'package:sky/rendering/sky_binding.dart';
import 'package:sky/rendering/auto_layout.dart';
import 'package:cassowary/cassowary.dart' as AL;

void main() {
  var c1 = new RenderDecoratedBox(
    decoration: new BoxDecoration(backgroundColor: const Color(0xFFFF0000))
  );

  var c2 = new RenderDecoratedBox(
    decoration: new BoxDecoration(backgroundColor: const Color(0xFF00FF00))
  );

  var c3 = new RenderDecoratedBox(
    decoration: new BoxDecoration(backgroundColor: const Color(0xFF0000FF))
  );

  var c4 = new RenderDecoratedBox(
    decoration: new BoxDecoration(backgroundColor: const Color(0xFFFFFFFF))
  );

  var root = new RenderAutoLayout(children: [c1, c2, c3, c4]);

  AutoLayoutParentData p1 = c1.parentData;
  AutoLayoutParentData p2 = c2.parentData;
  AutoLayoutParentData p3 = c3.parentData;
  AutoLayoutParentData p4 = c4.parentData;

  root.addConstraints(<AL.Constraint>[
    // Sum of widths of each box must be equal to that of the container
    (p1.width + p2.width + p3.width == root.width) as AL.Constraint,

    // The boxes must be stacked left to right
    p1.rightEdge <= p2.leftEdge,
    p2.rightEdge <= p3.leftEdge,

    // The widths of the first and the third boxes should be equal
    (p1.width == p3.width) as AL.Constraint,

    // The width of the second box should be twice as much as that of the first
    // and third
    (p2.width * AL.CM(2.0) == p1.width) as AL.Constraint,

    // The height of the three boxes should be equal to that of the container
    (p1.height == p2.height) as AL.Constraint,
    (p2.height == p3.height) as AL.Constraint,
    (p3.height == root.height) as AL.Constraint,

    // The fourth box should be half as wide as the second and must be attached
    // to the right edge of the same (by its center)
    (p4.width == p2.width / AL.CM(2.0)) as AL.Constraint,
    (p4.height == AL.CM(50.0)) as AL.Constraint,
    (p4.horizontalCenter == p2.rightEdge) as AL.Constraint,
    (p4.verticalCenter == p2.height / AL.CM(2.0)) as AL.Constraint,
  ]);

  new SkyBinding(root: root);
}
