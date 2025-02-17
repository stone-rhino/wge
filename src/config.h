#pragma once

#include <bitset>
#include <optional>
#include <string>

namespace SrSecurity {
/**
 * The configuration of the SrSecurity engine.
 */
struct EngineConfig {
  enum class Option { On, Off, DetectionOnly };
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
} // namespace SrSecurity