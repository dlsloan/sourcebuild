/* Copyright (C) 2020 David Sloan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __Base_Integer_h
#define __Base_Integer_h

namespace Base {
  template<bool> struct Range;

  template<int bits, typename = Range<true> >
  class Integer {
    static_assert(bits > 0, "Integer must have at least 1 bit");
  };

  template<int bits>
  class Integer<bits, Range<(0 < bits && bits <= 8)>> {
  public:
    Integer(int8_t value) :
      value_(value) {}

    template<int newBits, typename = Range<true>>
    Integer<newBits> truncate() {
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(0 < newBits && newBits <= 8)>>() {
      return Integer<newBits>(static_cast<int8_t>(static_cast<int8_t>(value_ << (8 - bits)) >> (8 - bits)));
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(8 < newBits && newBits <= 16)>>() {
      return Integer<newBits>(static_cast<int16_t>(static_cast<int8_t>(value_ << (8 - bits)) >> (8 - bits)));
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(16 < newBits && newBits <= 32)>>() {
      return Integer<newBits>(static_cast<int32_t>(static_cast<int8_t>(value_ << (8 - bits)) >> (8 - bits)));
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(32 < newBits && newBits <= 64)>>() {
      return Integer<newBits>(static_cast<int64_t>(static_cast<int8_t>(value_ << (8 - bits)) >> (8 - bits)));
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(32 < newBits && newBits <= 64)>>() {
      return Integer<newBits>(static_cast<int64_t>(static_cast<int8_t>(value_ << (8 - bits)) >> (8 - bits)));
    }
  private:
    int8_t value_;
  };

  template<int bits>
  class Integer<bits, Range<(8 < bits && bits <= 16)>> {
  public:
    Integer(int16_t value) :
      value_(value) {}
  private:
    int16_t value_;
  };

  template<int bits>
  class Integer<bits, Range<(16 < bits && bits <= 32)>> {
  public:
    Integer(int32_t value) :
      value_(value) {}
  private:
    int32_t value_;
  };

  template<int bits>
  class Integer<bits, Range<(32 < bits && bits <= 64)>> {
  public:
    Integer(int64_t value) :
      value_(value) {}
  private:
    int64_t value_;
  };

  template<int bits>
  class Integer<bits, Range<(64 < bits)>> {
  public:
    Integer(int64_t value) :
      value_ {}
    {
      value_[0] = value;
      if (value < 0)
      {
        for (uint64_t i = 1; i < BLOCK_COUNT; ++i)  
          value_[i] = -1;
      }
      else
      {
        for (uint64_t i = 1; i < BLOCK_COUNT; ++i)  
          value_[i] = 0;
      }
    }

    template<int newBits, typename = Range<true>>
    Integer<newBits> truncate() {
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(0 < newBits && newBits <= 8)>>() {
      return Integer<newBits> { static_cast<int8_t>(value_[0]) };
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(8 < newBits && newBits <= 16)>>() {
      return Integer<newBits> { static_cast<int16_t>(value_[0]) };
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(16 < newBits && newBits <= 32)>>() {
      return Integer<newBits> { static_cast<int32_t>(value_[0]) };
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(32 < newBits && newBits <= 64)>>() {
      return Integer<newBits> { static_cast<int64_t>(value_[0]) };
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(64 < newBits && newBits <= bits)>>() {
      constexpr uint64_t blockCount = (newBits - 1) / 64 + 1;
      int64_t value[blockCount];
      for (uint64_t i = 0; i < blockCount; ++i)
        value[i] = value_[i];
      return Integer<newBits>(value);
    }
    template<int newBits>
    Integer<newBits> truncate<newBits, Range<(bits < newBits)>>() {
      constexpr uint64_t blockCount = (newBits - 1) / 64 + 1;
      int64_t value[blockCount];
      for (uint64_t i = 0; i < BLOCK_COUNT; ++i)
        value[i] = value_[i];
      //TODO: propper calcs (last bits sign extend...)
      value[BLOCK_COUNT - 1] = (value[BLOCK_COUNT - 1] << (64 - bits)) >> (64 - bits);

      if (value[BLOCK_COUNT - 1] < 0)
      {
        for (uint64_t i = BLOCK_COUNT; i < blockCount; ++i)
          value[i] = -1;
      }
      else
      {
        for (uint64_t i = BLOCK_COUNT; i < blockCount; ++i)
          value[i] = 0;
      }
      return Integer<newBits>(value);
    }
  private:
    const uint64_t BLOCK_COUNT = (bits - 1) / 64 + 1;
    int64_t[BLOCK_COUNT] value_;
  };
}

#endif // !__Base_Integer_h


      // inline Integer operator + (Integer const& rhs) const { return Integer(this->value_ + rhs.value_); }
      // inline Integer operator += (Integer const& rhs) { this->value_ += rhs.value_; return (*this); }
      // inline Integer operator ++ () { ++this->value_; return (*this); }
      // inline Integer operator ++ (int) { Integer temp(value_); ++(*this); return temp; }
      // inline Integer operator - (Integer const& rhs) const { return Integer(this->value_ - rhs.value_); }
      // inline Integer operator -= (Integer const& rhs) { this->value_ -= rhs.value_; return (*this);  }
      // inline Integer operator -- () { --this->value_; return (*this); }
      // inline Integer operator -- (int) { Integer temp(value_); --(*this); return temp; }
      // inline Integer operator * (Integer const& rhs) const { return Integer(this->value_ * rhs.value_); }
      // inline Integer operator *= (Integer const& rhs) { this->value_ *= rhs.value_; return (*this); }
      // inline Integer operator / (Integer const& rhs) const { return Integer(this->value_ / rhs.value_); }
      // inline Integer operator /= (Integer const& rhs) { this->value_ /= rhs.value_; return (*this); }
      // inline Integer operator % (Integer const& rhs) const { return Integer(this->value_ % rhs.value_); }
      // inline Integer operator %= (Integer const& rhs) { this->value_ %= rhs.value_; return (*this); }
      // inline Integer operator << (Integer const& rhs) const { return Integer(this->value_ << rhs.value_); }
      // inline Integer operator <<= (Integer const& rhs) { this->value_ <<= rhs.value_; return (*this); }
      // inline Integer operator >> (Integer const& rhs) const { return Integer(this->value_ >> rhs.value_); }
      // inline Integer operator >>= (Integer const& rhs) { this->value_ >>= rhs.value_; return (*this); }
      // inline Integer operator & (Integer const& rhs) const { return Integer(this->value_ & rhs.value_); }
      // inline Integer operator &= (Integer const& rhs) { this->value_ &= rhs.value_; return (*this); }
      // inline Integer operator | (Integer const& rhs) const { return Integer(this->value_ | rhs.value_); }
      // inline Integer operator |= (Integer const& rhs) { this->value_ |= rhs.value_; return (*this); }
      // inline Integer operator ^ (Integer const& rhs) const { return Integer(this->value_ ^ rhs.value_); }
      // inline Integer operator ^= (Integer const& rhs) { this->value_ ^= rhs.value_; return (*this); }
      // inline Integer operator ~ () { this->value_ = ~this->value_; return (*this); }
      // inline bool operator == (Integer const& rhs) const { return this->value_ == rhs.value_; }
      // inline bool operator != (Integer const& rhs) const { return this->value_ != rhs.value_; }
      // inline bool operator > (Integer const& rhs) const { return this->value_ > rhs.value_; }
      // inline bool operator >= (Integer const& rhs) const { return this->value_ >= rhs.value_; }
      // inline bool operator < (Integer const& rhs) const { return this->value_ < rhs.value_; }
      // inline bool operator <= (Integer const& rhs) const { return this->value_ <= rhs.value_; }

