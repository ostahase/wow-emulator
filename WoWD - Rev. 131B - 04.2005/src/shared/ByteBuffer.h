// Copyright (c) quetzal
// Copyright (c) WoWD Team

// ChangeLog
//     25/09/2004 (Ignatich):
//       fixed few portability issues.
//       added a copy constructors.
//       removed some functions we'll never need.

#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include "Common.h"

class ByteBuffer {
public:
    class error {
    };

    const static size_t DEFAULT_SIZE = 0x1000;

    ByteBuffer(): _rpos(0), _wpos(0) {
        _storage.reserve(DEFAULT_SIZE);
    }
    ByteBuffer(size_t res): _rpos(0), _wpos(0) {
        _storage.reserve(res);
    }
    ByteBuffer(const ByteBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }

    void clear() {
        _storage.clear();
        _rpos = _wpos = 0;
    }

    //template <typename T> void insert(size_t pos, T value) {
    //  insert(pos, (uint8 *)&value, sizeof(value));
    //}
    template <typename T> void append(T value) {
        append((uint8 *)&value, sizeof(value));
    }
    template <typename T> void put(size_t pos,T value) {
        put(pos,(uint8 *)&value,sizeof(value));
    }

    // stream like operators for storing data
    ByteBuffer &operator<<(bool value) {
        append<char>((char)value);
        return *this;
    }
    ByteBuffer &operator<<(uint8 value) {
        append<uint8>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint16 value) {
        append<uint16>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint32 value) {
        append<uint32>(value);
        return *this;
    }
    ByteBuffer &operator<<(uint64 value) {
        append<uint64>(value);
        return *this;
    }
    ByteBuffer &operator<<(float value) {
        append<float>(value);
        return *this;
    }
    ByteBuffer &operator<<(double value) {
        append<double>(value);
        return *this;
    }
    ByteBuffer &operator<<(const std::string &value) {
        append((uint8 *)value.c_str(), value.length());
        append((uint8)0);
        return *this;
    }
    ByteBuffer &operator<<(const char *str) {
        append((uint8 *)str, strlen(str));
        append((uint8)0);
        return *this;
    }

    // stream like operators for reading data
    ByteBuffer &operator>>(bool &value) {
        value = read<char>() > 0 ? true : false;
        return *this;
    }
    ByteBuffer &operator>>(uint8 &value) {
        value = read<uint8>();
        return *this;
    }
    ByteBuffer &operator>>(uint16 &value) {
        value = read<uint16>();
        return *this;
    }
    ByteBuffer &operator>>(uint32 &value) {
        value = read<uint32>();
        return *this;
    }
    ByteBuffer &operator>>(uint64 &value) {
        value = read<uint64>();
        return *this;
    }
    ByteBuffer &operator>>(float &value) {
        value = read<float>();
        return *this;
    }
    ByteBuffer &operator>>(double &value) {
        value = read<double>();
        return *this;
    }
    ByteBuffer &operator>>(std::string& value) {
        value.clear();
        while (true) {
            char c=read<char>();
            if (c==0)
                break;
            value+=c;
        }
        return *this;
    }

    uint8 operator[](size_t pos) {
        return read<uint8>(pos);
    }


    size_t rpos() {
        return _rpos;
    };

    size_t rpos(size_t rpos) {
        _rpos = rpos;
        return _rpos;
    };

    size_t wpos() {
        return _wpos;
    }

    size_t wpos(size_t wpos) {
        _wpos = wpos;
        return _wpos;
    }

    template <typename T> T read() {
        T r=read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    };
    template <typename T> T read(size_t pos) const {
        ASSERT(pos + sizeof(T) <= size());
        return *((T*)&_storage[pos]);
    }

    void read(uint8 *dest, size_t len) {
        if (_rpos + len <= size()) {
            memcpy(dest, &_storage[_rpos], len);
        } else {
            throw error();
        }
        _rpos += len;
    }

    const uint8 *contents() const { return &_storage[0]; };

    inline size_t size() const { return _storage.size(); };
    // one should never use resize probably
    void resize(size_t newsize) {
        _storage.resize(newsize);
        _rpos = 0;
        _wpos = size();
    };
    void reserve(size_t ressize) {
        if (ressize > size()) _storage.reserve(ressize);
    };

        // appending to the end of buffer
    void append(const std::string& str) {
        append((uint8 *)str.c_str(),str.size() + 1);
    }
    void append(const char *src, size_t cnt) {
        return append((const uint8 *)src, cnt);
    }
    void append(const uint8 *src, size_t cnt) {
        if (!cnt) return;

        // noone should even need uint8buffer longer than 10mb
        // if you DO need, think about it
        // then think some more
        // then use something else
        // -- qz
        ASSERT(size() < 10000000);

        if (_storage.size() < _wpos + cnt)
            _storage.resize(_wpos + cnt);
        memcpy(&_storage[_wpos], src, cnt);
        _wpos += cnt;
    }
    void append(const ByteBuffer& buffer) {
        append(buffer.contents(),buffer.size());
    }

    void put(size_t pos, const uint8 *src, size_t cnt) {
        ASSERT(pos + cnt <= size());
        memcpy(&_storage[pos], src, cnt);
    }
    //void insert(size_t pos, const uint8 *src, size_t cnt) {
    //  std::copy(src, src + cnt, inserter(_storage, _storage.begin() + pos));
    //}

protected:
    // read and write positions
    size_t _rpos, _wpos;
    std::vector<uint8> _storage;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); i++) {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); i++) {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++) {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while(msize--) {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

#endif
