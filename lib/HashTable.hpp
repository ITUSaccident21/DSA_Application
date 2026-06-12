#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include "AVL.hpp"
#include "Detail.hpp"

namespace ds {

template <typename Key, typename T, std::size_t BucketCount = 16, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
class HashTable {
private:
    using Entry = std::pair<Key, T>;
    using Bucket = AVL<Entry, std::less<Key>, PairFirst<Key, T>>;

    std::vector<Bucket> buckets_;
    std::size_t size_ = 0;
    Hash hash_{};
    KeyEqual keyEqual_{};

    std::size_t index_for(const Key& key) const {
        return buckets_.empty() ? 0 : static_cast<std::size_t>(hash_(key) % buckets_.size());
    }

public:
    HashTable() : buckets_(BucketCount ? BucketCount : 16) {}

    explicit HashTable(std::size_t bucketCount) : buckets_(bucketCount ? bucketCount : (BucketCount ? BucketCount : 16)) {}

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }
    std::size_t bucket_count() const noexcept { return buckets_.size(); }

    bool insert(const Key& key, const T& value) {
        auto& bucket = buckets_[index_for(key)];
        auto* entry = bucket.find(key);
        if (entry != nullptr) {
            entry->second = value;
            return false;
        }
        bucket.insert(Entry{key, value});
        ++size_;
        return true;
    }

    bool insert(const Key& key, T&& value) {
        auto& bucket = buckets_[index_for(key)];
        auto* entry = bucket.find(key);
        if (entry != nullptr) {
            entry->second = std::move(value);
            return false;
        }
        bucket.insert(Entry{key, std::move(value)});
        ++size_;
        return true;
    }

    T* find(const Key& key) {
        auto& bucket = buckets_[index_for(key)];
        auto* entry = bucket.find(key);
        return entry ? &entry->second : nullptr;
    }

    const T* find(const Key& key) const {
        return const_cast<HashTable*>(this)->find(key);
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    bool erase(const Key& key) {
        auto& bucket = buckets_[index_for(key)];
        if (bucket.remove(key)) {
            --size_;
            return true;
        }
        return false;
    }

    T& operator[](const Key& key) {
        auto& bucket = buckets_[index_for(key)];
        auto* entry = bucket.find(key);
        if (entry == nullptr) {
            bucket.insert(Entry{key, T{}});
            ++size_;
            entry = bucket.find(key);
        }
        return entry->second;
    }

    void clear() {
        for (auto& bucket : buckets_) {
            bucket.clear();
        }
        size_ = 0;
    }
};

} // namespace ds