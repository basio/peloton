//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// numeric_value.cpp
//
// Identification: src/backend/common/numeric_value.cpp
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "common/tinyint_type.h"

#include <cmath>
#include <iostream>
#include "common/boolean_type.h"
#include "common/decimal_type.h"
#include "common/varlen_type.h"

namespace peloton {
namespace common {

TinyintType::TinyintType() :
    IntegerParentType(TINYINT) {
}

bool TinyintType::IsZero(const Value& val) const {
  return (val.value_.tinyint == 0);
}

Value *TinyintType::Add(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return left.OperateNull(right);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return AddValue<int8_t, int8_t>(left, right);
  case Type::SMALLINT:
    return AddValue<int8_t, int16_t>(left, right);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return AddValue<int8_t, int32_t>(left, right);
  case Type::BIGINT:
    return AddValue<int8_t, int64_t>(left, right);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL, left.value_.tinyint + right.GetAs<double>());
  default:
    break;
  }
  throw Exception("type error");
}

Value *TinyintType::Subtract(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return left.OperateNull(right);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return SubtractValue<int8_t, int8_t>(left, right);
  case Type::SMALLINT:
    return SubtractValue<int8_t, int16_t>(left, right);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return SubtractValue<int8_t, int32_t>(left, right);
  case Type::BIGINT:
    return SubtractValue<int8_t, int64_t>(left, right);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL, left.value_.tinyint - right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::Multiply(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return left.OperateNull(right);
  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return MultiplyValue<int8_t, int8_t>(left, right);
  case Type::SMALLINT:
    return MultiplyValue<int8_t, int16_t>(left, right);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return MultiplyValue<int8_t, int32_t>(left, right);
  case Type::BIGINT:
    return MultiplyValue<int8_t, int64_t>(left, right);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL, left.value_.tinyint * right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::Divide(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return left.OperateNull(right);

  if (right.IsZero()) {
    throw Exception(EXCEPTION_TYPE_DIVIDE_BY_ZERO, "Division by zerright.");
  }

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return DivideValue<int8_t, int8_t>(left, right);
  case Type::SMALLINT:
    return DivideValue<int8_t, int16_t>(left, right);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return DivideValue<int8_t, int32_t>(left, right);
  case Type::BIGINT:
    return DivideValue<int8_t, int64_t>(left, right);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL, left.value_.tinyint / right.GetAs<double>());
  default:
    break;
  }
  throw Exception("type error");
}

Value *TinyintType::Modulo(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return left.OperateNull(right);

  if (right.IsZero()) {
    throw Exception(EXCEPTION_TYPE_DIVIDE_BY_ZERO, "Division by zerright.");
  }

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return ModuloValue<int8_t, int8_t>(left, right);
  case Type::SMALLINT:
    return ModuloValue<int8_t, int16_t>(left, right);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return ModuloValue<int8_t, int32_t>(left, right);
  case Type::BIGINT:
    return ModuloValue<int8_t, int64_t>(left, right);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL,
        ValMod(left.value_.tinyint, right.GetAs<double>()));
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::Sqrt(const Value& val) const {
  val.CheckInteger();
  if (val.IsNull())
    return new Value(Type::DECIMAL, PELOTON_DECIMAL_NULL);

  if (val.value_.tinyint < 0) {
    throw Exception(EXCEPTION_TYPE_DECIMAL,
        "Cannot take square root of a negative number.");
  }
  return new Value(Type::DECIMAL, sqrt(val.value_.tinyint));

  throw Exception("type error");
}

Value *TinyintType::OperateNull(const Value& left UNUSED_ATTRIBUTE, const Value &right) const {
  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(right.GetTypeId(), (int8_t) PELOTON_INT8_NULL);
  case Type::SMALLINT:
    return new Value(right.GetTypeId(), (int16_t) PELOTON_INT16_NULL);
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(right.GetTypeId(), (int32_t) PELOTON_INT32_NULL);
  case Type::BIGINT:
    return new Value(right.GetTypeId(), (int64_t) PELOTON_INT64_NULL);
  case Type::DECIMAL:
    return new Value(Type::DECIMAL, (double) PELOTON_DECIMAL_NULL);
  default:
    break;
  }
  throw Exception("type error");
}

Value *TinyintType::CompareEquals(const Value& left, const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);

  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint == right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint == right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint == right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint == right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint == right.GetAs<double>());
  default:
    break;
  }
  throw Exception("type error");
}

Value *TinyintType::CompareNotEquals(const Value& left,
    const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint != right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint != right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint != right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint != right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint != right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::CompareLessThan(const Value& left,
    const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN, left.value_.tinyint < right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint < right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint < right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint < right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN, left.value_.tinyint < right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::CompareLessThanEquals(const Value& left,
    const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint <= right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint <= right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint <= right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint <= right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint <= right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

Value *TinyintType::CompareGreaterThan(const Value& left,
    const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN, left.value_.tinyint > right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint > right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint > right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint > right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN, left.value_.tinyint > right.GetAs<double>());
  default:
    break;
  }
  throw Exception("type error");
}

Value *TinyintType::CompareGreaterThanEquals(const Value& left,
    const Value &right) const {
  left.CheckInteger();
  left.CheckComparable(right);
  if (left.IsNull() || right.IsNull())
    return new Value(Type::BOOLEAN, PELOTON_BOOLEAN_NULL);

  switch (right.GetTypeId()) {
  case Type::TINYINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint >= right.GetAs<int8_t>());
  case Type::SMALLINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint >= right.GetAs<int16_t>());
  case Type::INTEGER:
  case Type::PARAMETER_OFFSET:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint >= right.GetAs<int32_t>());
  case Type::BIGINT:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint >= right.GetAs<int64_t>());
  case Type::DECIMAL:
    return new Value(Type::BOOLEAN,
        left.value_.tinyint >= right.GetAs<double>());
  default:
    break;
  }

  throw Exception("type error");
}

std::string TinyintType::ToString(const Value& val) const {
  val.CheckInteger();
  if (val.IsNull())
    return "tinyint_null";
  return std::to_string(val.value_.tinyint);

}

size_t TinyintType::Hash(const Value& val) const {
  val.CheckInteger();
  return std::hash<int8_t> { }(val.value_.tinyint);
}

void TinyintType::HashCombine(const Value& val, size_t &seed) const {
  val.hash_combine<int8_t>(seed, val.value_.tinyint);
}

void TinyintType::SerializeTo(const Value& val, SerializeOutput &out) const {
  out.WriteByte(val.value_.tinyint);
  return;
}

void TinyintType::SerializeTo(const Value& val, char *storage, bool inlined UNUSED_ATTRIBUTE,
    VarlenPool *pool UNUSED_ATTRIBUTE) const {

  *reinterpret_cast<int8_t *>(storage) = val.value_.tinyint;
  return;

}

Value *TinyintType::Copy(const Value& val) const {
  val.CheckInteger();
  return new Value(Type::TINYINT, val.value_.tinyint);
  throw Exception("type error");
}

Value *TinyintType::CastAs(const Value& val, const Type::TypeId type_id) const {
  switch (type_id) {
      case Type::TINYINT: {
        if (val.IsNull())
          return new Value(Type::TINYINT, PELOTON_INT8_NULL);
            return val.Copy();
      }
      case Type::SMALLINT: {
        if (val.IsNull())
          return new Value(Type::SMALLINT, PELOTON_INT16_NULL);
            return new Value(Type::SMALLINT, (int16_t)val.GetAs<int8_t>());

      }
      case Type::INTEGER:
      case Type::PARAMETER_OFFSET: {
        if (val.IsNull())
          return new Value(type_id, PELOTON_INT32_NULL);
            new Value(type_id, (int32_t)val.GetAs<int8_t>());
      }
      case Type::BIGINT: {
        if (val.IsNull())
          return new Value(Type::BIGINT, PELOTON_INT64_NULL);
            return new Value(Type::BIGINT, (int64_t)val.GetAs<int8_t>());
      }
      case Type::DECIMAL: {
        if (val.IsNull())
          return new Value(Type::DECIMAL, PELOTON_DECIMAL_NULL);

            return new Value(Type::DECIMAL, (double)val.GetAs<int8_t>());
      }
      case Type::VARCHAR:
        if (val.IsNull())
          return new Value(Type::VARCHAR, nullptr, 0);
        return new Value(Type::VARCHAR, val.ToString());
      default:
        break;
    }
    throw Exception(Type::GetInstance(val.GetTypeId())->ToString()
        + " is not coercable to "
        + Type::GetInstance(type_id)->ToString());
}

}  // namespace common
}  // namespace peloton
