// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: FeatureExtractionApi.proto
#ifndef GRPC_FeatureExtractionApi_2eproto__INCLUDED
#define GRPC_FeatureExtractionApi_2eproto__INCLUDED

#include "FeatureExtractionApi.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

class FeatureExtraction final {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientWriterInterface< ::Image>> processVideo(::grpc::ClientContext* context, ::Response* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::Image>>(processVideoRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::Image>> AsyncprocessVideo(::grpc::ClientContext* context, ::Response* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::Image>>(AsyncprocessVideoRaw(context, response, cq, tag));
    }
  private:
    virtual ::grpc::ClientWriterInterface< ::Image>* processVideoRaw(::grpc::ClientContext* context, ::Response* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::Image>* AsyncprocessVideoRaw(::grpc::ClientContext* context, ::Response* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientWriter< ::Image>> processVideo(::grpc::ClientContext* context, ::Response* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::Image>>(processVideoRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::Image>> AsyncprocessVideo(::grpc::ClientContext* context, ::Response* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::Image>>(AsyncprocessVideoRaw(context, response, cq, tag));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientWriter< ::Image>* processVideoRaw(::grpc::ClientContext* context, ::Response* response) override;
    ::grpc::ClientAsyncWriter< ::Image>* AsyncprocessVideoRaw(::grpc::ClientContext* context, ::Response* response, ::grpc::CompletionQueue* cq, void* tag) override;
    const ::grpc::RpcMethod rpcmethod_processVideo_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status processVideo(::grpc::ServerContext* context, ::grpc::ServerReader< ::Image>* reader, ::Response* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_processVideo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_processVideo() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_processVideo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status processVideo(::grpc::ServerContext* context, ::grpc::ServerReader< ::Image>* reader, ::Response* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestprocessVideo(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::Response, ::Image>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(0, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_processVideo<Service > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_processVideo : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_processVideo() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_processVideo() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status processVideo(::grpc::ServerContext* context, ::grpc::ServerReader< ::Image>* reader, ::Response* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};


#endif  // GRPC_FeatureExtractionApi_2eproto__INCLUDED
