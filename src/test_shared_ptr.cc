#include <atomic>
#include <iostream>

class SharedCount {
   public:
    SharedCount() : count_{1} {}

    void add() { ++count_; }

    void minus() { --count_; }

    int get() const { return count_; }

   private:
    std::atomic<int> count_;
};

template <typename T>
class SharedPtr {
   public:
    template <typename U>
    friend class SharedPtr;

    template <typename K, typename U>
    friend SharedPtr<K> static_pointer_cast(const SharedPtr<U>& p);

    SharedPtr(T* ptr) : ptr_{ptr}, ref_count_{new SharedCount} {}

    SharedPtr() : ptr_{nullptr}, ref_count_{new SharedCount} {}

    ~SharedPtr() { clean(); }

    SharedPtr(const SharedPtr& p) {
        this->ptr_ = p.ptr_;
        this->ref_count_ = p.ref_count_;
        ref_count_->add();
    }

    SharedPtr(SharedPtr&& p) {
        this->ptr_ = p.ptr_;
        this->ref_count_ = p.ref_count_;
        p.ptr_ = nullptr;
        p.ref_count_ = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& p) {
        clean();
        this->ptr_ = p.ptr_;
        this->ref_count_ = p.ref_count_;
        ref_count_->add();
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& p) {
        clean();
        this->ptr_ = p.ptr_;
        this->ref_count_ = p.ref_count_;
        p.ptr_ = nullptr;
        p.ref_count_ = nullptr;
        return *this;
    }

    int use_count() { return ref_count_->get(); }

    T* get() const { return ptr_; }

    T* operator->() const { return ptr_; }

    T& operator*() const { return *ptr_; }

    operator bool() const { return ptr_; }

   private:
    template <typename U>
    SharedPtr(const SharedPtr<U>& p, T* ptr) {
        this->ptr_ = ptr;
        this->ref_count_ = p.ref_count_;
        ref_count_->add();
    }

    void clean() {
        if (ref_count_) {
            ref_count_->minus();
            if (ref_count_->get() == 0) {
                if (ptr_) delete ptr_;
                delete ref_count_;
            }
        }
    }

    T* ptr_;
    SharedCount* ref_count_;
};

template <typename T, typename U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U>& p) {
    T* ptr = static_cast<T*>(p.get());
    return SharedPtr<T>(p, ptr);
}

struct A {
    A() { std::cout << "A() \n"; }
    ~A() { std::cout << "~A() \n"; }
};

struct B : A {
    B() { std::cout << "B() \n"; }
    ~B() { std::cout << "~B() \n"; }
};

void test_cast_shared() {
    B* a = new B;
    SharedPtr<B> ptr(a);
    {
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<A> b = static_pointer_cast<A>(ptr);
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<B> c = ptr;
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<B> d = ptr;
        std::cout << ptr.use_count() << std::endl;
    }
    std::cout << ptr.use_count() << std::endl;
}

void test_simple_shared() {
    A* a = new A;
    SharedPtr<A> ptr(a);
    {
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<A> b = ptr;
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<A> c = ptr;
        std::cout << ptr.use_count() << std::endl;
        SharedPtr<A> d = std::move(b);
        std::cout << ptr.use_count() << std::endl;
    }
    std::cout << ptr.use_count() << std::endl;
}

int main() { test_simple_shared(); }
