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
    // Sum of widths is 100 units
    (p1.width + p2.width + p3.width == AL.CM(300.0)) as AL.Constraint,

    // The positions and sizes must be positive
    // TODO: Make these implicit weak
    p1.width >= AL.CM(0.0),
    p2.width >= AL.CM(0.0),
    p3.width >= AL.CM(0.0),

    p1.leftEdge >= AL.CM(0.0),
    p2.leftEdge >= AL.CM(0.0),
    p3.leftEdge >= AL.CM(0.0),

    // They must be stacked left to right
    p1.rightEdge <= p2.leftEdge,
    p2.rightEdge <= p3.leftEdge,

    // Their widths must be equal
    (p1.width == p2.width) as AL.Constraint,
    (p2.width == p3.width) as AL.Constraint,

    // Their heights should be equal to a constant
    (p1.height == p2.height) as AL.Constraint,
    (p2.height == p3.height) as AL.Constraint,
    (p3.height == AL.CM(300.0)) as AL.Constraint,

    // The center of the last box must be over the right edge of the second box
    (p4.width == AL.CM(50.0)) as AL.Constraint,
    (p4.height == AL.CM(50.0)) as AL.Constraint,
  ]);

  new SkyBinding(root: root);
}
