/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

#include "variable_base.h"

#include "../macro/variable_macro.h"

namespace Wge {
namespace Variable {
class TimeBase : public VariableBase {

public:
  enum class TimeType {
    TimeDay = 0,
    TimeEpoch,
    TimeHour,
    TimeMin,
    TimeMon,
    TimeSec,
    TimeWDay,
    TimeYear,
    Time
  };

public:
  TimeBase(std::string&& sub_name, bool is_not, bool is_counter,
           std::string_view curr_rule_file_path, TimeType time_type)
      : VariableBase(std::move(sub_name), is_not, is_counter), time_type_(time_type) {

    switch (time_type_) {
    case TimeType::TimeDay:
      evaluate_func_ =
          std::bind(&TimeBase::evaluateTimeDay, this, std::placeholders::_1, std::placeholders::_2);
      break;
    case TimeType::TimeEpoch:
      evaluate_func_ = std::bind(&TimeBase::evaluateTimeEpoch, this, std::placeholders::_1,
                                 std::placeholders::_2);
      break;
    case TimeType::TimeHour:
      evaluate_func_ = std::bind(&TimeBase::evaluateTimeHour, this, std::placeholders::_1,
                                 std::placeholders::_2);
      break;
    case TimeType::TimeMin:
      evaluate_func_ =
          std::bind(&TimeBase::evaluateTimeMin, this, std::placeholders::_1, std::placeholders::_2);
      break;
    case TimeType::TimeMon:
      evaluate_func_ =
          std::bind(&TimeBase::evaluateTimeMon, this, std::placeholders::_1, std::placeholders::_2);
      break;
    case TimeType::TimeSec:
      evaluate_func_ =
          std::bind(&TimeBase::evaluateTimeSec, this, std::placeholders::_1, std::placeholders::_2);
      break;
    case TimeType::TimeWDay:
      evaluate_func_ = std::bind(&TimeBase::evaluateTimeWDay, this, std::placeholders::_1,
                                 std::placeholders::_2);
      break;
    case TimeType::TimeYear:
      evaluate_func_ = std::bind(&TimeBase::evaluateTimeYear, this, std::placeholders::_1,
                                 std::placeholders::_2);
      break;
    case TimeType::Time:
      evaluate_func_ =
          std::bind(&TimeBase::evaluateTime, this, std::placeholders::_1, std::placeholders::_2);
      break;
    default:
      UNREACHABLE();
      break;
    }
  }

  TimeBase(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
           std::string_view curr_rule_file_path, TimeType time_type)
      : VariableBase("", is_not, is_counter) {
    // Does not support sub_name macro
    UNREACHABLE();
  }

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    evaluate_func_(t, result);
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }

private:
  void evaluateTimeDay(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_mday);
  }

  void evaluateTimeEpoch(Transaction& t, Common::EvaluateResults& result) const {
    result.emplace_back(static_cast<int64_t>(t.getDuration().start()));
  }

  void evaluateTimeHour(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_hour);
  }

  void evaluateTimeMin(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_min);
  }

  void evaluateTimeMon(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_mon + 1);
  }

  void evaluateTimeSec(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_sec);
  }

  void evaluateTimeWDay(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_wday);
  }

  void evaluateTimeYear(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_year + 1900);
  }

  void evaluateTime(Transaction& t, Common::EvaluateResults& result) const {
    std::tm& tm = t.getTm();
    result.emplace_back(tm.tm_hour * 10000 + tm.tm_min * 100 + tm.tm_sec);
  }

private:
  TimeType time_type_;
  std::function<void(Transaction& t, Common::EvaluateResults&)> evaluate_func_;
};

class Time final : public TimeBase {
  DECLARE_VIRABLE_NAME(TIME);

public:
  Time(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : TimeBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path,
                 TimeBase::TimeType::Time) {}

  Time(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
       std::string_view curr_rule_file_path)
      : TimeBase("", is_not, is_counter, curr_rule_file_path, TimeBase::TimeType::Time) {
    // Does not support sub_name macro
    UNREACHABLE();
  }
};
} // namespace Variable
} // namespace Wge