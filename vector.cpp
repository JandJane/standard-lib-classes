#include <iostream>

template <typename T>
struct VectorBase {
    T * data;
    size_t cp;  // capacity

    VectorBase(const size_t n) {
        cp = n;
        data = static_cast<T*>(operator new(n * sizeof(T)));
    }

    void swap(const VectorBase<T>& other) {
        cp = other.cp;
        data = other.data;
    }
};

template <typename T>
class Vector {
private:
    VectorBase<T> base;
    size_t sz;  // size

    void realocate(size_t n) {
        VectorBase<T> tmp = VectorBase<T>(n);
        size_t i;
        try {
            for (i = 0; i != sz; ++i) {
                new (tmp.data + i) T(base.data[i]);
            }
            for (size_t i = 0; i != sz; ++i) {
                base.data[i].~T();
            }
            operator delete(base.data);
            base.swap(tmp);
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                tmp.data[j].~T();
            }
            operator delete(tmp.data);
        }
    }

    void realocate_and_fill(size_t n) {
        VectorBase<T> tmp = VectorBase<T>(n);
        size_t i;
        try {
            for (i = 0; i != sz; ++i) {
                new (tmp.data + i) T(base.data[i]);
            }
            for (size_t i = sz; i != n; ++i) {
                    new (tmp.data + i) T();
                }
            for (size_t i = 0; i != sz; ++i) {
                base.data[i].~T();
            }
            operator delete(base.data);
            base.swap(tmp);
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                tmp.data[j].~T();
            }
            operator delete(tmp.data);
        }
    }

public:
    Vector() : base(0) {
        sz = 0;
    }

    Vector(size_t n, T val) : base(n), sz(n) {
        size_t i;
        try {
            for (i = 0; i != n; ++i) {
                new (base.data + i) T(val);
            }
            sz = n;
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                base.data[j].~T();
            }
            operator delete (base.data);
        }
    }

    Vector(const Vector<T>& other) {
        size_t i;
        for (size_t i = 0; i != sz; ++i) {
            base.data[i].~T();
        }
        base = VectorBase<T>(other.sz);
        try {
            for (i = 0; i != other.sz; ++i) {
                new (base.data + i) T(other.base.data[i]);  // static cast или как
            }
            sz = other.sz;
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                base.data[j].~T();
            }
        }
    }

    Vector operator=(const Vector<T>& other) {
        size_t i;
        int tmp_sz = other.sz;
        VectorBase<T> tmp = VectorBase<T>(tmp_sz);
        try {
                for (i = 0; i != tmp_sz; ++i) {
                    new (tmp.data + i) T(other.base.data[i]);
                }
                for (size_t i = 0; i != sz; ++i) {
                    base.data[i].~T();
                }
                base.swap(tmp);
                sz = tmp_sz;
        } catch (...) {
                for (size_t j = 0; j != i; ++j) {
                    tmp.data[j].~T();
                }
                operator delete(tmp.data);
        }
        return *this;
    }

    void resize(size_t n) {
        if (n > sz) {
            if (n > base.cp) {
                realocate_and_fill(n);
            } else {
                int i = sz;
                try {
                    while (i != n) {
                        new (base.data + (i++)) T();
                    }
                } catch (...) {}
            }
        } else {
            for (size_t i = n; i != sz; ++i) {
                base.data[i].~T();
            }
        }
        sz = n;
    }

    void pop_back() {
        if (sz != 0) {
            --sz;
            base.data[sz].~T();
        }
    }

    void Print() const {
        for (size_t i = 0; i != sz; ++i) {
            std::cout << base.data[i] << " ";
        }
        std::cout << std::endl;
    }

    void push_back(const T& val) {
        if (base.cp > sz) {
                new (base.data + (sz++)) T(val);
        } else {
            if (sz == 0) {
                realocate(1);
            } else {
                realocate(base.cp * 2);
            }
            new (base.data + (sz++)) T(val);
        }
    }

    void push_back(T&& val) {
        if (base.cp > sz) {
            new (base.data + (sz++)) T(std::move(val));
        } else {
            if (sz == 0) {
                realocate(1);
            } else {
                realocate(base.cp * 2);
            }
            new (base.data + (sz++)) T(std::move(val));
        }
    }

    const T& operator[] (size_t i) const {
        return base.data[i];
    }

    T& operator[] (size_t i) {
        return base.data[i];
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return base.cp;
    }

    void reverse(size_t new_cp) {
        if (new_cp >= base.cp) {
            VectorBase<T> tmp = VectorBase<T>(new_cp);
            int i;
            try {
                for (i = sz - 1; i >= 0; --i) {
                    new (tmp.data + sz - i - 1) T(base.data[i]);
                }
                for (size_t i = 0; i != sz; ++i) {
                    base.data[i].~T();
                }
                operator delete(base.data);
                base.swap(tmp);
            } catch (...) {
                for (int j = sz - 1; j != i; --j) {
                    tmp.data[sz - j - 1].~T();
                }
                operator delete(tmp.data);
            }
        }
    }

    void reserve(size_t new_cp) {
        if (new_cp > base.cp) {
            realocate(new_cp);
        }
    }

    void clear() {
        for (size_t i = 0; i != sz; ++i) {
            base.data[i].~T();
        }
        sz = 0;
    }

    T* begin() {
        return (base.data);
    }

    T* end() {
        return (base.data + sz);
    }

    void swap(Vector<T>& other) {
        int tmp_sz = other.sz;
        VectorBase<T> tmp_base = other.base;
        other.sz = sz;
        other.base = base;
        base = tmp_base;
        sz = tmp_sz;
    }

    ~Vector() {
        for (size_t i = 0; i != sz; ++i) {
            base.data[i].~T();
        }
        operator delete(base.data);
    }
};
