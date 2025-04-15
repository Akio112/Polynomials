//#define _GLIBCXX_DEBUG
#pragma once
#include <iostream>


template<typename Type>

class List {
    class Node;
    public:
    struct Iterator{
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Type;
        using pointer = Node*;  
        using reference = Type&; 
        Iterator(pointer ptr = nullptr):this_ptr(ptr){}

        Iterator(const Iterator& other):this_ptr(other.this_ptr){}
        ~Iterator(){};

        Type& operator*()const {
            return this_ptr->val;
        }

        Type* operator->()const {
            return &(this_ptr->val);
        }

        Iterator& operator++() {
            this_ptr = this_ptr->next;
            return *this;
        }

        Iterator& operator--(){
            this_ptr = this_ptr->prev;
            return *this;
        }

        Iterator operator=(const Iterator& other) {
            this_ptr = other.this_ptr;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const Iterator& other) const{
            return other.this_ptr == this_ptr;
        }

        bool operator !=(const Iterator& other) const {
            return !(*this == other);
        }

        friend class List<Type>;

        private:

        void Del() {
            delete this_ptr;
        }
        pointer this_ptr;
    };


        List() = default;
        List(const List<Type>& other) {
            List<Type>::Iterator it = other.begin();
            while (it != other.end()) {
                this->push_back(Type(*it));
                ++it;
            }
        }


        void push_back(Type value) {
            if (size()) {
                back_->next = new Node(value);
                back_->next->prev = back_;
                back_ = back_->next;
            } else {
                back_ = new Node(value);
                begin_ = back_;
            }
            size_++;
        }
        void pop_back() {
            if (size() > 1) {
                back_ = back_->prev;
                delete back_->next;
                back_->next = nullptr;
            } else {
                delete back_;
                back_ = nullptr;
                begin_ = nullptr;
            }
            size_--;
        }

        ~List() {
            while (size()) {
                pop_back();
            }
        }
        void push_front(Type object) {
            if (size() == 0) {
                begin_ = new Node(object);
                back_ = begin_;
            } else {
                begin_->prev = new Node(object);
                begin_->prev->next = begin_;
                begin_ = begin_->prev;
            }
            size_++;
        }
        void pop_front() {
            if(size() > 1) {
                begin_ = begin_->next;
                delete begin_->prev;
                begin_->prev = nullptr;
            } else {
                delete back_;
                back_ = nullptr;
                begin_ =nullptr;
            }
            size_--;
        }

        Type& back() const {
            return back_->val;
        }

        Type front()const {
            return begin_->val;
        }

        Type& operator[](int pos) {
            List<Type>::Iterator it = begin();
            if (it == end()) {
                throw "Вы вышли за пределы списка:(";
            }
            for (int i = 0;i < pos; ++i) {
                ++it;
                if (it == end()) {
                    throw "Вы вышли за пределы списка:(";
                }
            }
            return (*it);
        }

        int size() const{
            return size_;
        }

        bool empty() const {
            return !size();
        }


        void clear() {
            while(size()) {
                pop_back();
            }
        }

        List& operator=(const List<Type>& other) {
            clear();
            for (Type elem:other) {
                push_back(elem);
            }
            return *this;
        }

        void MergeSort(Iterator& start,Iterator& end_sort, int size) {
            if (size <= 1) {
                return;
            }
            int mid = size / 2;
            Iterator it_mid = start;
            for (int i = 0;i < mid; ++i) {
                ++it_mid;
            }
            MergeSort(start, it_mid, mid);
            MergeSort(it_mid, end_sort, size - mid);
            List<Type> new_list;
            Iterator it_first = start;
            Iterator it_second = it_mid;
            while (it_first != it_mid && it_second != end_sort) {
                if (*it_first < *it_second) {
                    new_list.push_back(Type(*it_first));
                    ++it_first;
                } else {
                    new_list.push_back(Type(*it_second));
                    ++it_second;
                }
            }
            while (it_first != it_mid) {
                new_list.push_back(Type(*it_first));
                ++it_first;
            }
            while (it_second != end_sort) {
                new_list.push_back(Type(*it_second));
                ++it_second;
            }
            if (start != begin()) {
                start.this_ptr->prev->next = new_list.begin_;
                new_list.begin_->prev = start.this_ptr->prev;
            } else {
                begin_ = new_list.begin_;
            }
            if (end_sort != end()) {
                end_sort.this_ptr->prev = new_list.back_;
                new_list.back_->next = end_sort.this_ptr;
            } else {
                back_ = new_list.back_;
            }
            Iterator it_del = start;
            while (it_del != end_sort) {
                delete it_del.this_ptr;
                ++it_del;
            }
            start.this_ptr = new_list.begin_;
            new_list.begin_ = nullptr;
            new_list.back_ = nullptr;
            new_list.size_ = 0;
        }

        Iterator begin() const {
            return Iterator(begin_);
        }
        Iterator end() const{
            return Iterator(nullptr);
        }

        void erase(Iterator it_erase) {
            if (size() == 0) return;
            if (it_erase.this_ptr->prev) {
                it_erase.this_ptr->prev->next = it_erase.this_ptr->next;
            }
            if (it_erase.this_ptr->next) {
                it_erase.this_ptr->next->prev = it_erase.this_ptr->prev;
            }
            if (it_erase == begin()) {
                begin_ = it_erase.this_ptr->next;
            }
            if (it_erase == Iterator(back_)) {
                back_ = it_erase.this_ptr->prev;
            }
            it_erase.Del();
            size_--;
        }

        void erase(int index) {
            if (size_ == 0) {
                throw "Нельзя удалять из пустого списка!";
            }
            Iterator it_del = begin();
            for (int i = 0;i < index;++i) {
                ++it_del;
                if (it_del == end()) {
                    throw "Вы вышли за пределы массива";
                }
            }
            erase(it_del);
        }

        void sort() {
            Iterator start(begin_), end_sort(nullptr);
            MergeSort(start, end_sort, size_);
        }

    private:
        
        struct Node{
            public:
            Node() = default;
            Node(Type x):val(x){}
            Type operator*(){
                return val;
            }
            Node& operator++(){
                this = next;
            }
            friend class List;
            private:
            Type val;
            Node* prev = nullptr, *next = nullptr;
        };
        Node* begin_ = nullptr, *back_ = nullptr;
        int size_ = 0;
};

