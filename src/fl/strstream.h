#pragma once

#include "crgb.h"
#include "str.h"

#ifndef FASTLED_STRSTREAM_USES_SIZE_T
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
#define FASTLED_STRSTREAM_USES_SIZE_T 0
#else
#define FASTLED_STRSTREAM_USES_SIZE_T 1
#endif
#endif

namespace fl {

class Tile2x2_u8;

template <typename T> struct StrStreamHelper {
    static void append(string &str, const T &n) { str.append(n); }
};

template <> struct StrStreamHelper<int> {
    static void append(string &str, const int &n) { str.append(int32_t(n)); }
};

template <> struct StrStreamHelper<uint8_t> {
    static void append(string &str, const uint8_t &n) { str.append(uint16_t(n)); }
};

template <> struct StrStreamHelper<char> {
    static void append(string &str, const char &n) { str.append(uint16_t(n)); }
};

template <> struct StrStreamHelper<unsigned int> {
    static void append(string &str, const unsigned int &n) {
        str.append(uint32_t(n));
    }
};

class StrStream {
  public:
    StrStream() = default;
    StrStream(const string &str) : mStr(str) {}

    void setTreatCharAsInt(bool treatCharAsInt) {
        mTreatCharAsInt = treatCharAsInt;
    }

    const string &str() const { return mStr; }
    const char *c_str() const { return mStr.c_str(); }

    StrStream &operator<<(const CRGB &rgb) {
        mStr.append(rgb);
        return *this;
    }
    StrStream &operator<<(const StrStream &strStream) {
        mStr.append(strStream.str());
        return *this;
    }

    StrStream &operator<<(const Tile2x2_u8 &subpixel);

    StrStream &operator=(const uint16_t &n) {
        mStr.clear();
        (*this) << n;
        return *this;
    }

    StrStream &operator=(const uint8_t &n) {
        mStr.clear();
        (*this) << n;
        return *this;
    }

    StrStream &operator=(char c) {
        mStr.clear();
        (*this) << c;
        return *this;
    }

    // << operator section
    StrStream &operator<<(const string &str) {
        mStr.append(str);
        return *this;
    }

    StrStream &operator<<(const char *str) {
        mStr.append(str);
        return *this;
    }

    StrStream &operator<<(const float &f) {
        // multiply by 100 and round to get 2 decimal places
        mStr.append(f);
        return *this;
    }

    StrStream &operator<<(const double &f) {
        // multiply by 100 and round to get 2 decimal places
        mStr.append(f);
        return *this;
    }

    StrStream &operator<<(const char &c) {
        if (mTreatCharAsInt) {
            StrStreamHelper<int>::append(mStr, c);
        } else {
            StrStreamHelper<char>::append(mStr, c);
        }
        return *this;
    }

#if FASTLED_STRSTREAM_USES_SIZE_T
    StrStream &operator<<(size_t n) {
        mStr.append(uint32_t(n));
        return *this;
    }
#endif

    template <typename T> StrStream &operator<<(T n) {
        StrStreamHelper<T>::append(mStr, n);
        return *this;
    }

    StrStream &operator<<(const uint8_t &n) {
        if (mTreatCharAsInt) {
            mStr.append(uint16_t(n));
        } else {
            mStr.append(n);
        }
        return *this;
    }

    StrStream &operator<<(const uint16_t &n) {
        mStr.append(n);
        return *this;
    }

    StrStream &operator<<(const int16_t &n) {
        mStr.append(n);
        return *this;
    }

    StrStream &operator<<(const uint32_t &n) {
        mStr.append(uint32_t(n));
        return *this;
    }

    StrStream &operator<<(const int32_t &n) {
        mStr.append(n);
        return *this;
    }

    // assignment operator completely replaces the current string
    StrStream &operator=(const string &str) {
        mStr = str;
        return *this;
    }

    StrStream &operator=(const char *str) {
        mStr.clear();
        mStr.append(str);
        return *this;
    }

    // crgb
    StrStream &operator=(const CRGB &rgb) {
        mStr.clear();
        (*this) << rgb;
        return *this;
    }

    void clear() { mStr.clear(); }

  private:
    string mStr;
    bool mTreatCharAsInt = true;
};

class FakeStrStream {
  public:
    template <typename T> FakeStrStream &operator<<(const T &) { return *this; }

    FakeStrStream &operator<<(const char *) { return *this; }

    template <typename T> FakeStrStream &operator=(const T &) { return *this; }

    FakeStrStream &operator<<(const CRGB &) { return *this; }
    FakeStrStream &operator<<(const string &) { return *this; }
    FakeStrStream &operator<<(char) { return *this; }

#if FASTLED_STRSTREAM_USES_SIZE_T
    FakeStrStream &operator<<(size_t) { return *this; }
#endif

    FakeStrStream &operator<<(uint8_t) { return *this; }
    FakeStrStream &operator<<(uint16_t) { return *this; }
    FakeStrStream &operator<<(int16_t) { return *this; }
    FakeStrStream &operator<<(uint32_t) { return *this; }
    FakeStrStream &operator<<(int32_t) { return *this; }

    FakeStrStream &operator=(const string &) { return *this; }
    FakeStrStream &operator=(const CRGB &) { return *this; }
    FakeStrStream &operator=(uint16_t) { return *this; }
    FakeStrStream &operator=(uint8_t) { return *this; }
    FakeStrStream &operator=(char) { return *this; }
};

} // namespace fl
