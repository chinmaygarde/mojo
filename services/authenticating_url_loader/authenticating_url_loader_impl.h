// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_AUTHENTICATING_URL_LOADER_AUTHENTICATING_URL_LOADER_IMPL_H_
#define SERVICES_AUTHENTICATING_URL_LOADER_AUTHENTICATING_URL_LOADER_IMPL_H_

#include "mojo/public/cpp/bindings/binding.h"
#include "mojo/public/cpp/bindings/error_handler.h"
#include "mojo/services/authenticating_url_loader/public/interfaces/authenticating_url_loader.mojom.h"
#include "mojo/services/network/public/interfaces/url_loader.mojom.h"
#include "services/authenticating_url_loader/authenticating_url_loader_factory_impl.h"
#include "url/gurl.h"

namespace mojo {

class NetworkService;

enum RequestAuthorizationState {
  REQUEST_INITIAL,
  REQUEST_USED_CURRENT_AUTH_SERVICE_TOKEN,
  REQUEST_USED_FRESH_AUTH_SERVICE_TOKEN,
};

class AuthenticatingURLLoaderImpl : public AuthenticatingURLLoader,
                                    public ErrorHandler {
 public:
  AuthenticatingURLLoaderImpl(InterfaceRequest<AuthenticatingURLLoader> request,
                              AuthenticatingURLLoaderFactoryImpl* factory);
  ~AuthenticatingURLLoaderImpl() override;

 private:
  // AuthenticatingURLLoader methods:
  void Start(URLRequestPtr request,
             const Callback<void(URLResponsePtr)>& callback) override;
  void FollowRedirect(const Callback<void(URLResponsePtr)>& callback) override;

  // ErrorHandler methods:
  void OnConnectionError() override;

  void StartNetworkRequest(URLRequestPtr request);

  void OnLoadComplete(URLResponsePtr response);

  void FollowRedirectInternal();

  void OnOAuth2TokenReceived(std::string token);

  Binding<AuthenticatingURLLoader> binding_;
  AuthenticatingURLLoaderFactoryImpl* factory_;
  URLLoaderPtr url_loader_;
  URLResponsePtr pending_response_;
  RequestAuthorizationState request_authorization_state_;
  GURL url_;
  bool auto_follow_redirects_;
  bool bypass_cache_;
  Array<HttpHeaderPtr> headers_;
  Callback<void(URLResponsePtr)> pending_request_callback_;
};

}  // namespace mojo

#endif  // SERVICES_AUTHENTICATING_URL_LOADER_AUTHENTICATING_URL_LOADER_IMPL_H_
