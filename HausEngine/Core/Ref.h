#pragma once

#include <memory>
#include <utility>

namespace HausEngine {
    template<typename T>
    class Ref {
    public:
        explicit Ref(std::shared_ptr<T> ptr) : ptr_(ptr) {}

        Ref() = default;

        T* operator->() const { return ptr_.get(); }
        T& operator*() const { return *ptr_; }

        template<typename... Args>
        static Ref<T> Create(Args&&... args) {
            return Ref<T>(std::make_shared<T>(std::forward<Args>(args)...));
        }

    private:
        std::shared_ptr<T> ptr_;
    };
}
