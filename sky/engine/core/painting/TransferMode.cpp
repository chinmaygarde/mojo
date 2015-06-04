// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "sky/engine/config.h"
#include "sky/engine/core/painting/TransferMode.h"

#include "sky/engine/core/script/dom_dart_state.h"
#include "sky/engine/tonic/dart_builtin.h"
#include "sky/engine/tonic/dart_error.h"
#include "sky/engine/tonic/dart_value.h"

namespace blink {

// If this fails, it's because SkXfermode has changed. We need to change
// TransferMode.dart to ensure the TransferMode enum is in sync with the C++
// values.
COMPILE_ASSERT(SkXfermode::kLastMode == 28, Need_to_update_TransferMode_dart);

// Convert dart_mode => SkXfermode::Mode.
SkXfermode::Mode DartConverter<TransferMode>::FromArgumentsWithNullCheck(
    Dart_NativeArguments args,
    int index,
    Dart_Handle& exception) {
  SkXfermode::Mode result;

  Dart_Handle dart_mode = Dart_GetNativeArgument(args, index);
  DCHECK(!LogIfError(dart_mode));

  Dart_Handle value =
      Dart_GetField(dart_mode, DOMDartState::Current()->index_handle());

  uint64_t mode = 0;
  Dart_Handle rv = Dart_IntegerToUint64(value, &mode);
  DCHECK(!LogIfError(rv));

  result = static_cast<SkXfermode::Mode>(mode);
  return result;
}

} // namespace blink
