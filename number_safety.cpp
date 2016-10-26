#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits>
#include <iostream>

struct unsigned_tag {};

struct signed_tag {};

template <typename T>
struct checked_trails;

template <typename T, bool>
struct checked_tag;

template <typename T>
struct checked_tag<T, true> {
    typedef signed_tag tag;
};

template <typename T>
struct checked_tag<T, false> {
    typedef unsigned_tag tag;
};

template <typename T>
struct checked_trails {
    typedef typename checked_tag<T, std::numeric_limits<T>::is_signed>::tag tag;
};

template <typename K>
struct checked_number {
private:
    K val;
public:

    void print() {
        std::cout << this->val;
    }
    checked_number(K val) {
        this->val = val;
    }
    template<typename T>
    friend checked_number<T> operator+(checked_number<T> const&, checked_number<T> const&);
    template<typename T>
    friend checked_number<T> operator-(checked_number<T> const&, checked_number<T> const&);
    template<typename T>
    friend checked_number<T> operator/(checked_number<T> const&, checked_number<T> const&);
    template<typename T>
    friend checked_number<T> operator*(checked_number<T> const&, checked_number<T> const&);

    template<typename T>
    friend checked_number<T> add(checked_number<T> const&, checked_number<T> const&, signed_tag);
    template<typename T>
    friend checked_number<T> add(checked_number<T> const&, checked_number<T> const&, unsigned_tag);
    template<typename T>
    friend checked_number<T> sub(checked_number<T> const&, checked_number<T> const&, signed_tag);
    template<typename T>
    friend checked_number<T> sub(checked_number<T> const&, checked_number<T> const&, unsigned_tag);
    template<typename T>
    friend checked_number<T> mul(checked_number<T> const&, checked_number<T> const&, signed_tag);
    template<typename T>
    friend checked_number<T> mul(checked_number<T> const&, checked_number<T> const&, unsigned_tag);
    template<typename T>
    friend checked_number<T> div(checked_number<T> const&, checked_number<T> const&, signed_tag);
    template<typename T>
    friend checked_number<T> div(checked_number<T> const&, checked_number<T> const&, unsigned_tag);


    template<typename T>
    checked_number<T> unary(signed_tag) const {
        assert(this->val == std::numeric_limits<T>::min());
        return checked_number<T>(-this->val);
    }
    template<typename T>
    checked_number<T> unary(unsigned_tag) const {
        assert(this->val == 0);
        return checked_number(0);
    }

    template<typename T>
    checked_number<T> operator-() const {
        return unary(typename checked_trails<T>::tag());
    }
};
    template<typename T>
    checked_number<T> operator+(checked_number<T> const& left,checked_number<T> const& right )
    {
        return add(left, right, typename checked_trails<T>::tag());
    }

    template<typename T>
    checked_number<T> operator-(checked_number<T> const& left, checked_number<T> const& right)
    {
        return sub(left, right, typename checked_trails<T>::tag());
    }

    template<typename T>
    checked_number<T> operator/(checked_number<T> const& left, checked_number<T> const& right)
    {
        assert(right == 0);
        return div(left, right, typename checked_trails<T>::tag());
    }

    template<typename T>
    checked_number<T> operator*(checked_number<T> const& left, checked_number<T> const& right)
    {
        return add(left, right, typename checked_trails<T>::tag());
    }

    template<typename T>
    checked_number<T> add(checked_number<T> const& left, checked_number<T> const& right, signed_tag)
    {
        assert(!(right.val > 0 && std::numeric_limits<T>::max() - right.val < left.val || right.val < 0 && std::numeric_limits<T>::min() - right.val > left.val));
        return checked_number<T>(left.val + right.val);
    }

    template<typename T>
    checked_number<T> add(checked_number<T> const& left, checked_number<T> const& right, unsigned_tag)
    {
        assert(std::numeric_limits<T>::max() - right.val > left.val);
        return checked_number<T>(left.val + right.val);
    }

    template<typename T>
    checked_number<T> sub(checked_number<T> const& left, checked_number<T> const& right, signed_tag)
    {
        assert(!(right.val < 0 && std::numeric_limits<T>::max() + right.val < left.val || right.val > 0 && std::numeric_limits<T>::min() + right.val > left.val));
        return checked_number<T>(left.val - right.val);
    }

    template<typename T>
    checked_number<T> sub(checked_number<T> const& left, checked_number<T> const& right, unsigned_tag)
    {
        assert(right.val <= left.val);
        return checked_number<T>(left.val - right.val);
    }

    template<typename T>
    checked_number<T> mul(checked_number<T> const& left, checked_number<T> const& right, signed_tag)
    {
        assert( !(
                (left.val == 0 || right.val == 0) ||
                (std::numeric_limits<T>::max() / abs(left.val) < abs(right.val))
                ) ||
                ((std::min(left.val, right.val) == std::numeric_limits<T>::min()) &&
                (std::max(left.val, right.val) == -1))
                );
        return checked_number<T>(left.val * right.val);
    }

    template<typename T>
    checked_number<T> mul(checked_number<T> const& left, checked_number<T> const& right, unsigned_tag)
    {
        assert(!(left.val == 0 || right.val == 0) ||
                (std::numeric_limits<T>::max() / abs(left.val) < abs(right.val)));
        return checked_number<T>(left.val * right.val);
    }

    template<typename T>
    checked_number<T> div(checked_number<T> const& left, checked_number<T> const& right, signed_tag)
    {
        assert(!(std::min(left.val, right.val) == std::numeric_limits<T>::min()) &&
                (std::max(left.val, right.val) == -1));
        return checked_number<T>(left.val / right);
    }
    template<typename T>
    checked_number<T> div(checked_number<T> const& left, checked_number<T> const& right, unsigned_tag)
    {
        return checked_number<T>(left.val / right);
    }
int main() {
    checked_number<unsigned int> a = 1;
    a = a - a - a;
    a.print();
}
