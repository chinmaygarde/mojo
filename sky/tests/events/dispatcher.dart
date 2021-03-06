import "../resources/third_party/unittest/unittest.dart";
import "../resources/unit.dart";
import "dart:sky";
import 'dart:async';

void send20IntegersToDispatcherController(DispatcherController d) {
  for (var index = 0; index < 20; index += 1)
    d.add(index);
}

void main() {
  initUnit();

  group('Dispatcher', () {

    test('simple listen', () {
      var d = new DispatcherController();
      var result = new List();
      d.dispatcher.listen((v) => result.add(v));
      send20IntegersToDispatcherController(d);
      expect(result, orderedEquals([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]));
    });

    test('firstWhere', () {
      var d = new DispatcherController();
      var result = new List();
      d.dispatcher.firstWhere((n) => n > 15).then((v) => result.add(v));
      send20IntegersToDispatcherController(d);
      new Timer(new Duration(), expectAsync(() => expect(result, orderedEquals([16]))));
    });

    test('where', () {
      var d = new DispatcherController();
      var result = new List();
      d.dispatcher.where((n) => n % 2 == 0).until((n) => n > 10).listen((v) => result.add(v));
      send20IntegersToDispatcherController(d);
      expect(result, orderedEquals([0,2,4,6,8,10]));
    });

    test('where without listener', () {
      var d = new DispatcherController();
      var result = new List();
      d.dispatcher.where((n) => result.add(n));
      send20IntegersToDispatcherController(d);
      expect(result, orderedEquals([]));
    });

    test('where with listeners removed', () {
      var d = new DispatcherController();
      var result = new List();
      var w = d.dispatcher.where((n) { result.add(n); return true; });
      d.add(0);
      var f1 = (v) => result.add(100 + v);
      w.listen(f1);
      d.add(1);
      var f2 = (v) => result.add(200 + v);
      w.listen(f2);
      d.add(2);
      w.unlisten(f1);
      d.add(3);
      w.unlisten(f2);
      d.add(4);
      expect(result, orderedEquals([1, 101, 2, 102, 202, 3, 203]));
    });

    test('double listeners', () {
      var d = new DispatcherController();
      var result = new List();
      d.dispatcher..listen((v) => result.add(v))
                  ..where((n) => n < 3).listen((v) => result.add(v*100));
      send20IntegersToDispatcherController(d);
      expect(result, orderedEquals([0,0,1,100,2,200,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19]));
    });

    test('zones', () {
      var d = new DispatcherController();
      var result = new List();
      runZoned(() {
        var zoneA = Zone.current;
        var listener;
        listener = (v) {
          if (Zone.current == zoneA)
            result.add(v + 0.1);
          if (v >= 18)
            d.dispatcher.unlisten(listener);
        };
        d.dispatcher.listen(listener);
      });
      runZoned(() {
        var zoneB = Zone.current;
        d.dispatcher.until((n) => n >= 10).listen((v) {
          if (Zone.current == zoneB)
            result.add(v + 0.2);
        });
      });
      send20IntegersToDispatcherController(d);
      expect(result, orderedEquals([0.1,0.2,1.1,1.2,2.1,2.2,3.1,3.2,4.1,4.2,5.1,5.2,6.1,6.2,7.1,7.2,8.1,8.2,9.1,9.2,10.1,11.1,12.1,13.1,14.1,15.1,16.1,17.1,18.1]));
    });

  });
}
