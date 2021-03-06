// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FeatureExtractionApi.proto

#ifndef PROTOBUF_FeatureExtractionApi_2eproto__INCLUDED
#define PROTOBUF_FeatureExtractionApi_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_FeatureExtractionApi_2eproto();
void protobuf_InitDefaults_FeatureExtractionApi_2eproto();
void protobuf_AssignDesc_FeatureExtractionApi_2eproto();
void protobuf_ShutdownFile_FeatureExtractionApi_2eproto();

class Image;
class Response;

// ===================================================================

class Image : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Image) */ {
 public:
  Image();
  virtual ~Image();

  Image(const Image& from);

  inline Image& operator=(const Image& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Image& default_instance();

  static const Image* internal_default_instance();

  void Swap(Image* other);

  // implements Message ----------------------------------------------

  inline Image* New() const { return New(NULL); }

  Image* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Image& from);
  void MergeFrom(const Image& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Image* other);
  void UnsafeMergeFrom(const Image& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional bytes source = 1;
  void clear_source();
  static const int kSourceFieldNumber = 1;
  const ::std::string& source() const;
  void set_source(const ::std::string& value);
  void set_source(const char* value);
  void set_source(const void* value, size_t size);
  ::std::string* mutable_source();
  ::std::string* release_source();
  void set_allocated_source(::std::string* source);

  // optional string file_name = 2;
  void clear_file_name();
  static const int kFileNameFieldNumber = 2;
  const ::std::string& file_name() const;
  void set_file_name(const ::std::string& value);
  void set_file_name(const char* value);
  void set_file_name(const char* value, size_t size);
  ::std::string* mutable_file_name();
  ::std::string* release_file_name();
  void set_allocated_file_name(::std::string* file_name);

  // @@protoc_insertion_point(class_scope:Image)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr source_;
  ::google::protobuf::internal::ArenaStringPtr file_name_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_FeatureExtractionApi_2eproto_impl();
  friend void  protobuf_AddDesc_FeatureExtractionApi_2eproto_impl();
  friend void protobuf_AssignDesc_FeatureExtractionApi_2eproto();
  friend void protobuf_ShutdownFile_FeatureExtractionApi_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<Image> Image_default_instance_;

// -------------------------------------------------------------------

class Response : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Response) */ {
 public:
  Response();
  virtual ~Response();

  Response(const Response& from);

  inline Response& operator=(const Response& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Response& default_instance();

  static const Response* internal_default_instance();

  void Swap(Response* other);

  // implements Message ----------------------------------------------

  inline Response* New() const { return New(NULL); }

  Response* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Response& from);
  void MergeFrom(const Response& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Response* other);
  void UnsafeMergeFrom(const Response& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string status = 1;
  void clear_status();
  static const int kStatusFieldNumber = 1;
  const ::std::string& status() const;
  void set_status(const ::std::string& value);
  void set_status(const char* value);
  void set_status(const char* value, size_t size);
  ::std::string* mutable_status();
  ::std::string* release_status();
  void set_allocated_status(::std::string* status);

  // @@protoc_insertion_point(class_scope:Response)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr status_;
  mutable int _cached_size_;
  friend void  protobuf_InitDefaults_FeatureExtractionApi_2eproto_impl();
  friend void  protobuf_AddDesc_FeatureExtractionApi_2eproto_impl();
  friend void protobuf_AssignDesc_FeatureExtractionApi_2eproto();
  friend void protobuf_ShutdownFile_FeatureExtractionApi_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<Response> Response_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Image

// optional bytes source = 1;
inline void Image::clear_source() {
  source_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Image::source() const {
  // @@protoc_insertion_point(field_get:Image.source)
  return source_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Image::set_source(const ::std::string& value) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Image.source)
}
inline void Image::set_source(const char* value) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Image.source)
}
inline void Image::set_source(const void* value, size_t size) {
  
  source_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Image.source)
}
inline ::std::string* Image::mutable_source() {
  
  // @@protoc_insertion_point(field_mutable:Image.source)
  return source_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Image::release_source() {
  // @@protoc_insertion_point(field_release:Image.source)
  
  return source_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Image::set_allocated_source(::std::string* source) {
  if (source != NULL) {
    
  } else {
    
  }
  source_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), source);
  // @@protoc_insertion_point(field_set_allocated:Image.source)
}

// optional string file_name = 2;
inline void Image::clear_file_name() {
  file_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Image::file_name() const {
  // @@protoc_insertion_point(field_get:Image.file_name)
  return file_name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Image::set_file_name(const ::std::string& value) {
  
  file_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Image.file_name)
}
inline void Image::set_file_name(const char* value) {
  
  file_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Image.file_name)
}
inline void Image::set_file_name(const char* value, size_t size) {
  
  file_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Image.file_name)
}
inline ::std::string* Image::mutable_file_name() {
  
  // @@protoc_insertion_point(field_mutable:Image.file_name)
  return file_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Image::release_file_name() {
  // @@protoc_insertion_point(field_release:Image.file_name)
  
  return file_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Image::set_allocated_file_name(::std::string* file_name) {
  if (file_name != NULL) {
    
  } else {
    
  }
  file_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), file_name);
  // @@protoc_insertion_point(field_set_allocated:Image.file_name)
}

inline const Image* Image::internal_default_instance() {
  return &Image_default_instance_.get();
}
// -------------------------------------------------------------------

// Response

// optional string status = 1;
inline void Response::clear_status() {
  status_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Response::status() const {
  // @@protoc_insertion_point(field_get:Response.status)
  return status_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Response::set_status(const ::std::string& value) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Response.status)
}
inline void Response::set_status(const char* value) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Response.status)
}
inline void Response::set_status(const char* value, size_t size) {
  
  status_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Response.status)
}
inline ::std::string* Response::mutable_status() {
  
  // @@protoc_insertion_point(field_mutable:Response.status)
  return status_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Response::release_status() {
  // @@protoc_insertion_point(field_release:Response.status)
  
  return status_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Response::set_allocated_status(::std::string* status) {
  if (status != NULL) {
    
  } else {
    
  }
  status_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), status);
  // @@protoc_insertion_point(field_set_allocated:Response.status)
}

inline const Response* Response::internal_default_instance() {
  return &Response_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_FeatureExtractionApi_2eproto__INCLUDED
