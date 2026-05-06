#pragma once

#include <type_traits>
#include <utility>
#include <memory>

namespace brstd {

template <typename T, typename U>
class function_ref;

template<typename Ret, typename... Args, typename U>
class function_ref<Ret(Args...), U> {
public:
    function_ref() = default;

    template<typename F>
    function_ref(F&& f) noexcept {
        mObject = (void*)std::addressof(f);
        mCallback = [](void* obj, Args... args) -> Ret {
            return (*(typename std::remove_reference<F>::type*)obj)(std::forward<Args>(args)...);
        };
    }

    Ret operator()(Args... args) const {
        return mCallback(mObject, std::forward<Args>(args)...);
    }

private:
     Ret (*mCallback)(void*, Args...) = nullptr;
     void* mObject = nullptr;
};

}

