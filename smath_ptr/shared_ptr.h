#pragma once
namespace dom
{
  template <typename T>
  class SharePtr
  {
  public:
    SharePtr() : data_(nullptr), use_count_(nullptr) {}
    SharePtr(T *data) : data_(data), use_count_(data ? new int(1) : nullptr) {}
    SharePtr(SharePtr<T> &other) : data_(other.data_), use_count_(other.use_count_)
    {
      if (use_count_)
      {
        (*use_count_)++;
      }
    }
    SharePtr(SharePtr<T> &&other) noexcept : data_(other.data_), use_count_(other.use_count_)
    {
      other.use_count_ = nullptr;
      other.data_ = nullptr;
    }

    ~SharePtr()
    {
      if (!use_count_)
      {
        return;
      }
      if (--(*use_count_) == 0)
      {
        delete data_;
        delete use_count_;
      }
      data_ = nullptr;
      use_count_ = nullptr;
    }

    SharePtr &operator=(const SharePtr<T> &other)
    {
      if (this == &other)
      {
        return *this;
      }
      if (use_count_ && --(*use_count_) == 0)
      {
        delete data_;
        delete use_count_;
      }
      data_ = other.data_;
      use_count_ = other.use_count_;
      if (use_count_)
      {
        (*use_count_)++;
      }
      return *this;
    }

    SharePtr &operator=(SharePtr<T> &&other) noexcept
    {
      if (this == &other)
      {
        return *this;
      }
      if (use_count_ && --(*use_count_) == 0)
      {
        delete data_;
        delete use_count_;
      }
      data_ = other.data_;
      use_count_ = other.use_count_;
      other.data_ = nullptr;
      other.use_count_ = nullptr;
      return *this;
    }

    T *operator->() const
    {
      return data_;
    }

    T &operator*() const
    {
      return *data_;
    }

    explicit operator bool() const
    {
      return data_ != nullptr && use_count_ != nullptr;
    }

    T *get() const
    {
      return data_;
    }

    void reset(T *data = nullptr)
    {
      if (use_count_ && --(*use_count_) == 0)
      {
        delete data_;
        delete use_count_;
      }
      data_ = data;
      use_count_ = data ? new int(1) : nullptr;
    }

    bool unique() const
    {
      return use_count_ && *use_count_ == 1;
    }

    void swap(SharePtr<T> &other)
    {
      if (&other == this)
      {
        return;
      }
      std::swap(data_, other.data_);
      std::swap(use_count_, other.use_count_);
    }

  private:
    T *data_;
    int *use_count_;
  };

  template <typename T, typename... Args>
  SharePtr<T> make_shared(Args&& ...args) {
      return SharePtr<T>(new T(std::forward<Args>(args)...));
  }
} // namespace dom