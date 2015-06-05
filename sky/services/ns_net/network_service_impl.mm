// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "network_service_impl.h"
#include "url_loader_impl.h"
#include "base/logging.h"
#include <Foundation/Foundation.h>

namespace mojo {

void NetworkServiceImpl::CreateURLLoader(
    mojo::InterfaceRequest<mojo::URLLoader> loader) {
  new URLLoaderImpl(loader.Pass());
}

void NetworkServiceImpl::GetCookieStore(
    mojo::InterfaceRequest<mojo::CookieStore> cookie_store) {
  DCHECK(false);
}

void NetworkServiceImpl::CreateWebSocket(
    mojo::InterfaceRequest<mojo::WebSocket> socket) {
  DCHECK(false);
}

void NetworkServiceImpl::CreateTCPBoundSocket(
    mojo::NetAddressPtr local_address,
    mojo::InterfaceRequest<mojo::TCPBoundSocket> bound_socket,
    const CreateTCPBoundSocketCallback& callback) {
  DCHECK(false);
}

void NetworkServiceImpl::CreateTCPConnectedSocket(
    mojo::NetAddressPtr remote_address,
    mojo::ScopedDataPipeConsumerHandle send_stream,
    mojo::ScopedDataPipeProducerHandle receive_stream,
    mojo::InterfaceRequest<mojo::TCPConnectedSocket> client_socket,
    const CreateTCPConnectedSocketCallback& callback) {
  DCHECK(false);
}

void NetworkServiceImpl::CreateUDPSocket(
    mojo::InterfaceRequest<mojo::UDPSocket> socket) {
  DCHECK(false);
}

void NetworkServiceImpl::CreateHttpServer(
    mojo::NetAddressPtr local_address,
    mojo::HttpServerDelegatePtr delegate,
    const CreateHttpServerCallback& callback) {
  DCHECK(false);
}

void NetworkServiceFactory::Create(ApplicationConnection* connection,
                                   InterfaceRequest<NetworkService> request) {
  new NetworkServiceImpl(request.Pass());
}

}  // namespace mojo
