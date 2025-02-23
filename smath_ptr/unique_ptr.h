#pragma once
// #include <memory>
namespace dom
{
    template <typename T>
    class UniquePtr
    {
    public:
        UniquePtr() : data_(nullptr) {}
        UniquePtr(T *data) : data_(data) {}
        UniquePtr(UniquePtr<T> &&other) noexcept : data_(other.data_)
        {
            other.data_ = nullptr;
        }

        ~UniquePtr()
        {
            delete data_;
            data_ = nullptr;
        }

        UniquePtr &operator=(UniquePtr<T> &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            data_ = other.data_;
            other.data_ = nullptr;
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
            return data_ != nullptr;
        }

        T *get() const
        {
            return data_;
        }

        void reset(T *data = nullptr)
        {
            if (data_)
            {
                delete data_;
            }
            data_ = data;
        }
        T *release()
        {
            auto data = data_;
            data_ = nullptr;
            return data;
        }

        void swap(UniquePtr<T> &other)
        {
            if (&other == this)
            {
                return;
            }
            std::swap(data_, other.data_);
        }

    private:
        T *data_;
    };
    template <typename T, typename... Args>
    UniquePtr<T> make_unique(Args&& ...args) {
        return UniquePtr<T>(new T(std::forward<Args>(args)...));
    }
} // namespace dom