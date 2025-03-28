#ifndef RESULT_H
#define RESULT_H

#include <functional>
#include <string>

#include <try.h>

#define IF_LET_OK(value, result) for(auto value : result._ok_iterator_())
#define IF_LET_ERR(value, result) for(auto value : result._err_iterator_())

#ifndef OPTION_H
template <typename T>
class Option;
#endif

template <typename T, typename E>
class Result {
private:
    template <typename U, typename F>
    union ResultInner {
        U ok;
        F err;

        ResultInner(U u) : ok(u) {}
        ResultInner(F f) : err(f) {}
    };
    
    enum ResultVariant {
        This, That
    };

    ResultInner<T, E> inner;
    ResultVariant variant;

    Result(T ok) : inner(ResultInner<T, E>(ok)), variant(ResultVariant::This) {}
    Result(E err) : inner(ResultInner<T, E>(err)), variant(ResultVariant::That) {}
public:
    ~Result() {
        switch (this->variant) {
        case ResultVariant::This:
            this->inner.ok.~T();
            break;
        case ResultVariant::That:
            this->inner.err.~E();
            break;
        default:
            std::__throw_invalid_argument("unreachable!");
        }
    }
    
    static Result<T, E> Ok(T ok) noexcept {
        return Result<T, E>(ok);
    }

    static Result<T, E> Err(E err) noexcept {
        return Result<T, E>(err);
    }

    T unwrap() const {
        if (this->is_ok()) {
            return this->inner.ok;
        }
        throw std::bad_optional_access();
    }

    T unwrap_or(T or_value) const noexcept {
        if (this->is_ok()) {
            return this->inner.ok;
        }
        return or_value;
    }

    T unwrap_or_else(std::function<T()> or_fn) const noexcept {
        if (this->is_ok()) {
            return this->inner.ok;
        }
        return or_fn();
    }

    E unwrap_err() const {
        if (this->is_err()) {
            return this->inner.err;
        }
        throw std::bad_optional_access();
    }

    E unwrap_err_or(E or_value) const noexcept {
        if (this->is_err()) {
            return this->inner.err;
        }
        return or_value;
    }

    E unwrap_err_or_else(std::function<E()> or_fn) const noexcept {
        if (this->is_err()) {
            return this->inner.err;
        }
        return or_fn();
    }
    T expect(std::string msg) const {
        if (this->is_ok()) {
            return this->inner.ok;
        }
        throw std::bad_optional_access(msg);
    }

    E expect_err(std::string msg) const {
        if (this->is_err()) {
            return this->inner.err;
        }
        throw std::bad_optional_access(msg);
    }

    bool is_ok() const noexcept {
        return this->variant == ResultVariant::This;
    }

    bool is_ok_and(std::function<bool(T)> predicate) const noexcept {
        return this->is_ok() && predicate(this->inner.ok);
    }

    bool is_err() const noexcept {
        return this->variant == ResultVariant::That;
    }

    bool is_err_and(std::function<bool(E)> predicate) const noexcept {
        return this->is_err() && predicate(this->inner.err);
    }

    template <typename U>
    Result<U, E> map(std::function<U(T)> map_fn) const noexcept {
        if (this->is_ok()) {
            return Result<U, E>::Ok(map_fn(this->inner.ok));
        }
        return Result<U, E>::Err(this->inner.err);
    }

    template <typename U>
    U map_or(std::function<U(T)> map_fn, U or_value) const noexcept {
        if (this->is_ok()) {
            return map_fn(this->inner.ok);
        }
        return or_value;
    }
    
    template <typename U>
    U map_or_else(std::function<U(T)> map_fn, std::function<U()> or_fn) const noexcept {
        if (this->is_ok()) {
            return map_fn(this->inner.ok);
        }
        return or_fn();
    }
    
    template <typename F>
    Result<T, F> map_err(std::function<F(E)> map_fn) const noexcept {
        if (this->is_err()) {
            return Result<T, F>::Err(map_fn(this->inner.err));
        }
        return Result<T, F>::Ok(this->inner.ok);
    }
    
    template <typename F>
    F map_err_or(std::function<F(E)> map_fn, F or_value) const noexcept {
        if (this->is_err()) {
            return map_fn(this->inner.err);
        }
        return or_value;
    }
    
    template <typename F>
    F map_err_or_else(std::function<F(E)> map_fn, std::function<F()> or_fn) const noexcept {
        if (this->is_err()) {
            return map_fn(this->inner.err);
        }
        return or_fn();
    }

    Option<T> ok() const noexcept {
        if (this->is_ok()) {
            return Option<T>::Some(this->inner.ok);
        }
        return Option<T>::None();
    }

    Option<E> err() const noexcept {
        if (this->is_err()) {
            return Option<T>::Some(this->inner.err);
        }
        return Option<T>::None();
    }

    struct __ok_iterator__ {
    public:
        Result<T, E> &result;

        __ok_iterator__(Result<T, E> &result) : result(result) {}

        T* begin() {
            if (this->result.is_ok()) {
                return &this->result.inner.ok;
            }
            return this->end();
        }

        const T* begin() const {
            if (this->result.is_ok()) {
                return &this->result.inner.ok;
            }
            return this->end();
        }

        T* end() {
            return (&this->result.inner.ok) + 1;
        }

        const T* end() const {
            return (&this->result.inner.ok) + 1;
        }
    };

    Result<T, E>::__ok_iterator__ _ok_iterator_() noexcept {
        return __ok_iterator__(*this);
    }

    struct __error_iterator__ {
    public:
        Result<T, E> &result;

        __error_iterator__(Result<T, E> &result) : result(result) {}

        T* begin() {
            if (this->result.is_err()) {
                return &this->result.inner.err;
            }
            return this->end();
        }

        const T* begin() const {
            if (this->result.is_err()) {
                return &this->result.inner.err;
            }
            return this->end();
        }

        T* end() {
            return (&this->result.inner.err) + 1;
        }

        const T* end() const {
            return (&this->result.inner.err) + 1;
        }
    };

    Result<T, E>::__error_iterator__ _error_iterator_() noexcept {
        return __error_iterator__(*this);
    }

    T& operator() (std::tuple<none_return_buf, Option<Result<T, E>>>& buf) {
        if (this->is_err()) {
            std::get<1>(buf) = Option<Result<T, E>>::Some(*this);
            std::longjmp(std::get<0>(buf).buf, true);
        }
        return this->inner.ok;
    }
};

#include <option.h>

#endif