#include <iostream>

template <typename T>
class UniquePtr {
   public:
    UniquePtr(T* ptr) : ptr_{ptr} {}

    UniquePtr() : ptr_{nullptr} {}

    UniquePtr(const UniquePtr& p) = delete;
    UniquePtr& operator=(const UniquePtr& p) = delete;

    UniquePtr(UniquePtr&& p) {
        this->ptr_ = p.ptr_;
        p.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& p) {
        clean();
        this->ptr_ = p.ptr_;
        p.ptr_ = nullptr;
        return *this;
    }

    T* get() const { return ptr_; }

    T* operator->() const { return ptr_; }

    T& operator*() const { return *ptr_; }

    operator bool() const { return ptr_; }

    ~UniquePtr() { clean(); }

   private:
    void clean() {
        if (ptr_) delete ptr_;
    }

    T* ptr_;
};

struct A {
    A() { std::cout << "A() \n"; }
    ~A() { std::cout << "~A() \n"; }
};

int main() {
    A* a = new A;
    UniquePtr<A> ptr(a);
    UniquePtr<A> b(std::move(ptr));
}
