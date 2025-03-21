#pragma once

#include <string>
#include <string_view>

#include "full_name.h"

#include "../common/assert.h"
#include "../common/evaluate_result.h"
#include "../common/variant.h"
#include "../http_extractor.h"
#include "../transaction.h"

#define DECLARE_VIRABLE_NAME(name)                                                                 \
public:                                                                                            \
  FullName fullName() const override { return {main_name_, sub_name_}; }                           \
  const char* mainName() const override { return main_name_; }                                     \
                                                                                                   \
private:                                                                                           \
  static constexpr char main_name_[] = #name;

namespace SrSecurity {
namespace Variable {

/**
 * Base class for all variables.
 */
class VariableBase {
public:
  VariableBase(std::string&& sub_name, bool is_not, bool is_counter)
      : sub_name_(std::move(sub_name)), is_not_(is_not), is_counter_(is_counter) {
    // The name of variable is case-insensitive.
    std::transform(sub_name_.begin(), sub_name_.end(), sub_name_.begin(), ::tolower);
  }
  virtual ~VariableBase() = default;

public:
  /**
   * Evaluate the variable.
   * @param t the transaction.
   * @param result the result of the evaluation.
   */
  virtual void evaluate(Transaction& t, Common::EvaluateResults& result) const = 0;

  /**
   * Get the full name of the variable.
   * @return the full name of the variable.
   */
  virtual FullName fullName() const = 0;

  /**
   * Get the main(collection) name of the variable.
   * @return the main(collection) name of the variable.
   */
  virtual const char* mainName() const = 0;

  /**
   * Get whether the variable is a collection.
   * @return true if the variable is a collection, false otherwise.
   */
  virtual bool isCollection() const { return false; }

public:
  /**
   * Get the sub name of the variable.
   * @return the sub name of the variable.
   */
  const std::string& subName() const { return sub_name_; }

  /**
   * Get whether the variable is negated.
   * @return true if the variable is negated, false otherwise.
   */
  bool isNot() const { return is_not_; }

  /**
   * Get whether the variable is a counter.
   * @return true if the variable is a counter, false otherwise.
   */
  bool isCounter() const { return is_counter_; }

  /**
   * Update the variable.
   * @param is_not whether the variable is negated.
   * @param is_counter whether the variable is a counter.
   */
  void update(bool is_not, bool is_counter) {
    ASSERT_IS_MAIN_THREAD();
    is_not_ = is_not;
    is_counter_ = is_counter;
  }

protected:
  std::string sub_name_;
  bool is_not_;
  bool is_counter_;
};
} // namespace Variable
} // namespace SrSecurity