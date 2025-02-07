#pragma once

#include <optional>

namespace SrSecurity {
// Engine configuration
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
} // namespace SrSecurity