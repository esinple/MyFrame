// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Gate2Super.proto

#ifndef PROTOBUF_Gate2Super_2eproto__INCLUDED
#define PROTOBUF_Gate2Super_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace MPMsg {
class GateVerify_Gate2Super;
class GateVerify_Gate2SuperDefaultTypeInternal;
extern GateVerify_Gate2SuperDefaultTypeInternal _GateVerify_Gate2Super_default_instance_;
class GateVerify_Super2Gate;
class GateVerify_Super2GateDefaultTypeInternal;
extern GateVerify_Super2GateDefaultTypeInternal _GateVerify_Super2Gate_default_instance_;
}  // namespace MPMsg

namespace MPMsg {

namespace protobuf_Gate2Super_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_Gate2Super_2eproto

// ===================================================================

class GateVerify_Gate2Super : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:MPMsg.GateVerify_Gate2Super) */ {
 public:
  GateVerify_Gate2Super();
  virtual ~GateVerify_Gate2Super();

  GateVerify_Gate2Super(const GateVerify_Gate2Super& from);

  inline GateVerify_Gate2Super& operator=(const GateVerify_Gate2Super& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  GateVerify_Gate2Super(GateVerify_Gate2Super&& from) noexcept
    : GateVerify_Gate2Super() {
    *this = ::std::move(from);
  }

  inline GateVerify_Gate2Super& operator=(GateVerify_Gate2Super&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const GateVerify_Gate2Super& default_instance();

  static inline const GateVerify_Gate2Super* internal_default_instance() {
    return reinterpret_cast<const GateVerify_Gate2Super*>(
               &_GateVerify_Gate2Super_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(GateVerify_Gate2Super* other);
  friend void swap(GateVerify_Gate2Super& a, GateVerify_Gate2Super& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline GateVerify_Gate2Super* New() const PROTOBUF_FINAL { return New(NULL); }

  GateVerify_Gate2Super* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const GateVerify_Gate2Super& from);
  void MergeFrom(const GateVerify_Gate2Super& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(GateVerify_Gate2Super* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:MPMsg.GateVerify_Gate2Super)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  mutable int _cached_size_;
  friend struct protobuf_Gate2Super_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class GateVerify_Super2Gate : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:MPMsg.GateVerify_Super2Gate) */ {
 public:
  GateVerify_Super2Gate();
  virtual ~GateVerify_Super2Gate();

  GateVerify_Super2Gate(const GateVerify_Super2Gate& from);

  inline GateVerify_Super2Gate& operator=(const GateVerify_Super2Gate& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  GateVerify_Super2Gate(GateVerify_Super2Gate&& from) noexcept
    : GateVerify_Super2Gate() {
    *this = ::std::move(from);
  }

  inline GateVerify_Super2Gate& operator=(GateVerify_Super2Gate&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const GateVerify_Super2Gate& default_instance();

  static inline const GateVerify_Super2Gate* internal_default_instance() {
    return reinterpret_cast<const GateVerify_Super2Gate*>(
               &_GateVerify_Super2Gate_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(GateVerify_Super2Gate* other);
  friend void swap(GateVerify_Super2Gate& a, GateVerify_Super2Gate& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline GateVerify_Super2Gate* New() const PROTOBUF_FINAL { return New(NULL); }

  GateVerify_Super2Gate* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const GateVerify_Super2Gate& from);
  void MergeFrom(const GateVerify_Super2Gate& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(GateVerify_Super2Gate* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint32 err_code = 1;
  void clear_err_code();
  static const int kErrCodeFieldNumber = 1;
  ::google::protobuf::uint32 err_code() const;
  void set_err_code(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:MPMsg.GateVerify_Super2Gate)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 err_code_;
  mutable int _cached_size_;
  friend struct protobuf_Gate2Super_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// GateVerify_Gate2Super

// -------------------------------------------------------------------

// GateVerify_Super2Gate

// uint32 err_code = 1;
inline void GateVerify_Super2Gate::clear_err_code() {
  err_code_ = 0u;
}
inline ::google::protobuf::uint32 GateVerify_Super2Gate::err_code() const {
  // @@protoc_insertion_point(field_get:MPMsg.GateVerify_Super2Gate.err_code)
  return err_code_;
}
inline void GateVerify_Super2Gate::set_err_code(::google::protobuf::uint32 value) {
  
  err_code_ = value;
  // @@protoc_insertion_point(field_set:MPMsg.GateVerify_Super2Gate.err_code)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace MPMsg

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Gate2Super_2eproto__INCLUDED