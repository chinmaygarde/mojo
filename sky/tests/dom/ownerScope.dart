import "../resources/third_party/unittest/unittest.dart";
import "../resources/unit.dart";

import "dart:sky";

void main() {
  initUnit();

  test("should return null for elements not a child of a scope", () {
    var doc = new Document();
    var element = doc.createElement("div");
    expect(element.owner, isNull);
  });
  test("should return the document for elements in the document scope", () {
    var doc = new Document();
    var element = doc.createElement("div");
    doc.appendChild(element);
    expect(element.owner, equals(doc));
  });
  test("should return the shadow root for elements in the shadow root scope", () {
    var doc = new Document();
    var host = doc.createElement("div");
    var child = doc.createElement("div");
    var shadowRoot = host.ensureShadowRoot();
    shadowRoot.appendChild(child);
    expect(child.owner, equals(shadowRoot));
  });
  test("should return self for a shadow root or document", () {
    var doc = new Document();
    var host = doc.createElement("div");
    doc.appendChild(host);
    var shadowRoot = host.ensureShadowRoot();
    expect(shadowRoot.owner, equals(shadowRoot));
    expect(doc.owner, equals(doc));
  });
  test("should dynamically update", () {
    var doc = new Document();
    var host = doc.createElement("div");
    var child = doc.createElement("div");
    var shadowRoot = host.ensureShadowRoot();
    expect(child.owner, isNull);
    shadowRoot.appendChild(child);
    expect(child.owner, equals(shadowRoot));
    child.remove();
    expect(child.owner, isNull);
  });
}
