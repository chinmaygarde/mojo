// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/public/cpp/application/interface_factory.h"
#include "mojo/public/cpp/bindings/strong_binding.h"
#include "mojo/services/network/public/interfaces/network_service.mojom.h"

namespace mojo {

class NetworkServiceImpl : public NetworkService {
 public:
  explicit NetworkServiceImpl(InterfaceRequest<NetworkService> request) 
      : binding_(this, request.Pass()) {}

  void CreateURLLoader(mojo::InterfaceRequest<mojo::URLLoader> loader) override;
  void GetCookieStore(
               mojo::InterfaceRequest<mojo::CookieStore> cookie_store) override;
  void CreateWebSocket(mojo::InterfaceRequest<mojo::WebSocket> socket) override;
  void CreateTCPBoundSocket(
                      mojo::NetAddressPtr local_address, 
                      mojo::InterfaceRequest<mojo::TCPBoundSocket> bound_socket,
                      const CreateTCPBoundSocketCallback& callback) override;
  void CreateTCPConnectedSocket(
                mojo::NetAddressPtr remote_address,
                mojo::ScopedDataPipeConsumerHandle send_stream,
                mojo::ScopedDataPipeProducerHandle receive_stream, 
                mojo::InterfaceRequest<mojo::TCPConnectedSocket> client_socket,
                const CreateTCPConnectedSocketCallback& callback) override;
  void CreateUDPSocket(mojo::InterfaceRequest<mojo::UDPSocket> socket) override;

 private:
  StrongBinding<NetworkService> binding_;
};

class NetworkServiceFactory : public InterfaceFactory<NetworkService> {
 public:
  void Create(ApplicationConnection* connection,
              InterfaceRequest<NetworkService> request) override;
};

}  // namespace mojo
