#ifndef OPTION_H
#define OPTION_H

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>

#include <try.h>

#define IF_LET_SOME(value, option) for(auto value : option._some_iterator_())

#ifndef RESULT_H
template <typename T, typename E>
class Result;
#endif

template <typename T>
class Option {
private:
    template <typename U>
    struct Unit {};

    template <typename U>
    union OptionInner {
        U just;
        Unit<U> nothing;

        OptionInner(U value) : just(value) {}
        OptionInner(Unit<U> value) : nothing(value) {}
    };

    enum OptionVariant {
        Just, Nothing
    };

    OptionVariant variant;
    union OptionInner<T> inner;


    Option(T value) : variant(OptionVariant::Just), inner(OptionInner(value)) {}
public:
    Option() : variant(OptionVariant::Nothing), inner(OptionInner(Unit<T> {})) {}
    ~Option() {}

    static Option<T> Some(T value) {
        return Option<T>(value);
    }
    static Option<T> None() {
        return Option<T>();
    }

    T unwrap() const {
        if (this->is_some()) {
            return this->inner.just;
        }
        throw std::bad_optional_access();
    }

    T expect(std::string msg) const {
        if (this->is_some()) {
            return this->inner.just;
        }
        throw std::bad_optional_access(msg);
    }

    T unwrap_or(const T or_value) const noexcept {
        if (this->is_some()) {
            return this->inner.just;
        }
        return or_value;
    }

    T unwrap_or_else(const std::function<T()> or_fn) const noexcept {
        if (this->is_some()) {
            return this->inner.just;
        }
        return or_fn();
    }

    template <typename U>
    Option<U> map(const std::function<U(T)> map_fn) const noexcept {
        if (this->is_some()) {
            return Some(map_fn(this->inner.just));
        }
        return None();
    }

    template <typename U>
    U map_or(const std::function<U(T)> map_fn, const U or_value) const noexcept {
        if (this->is_some()) {
            return Some(map_fn(this->inner.just));
        }
        return or_value;
    }

    template <typename U>
    U map_or_else(const std::function<U(T)> map_fn, const std::function<U()> or_fn) const noexcept {
        if (is_some()) {
            return Some(map_fn(this->inner.just));
        }
        return or_fn();
    }

    Option<T> filter(const std::function<bool(T)> predicate) const noexcept {
        if (this->is_some() && predicate(this->inner.just)) {
            return Some(this->inner.just);
        }
        return None();
    }

    template<typename U>
    Option<std::tuple<T, U>> zip(const Option<U> other) const noexcept {
        if (this->is_some() && other.is_some()) {
            return Some(std::tuple(this->inner.just, other->inner.just));
        }
        return None();
    }

    template<typename E>
    Result<T, E> ok_or(E err) const noexcept {
        if (this->is_some()) {
            return Result<T, E>::Ok(this->inner.just);
        }
        return Result<T, E>::Err(err);
    }

    template<typename E>
    Result<T, E> ok_or_else(std::function<E()> err_fn) const noexcept {
        if (this->is_some()) {
            return Result<T, E>::Ok(this->inner.just);
        }
        return Result<T, E>::Err(err_fn());
    }

    template<typename U>
    Result<U, T> err_or(U ok) const noexcept {
        if (this->is_some()) {
            return Result<U, T>::Err(this->inner.just);
        }
        return Result<U, T>::Ok(ok);
    }

    template<typename U>
    Result<U, T> err_or_else(std::function<U()> ok_fn) const noexcept {
        if (this->is_some()) {
            return Result<U, T>::Err(this->inner.just);
        }
        return Result<U, T>::Ok(ok_fn());
    }

    bool is_some() const noexcept {
        return this->variant == OptionVariant::Just;
    }

    bool is_some_and(const std::function<bool(T)> predicate) const noexcept {
        return this->is_some() && predicate(this->inner.just);
    }

    bool is_none() const noexcept {
        return this->variant == OptionVariant::Nothing;
    }

    friend std::ostream& operator<<(std::ostream &os, const Option<T> &option) {
        if (option.is_some()) {
            os << "Some(" << option.inner.just << ")";
        } else {
            os << "None";
        }
        return os;
    }

    struct __some_iterator__ {
    public:
        Option<T> &option;

        __some_iterator__(Option<T> &option) : option(option) {}

        T* begin() {
            if (this->option.is_some()) {
                return &this->option.inner.just;
            }
            return this->end();
        }

        const T* begin() const {
            if (this->is_some()) {
                return &this->option.inner.just;
            }
            return this->end();
        }

        T* end() {
            return (&this->option.inner.just) + 1;
        }

        const T* end() const {
            return (&this->option.inner.just) + 1;
        }

    };

    __some_iterator__ _some_iterator_() noexcept {
        return __some_iterator__(*this);
    }

    T& operator() (std::tuple<none_return_buf, Option<Option<T>>>& buf) {
        if (this->is_none()) {
            std::get<1>(buf) = Option<Option<T>>::Some(Option<T>::None());
            std::longjmp(std::get<0>(buf).buf, true);
        }
        return this->inner.just;
    }
};

#include <result.h>

#endif