#include <cstddef>
#include <utility>
#include <memory>
#include <tuple>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    std::tuple<T*, Deleter> data;

    UniquePtr() noexcept  {
        std::get<0>(data) = nullptr;
    }

    UniquePtr(T * x, const Deleter& del = Deleter()) noexcept {
        data = std::make_tuple(x, del);
    }

    UniquePtr(UniquePtr&& x) noexcept {
        data = std::make_tuple(std::get<0>(x.data), std::get<1>(x.data));
        std::get<0>(x.data) = nullptr;
    }

    UniquePtr operator=(UniquePtr&& other) noexcept {
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = std::get<0>(other.data);
        std::get<0>(other.data) = nullptr;
        return std::move(other);
    }

    UniquePtr operator=(std::nullptr_t other) noexcept {
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = nullptr;
        return std::move(other);
    }

    const T& operator*() const {
        return *std::get<0>(data);
    }

    T& operator*() {
        return *std::get<0>(data);
    }

    const T* operator->() const noexcept {
        return std::get<0>(data);
    }

    void reset(T* x) noexcept {
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = x;
    }

    T* release() noexcept {
       T * result = std::get<0>(data);
       std::get<0>(data) = nullptr;
       return result;
    }

    void swap(UniquePtr& other) noexcept {
        T * tmp = std::get<0>(other.data);
        std::get<0>(other.data) = std::get<0>(data);
        std::get<0>(data) = tmp;
    }

    operator bool() const noexcept {
        return (std::get<0>(data) != nullptr);
    }

    T * get() const noexcept {
        return std::get<0>(data);
    }

    Deleter& get_deleter() {
        return std::get<1>(data);
    }

    const Deleter& get_deleter() const {
        return std::get<1>(data);
    }

    ~UniquePtr() noexcept {
        std::get<1>(data)(std::get<0>(data));
    }
};
