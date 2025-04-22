/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <bitset>
#include <optional>
#include <string>
#include <vector>

#include <stdint.h>

namespace SrSecurity {
// There are five phases in the ModSecurity engine
// 1. Request headers are read
// 2. Request body is read
// 3. Response headers are read
// 4. Response body is read
// 5. Logging
// We use an array to store the rules in each phase, and the index of the array is phase - 1.
constexpr size_t PHASE_TOTAL = 5;

/**
 * The configuration of the SrSecurity engine.
 */
struct EngineConfig {
  enum class Option { On, Off, DetectionOnly };
  enum class BodyLimitAction { Reject, ProcessPartial };

  // SecRequestBodyAccess
  // Configures whether request bodies will be buffered and processed by ModSecurity.
  Option is_request_body_access_{Option::Off};

  // SecResponseBodyAccess
  // Configures whether response bodies are to be buffered.
  Option is_response_body_access_{Option::Off};

  // SecRuleEngine
  // Configures the rules engine.
  Option is_rule_engine_{Option::Off};

  // SecTmpSaveUploadedFiles
  // Configures whether or not files uploaded via a multipart POST request will be temporarily
  // saved to the file system.
  Option is_tmp_save_uploaded_files_{Option::Off};

  // SecUploadKeepFiles
  // Configures whether or not the intercepted files will be kept after transaction is processed.
  Option is_upload_keep_files_{Option::Off};

  // SecXmlExternalEntity
  // Enable or Disable the loading process of xml external entity. Loading external entity without
  // correct verifying process can lead to a security issue.
  Option is_xml_external_entity_{Option::Off};

  // SecUploadFileLimit
  // Configures the maximum number of file uploads processed in a multipart POST.
  uint32_t upload_file_limit_{0};

  // SecResponseBodyMimeType
  // Configures which MIME types are to be considered for response body buffering.
  std::vector<std::string> response_body_mime_types_;

  // SecRequestBodyLimit
  // Configures the maximum request body size ModSecurity will accept for buffering.
  // Default: 128KB
  uint64_t request_body_limit_{134217728};

  // SecRequestBodyNoFilesLimit
  // Default: 1MB
  uint64_t request_body_no_files_limit_{1048576};

  // Configures the maximum parsing depth that is allowed when parsing a JSON object.
  // Default: 0 (unlimited)
  uint64_t request_body_json_depth_limit_{0};

  // SecResponseBodyLimit
  // Configures the maximum response body size that will be accepted for buffering.
  // Default: 512KB
  uint64_t response_body_limit_{524288};

  // SecRequestBodyLimitAction
  // Controls what happens once a request body limit, configured with SecRequestBodyLimit, is
  // encountered
  BodyLimitAction request_body_limit_action_{BodyLimitAction::ProcessPartial};

  // SecResponseBodyLimitAction
  // Controls what happens once a response body limit, configured with SecResponseBodyLimit, is
  // encountered.
  BodyLimitAction response_body_limit_action_{BodyLimitAction::ProcessPartial};

  // SecArgumentsLimit
  // Configures the maximum number of ARGS that will be accepted for processing.
  // Default: 0 (unlimited)
  uint32_t arguments_limit_{0};

  // SecArgumentSeparator
  // Specifies which character to use as the separator for application/x-www-form-urlencoded
  // content. Default: &
  char argument_separator_{'&'};

  // SecUnicodeMapFile
  // Defines the path to the file that will be used by the urlDecodeUni transformation function to
  // map Unicode code points during normalization and specifies the Code Point to use.
  std::string unicode_map_file_;
  uint32_t unicode_code_point_{20127};

  // SecPcreMatchLimit
  // Sets the PCRE match limit for executions of the @rx and @rxGlobal operators.
  uint32_t pcre_match_limit_{0};
};

/**
 * The configuration of the audit log.
 */
struct AuditLogConfig {
  enum class AuditEngine {
    // log all transactions
    On,
    // do not log any transactions
    Off,
    // only the log transactions that have triggered a warning or an error, or have a status code
    // that is considered to be relevant (as determined by the SecAuditLogRelevantStatus
    // directive)
    RelevantOnly
  };

  enum class AuditLogType { Serial, Concurrent, Https };

  enum class AuditFormat { Json, Native };

  enum class AuditLogPart {
    A = 0,
    Headers = A,
    B = 1,
    RequestHeaders = B,
    C = 2,
    RequestBody = C,
    D = 3,
    IntermediaryResponseHeaders = D,
    E = 4,
    IntermediaryResponseBody = E,
    F = 5,
    FinalResponseHeaders = F,
    G = 6,
    FinalResponseBody = G,
    H = 7,
    Trailer = H,
    I = 8,
    J = 9,
    Uploaded = J,
    K = 10,
    Z = 11,
    FinalBoundary = Z,
    End = 12
  };

  // SecAuditEngine
  // Configures the audit logging engine.
  AuditEngine audit_engine_;

  // SecAuditLogType
  // Configures the type of audit logging mechanism to be used.
  AuditLogType audit_log_type_;

  // SecAuditLog
  // Defines the path to the main audit log file (serial logging format), or the concurrent
  // logging index file (concurrent logging format), or the url (HTTPS).
  std::string log_path_;

  // SecAuditLogStorageDir
  // Configures the directory where concurrent audit log entries are to be stored.
  std::string storage_dir_;

  // SecAuditLog2
  // Defines the path to the secondary audit log index file when concurrent logging is enabled.
  // See SecAuditLog for more details.
  std::string log_path2_;

  // SecAuditLogDirMode
  // Configures the mode (permissions) of any directories created for the concurrent audit logs,
  // using an octal mode value as parameter (as used in chmod).
  int dir_mode_;

  // SecAuditLogFileMode
  // Configures the mode (permissions) of any files created for concurrent audit logs using an
  // octal mode (as used in chmod). See SecAuditLogDirMode for controlling the mode of created
  // audit log directories.
  int file_mode_;

  // SecAuditLogFormat
  // Select the output format of the AuditLogs. The format can be either the native AuditLogs
  // format or JSON.
  AuditFormat format_;

  // SecAuditLogParts
  // Defines which parts of each transaction are going to be recorded in the audit log. Each part
  // is assigned a single letter; when a letter appears in the list then the equivalent part will
  // be recorded. See below for the list of all parts.
  std::bitset<16> log_parts_;

  // SecAuditLogRelevantStatus
  // Configures which response status code is to be considered relevant for the purpose of audit
  // logging.
  std::string relevant_status_regex_;

  // SecComponentSignature
  // Appends component signature to the ModSecurity signature.
  std::string component_signature_;
};

/**
 * The configuration of the request body processor.
 */
enum class BodyProcessorType { UrlEncoded, MultiPart, Xml, Json };

struct MultipartStrictError : public std::bitset<16> {
  enum class ErrorType {
    MultipartStrictError = 0,
    ReqbodyProcessorError,
    BoundaryQuoted,
    BoundaryWhitespace,
    DataBefore,
    DataAfter,
    HeaderFolding,
    LfLine,
    MissingSemicolon,
    InvalidQuoting,
    InvalidPart,
    InvalidHeaderFolding,
    FileLimitExceeded,
    UnmatchedBoundary
  };
  bool get(ErrorType type) const { return test(static_cast<size_t>(type)); }
  void set(ErrorType type) {
    std::bitset<16>::set(static_cast<size_t>(type));
    // MultipartStrictError will be set if any of the error is set.
    std::bitset<16>::set(
        static_cast<size_t>(MultipartStrictError::ErrorType::MultipartStrictError));
  }
};
} // namespace SrSecurity