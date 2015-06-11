// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/application/application_runner_chromium.h"
#include "mojo/public/c/system/main.h"
#include "mojo/public/cpp/application/application_connection.h"
#include "mojo/public/cpp/application/application_delegate.h"
#include "mojo/public/cpp/bindings/strong_binding.h"
#include "services/prediction/prediction_service_impl.h"

namespace prediction {

PredictionServiceImpl::PredictionServiceImpl(
    mojo::InterfaceRequest<PredictionService> request)
    : strong_binding_(this, request.Pass()) {
}

PredictionServiceImpl::~PredictionServiceImpl() {
}

// PredictionService implementation
void PredictionServiceImpl::SetSettings(SettingsPtr settings) {
  stored_settings_.correction_enabled = settings->correction_enabled;
  stored_settings_.block_potentially_offensive =
      settings->block_potentially_offensive;
  stored_settings_.space_aware_gesture_enabled =
      settings->space_aware_gesture_enabled;
}

// only predict "cat" no matter what prediction_info
// has for now
void PredictionServiceImpl::GetPredictionList(
    PredictionInfoPtr prediction_info,
    const GetPredictionListCallback& callback) {
  mojo::String cat = "cat";
  mojo::Array<mojo::String> prediction_list;
  prediction_list.push_back(cat);
  callback.Run(prediction_list.Pass());
}

PredictionServiceDelegate::PredictionServiceDelegate() {
}

PredictionServiceDelegate::~PredictionServiceDelegate() {
}

// mojo::ApplicationDelegate implementation
bool PredictionServiceDelegate::ConfigureIncomingConnection(
    mojo::ApplicationConnection* connection) {
  connection->AddService<PredictionService>(this);
  return true;
}

// mojo::InterfaceRequest<PredictionService> implementation
void PredictionServiceDelegate::Create(
    mojo::ApplicationConnection* connection,
    mojo::InterfaceRequest<PredictionService> request) {
  new PredictionServiceImpl(request.Pass());
}

}  // namespace prediction

MojoResult MojoMain(MojoHandle application_request) {
  mojo::ApplicationRunnerChromium runner(
      new prediction::PredictionServiceDelegate());
  return runner.Run(application_request);
}
