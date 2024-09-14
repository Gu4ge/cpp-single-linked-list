#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList
{

    struct Node
    {
        Node() = default;
        Node(const Type &val, Node *next)
            : value(val), next_node(next)
        {
        }
        Type value;
        Node *next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator
    {
        friend class SingleLinkedList;

        explicit BasicIterator(Node *node)
            : node_(node)
        {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type> &other) noexcept
            : node_(other.node_)
        {
        }

        BasicIterator &operator=(const BasicIterator &rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        BasicIterator &operator++() noexcept
        {
            assert(node_ != nullptr);

            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept
        {
            // assert(node_ != nullptr);

            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            assert(node_ != nullptr);

            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            assert(node_ != nullptr);

            return &node_->value;
        }

    private:
        Node *node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return begin();
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return end();
    }

    SingleLinkedList()
    {
    }

    SingleLinkedList(std::initializer_list<Type> values)
    {
        Assign(values);
    }

    SingleLinkedList(const SingleLinkedList &other)
    {
        assert(size_ == 0 && head_.next_node == nullptr);

        Assign(other);
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs)
    {
        if (this != &rhs)
        {
            if (rhs.IsEmpty())
            {
                Clear();
                return *this;
            }

            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }

    Iterator InsertAfter(ConstIterator pos, const Type &value)
    {
        assert(pos.node_ != nullptr);
        Node *insert_after = new Node(value, pos.node_->next_node);
        pos.node_->next_node = insert_after;
        size_++;
        return Iterator{insert_after};
    }

    void PopFront() noexcept
    {
        assert(!IsEmpty());
        Node *n_head = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = n_head;
        size_--;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        assert(!IsEmpty() || pos.node_ != nullptr);
        Node *erase_after = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = erase_after;
        size_--;
        return Iterator{pos.node_->next_node};
    }

    void swap(SingleLinkedList &other) noexcept
    {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }

    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept
    {
        return size_ == 0;
    }

    void PushFront(const Type &value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void Clear() noexcept
    {
        while (head_.next_node)
        {
            Node *new_head = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_head;
        }
        size_ = 0;
    }

private:
    Node head_;
    size_t size_ = 0;

    template <typename T>
    void Assign(T &elem)
    {
        SingleLinkedList tmp;
        SingleLinkedList tmp_reverse;

        for (auto it = elem.begin(); it != elem.end(); ++it)
        {
            tmp_reverse.PushFront(*it);
        }
        for (auto it = tmp_reverse.begin(); it != tmp_reverse.end(); ++it)
        {
            tmp.PushFront(*it);
        }

        swap(tmp);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs > rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs < rhs);
}
