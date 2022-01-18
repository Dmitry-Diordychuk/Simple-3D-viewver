/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 21:17:35 by kdustin           #+#    #+#             */
/*   Updated: 2021/11/21 00:48:54 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
#define MAP_HPP

#include <limits>
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "Pair.hpp"
#include "Allocator.hpp"

namespace ft
{
    template <class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template <class T>
    struct less : ft::binary_function <T,T,bool>
    {
        bool operator() (const T& x, const T& y) const { return x<y; }
    };

    template < class Key, class T, class Compare = less<Key>, class Alloc = Allocator<Pair<const Key,T> > >
    class Map
    {
    public:
        typedef Key                                 key_type;
        typedef T                                   mapped_type;
        typedef Pair<const key_type, mapped_type>   value_type;
        typedef Compare                             key_compare;
        typedef Alloc                               allocator_type;
        typedef typename Alloc::reference           reference;
        typedef typename Alloc::const_reference     const_reference;
        typedef typename Alloc::pointer	            pointer;
        typedef typename Alloc::const_pointer       const_pointer;
        typedef typename Alloc::difference_type		difference_type;
		typedef size_t								size_type;

        template <typename Tp, typename Ref, typename Ptr>
        class _Map_iterator;

		typedef _Map_iterator<value_type, value_type&, value_type*>				iterator;
		typedef _Map_iterator<value_type, const value_type&, const value_type*>	const_iterator;
		typedef ft::reverse_iterator<iterator>			                        reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	                        const_reverse_iterator;

        class value_compare : ft::binary_function <key_type, key_type, bool>
        {   // in C++98, it is required to inherit binary_function<value_type,value_type,bool>
            public:
                Compare comp;
                value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object

                typedef bool result_type;
                typedef value_type first_argument_type;
                typedef value_type second_argument_type;
                bool operator() (const value_type& x, const value_type& y) const
                {
                    return comp(x.first, y.first);
                }
        };

    private:
        struct node
        {
            bool        is_red;
            bool        is_nil;
            node*       parent;
            node*       left;
            node*       right;
            value_type* val;

            node() {};
            ~node() {};
        };

        allocator_type          _allocator;

        key_compare     _comp;
        node*           _tree;
        node*           _nil;
        size_type       _size;

        node *createNode(key_type key, mapped_type data)
        {
            node* p;
            try
            {
                p = new node();
            }
            catch(...)
            {
                throw;
            }
            try
            {
                p->val = _allocator.allocate(1);
            }
            catch(...)
            {
                delete p;
                throw;
            }
            _allocator.construct(p->val, value_type(key, data));
            p->is_red = false;
            p->is_nil = false;
            p->left = _nil;
            p->right = _nil;
            p->parent = _nil;
            return (p);
        }

        void deleteNode(node *n)
        {
            _allocator.destroy(n->val);
            _allocator.deallocate(n->val, 1);
            delete n;
        }

        node *createNil()
        {
            node *nil;
            try
            {
                nil = createNode(key_type(), mapped_type());
            }
            catch(...)
            {
                throw;
            }
            nil->is_nil = true;
            nil->left = NULL;
            nil->right = NULL;
            nil->parent = NULL;
            return (nil);
        }

        node *cloneNode(node *prev_node)
        {
            node* new_node;
            try
            {
                new_node = new node();
            }
            catch(...)
            {
                throw;
            }
            try
            {
                new_node->val = _allocator.allocate(1);
            }
            catch(...)
            {
                delete new_node;
                throw;
            }
            _allocator.construct(new_node->val, value_type(prev_node->val->first, prev_node->val->second));
            new_node->is_red = prev_node->is_red;
            new_node->is_nil = prev_node->is_nil;
            return (new_node);
        }

        static node *rbTreeMin(node *x)
        {
            while (x->left->is_nil != true)
                x = x->left;
            return (x);
        }

        static node *rbTreeMax(node *x)
        {
            while (x->right->is_nil != true)
                x = x->right;
            return (x);
        }

        node *rbTreeSearch(node *x, key_type key)
        {
            while (x->is_nil != true && key != x->val->first)
                if (_comp(key, x->val->first))
                    x = x->left;
                else
                    x = x->right;
            return (x);
        }

        static node *rbTreeSuccessor(node *x)
        {
            if (x->right->is_nil != true)
                return (rbTreeMin(x->right));
            node *y = x->parent;
            while (y->is_nil != true && x == y->right)
            {
                x = y;
                y = y->parent;
            }
            return (y);
        }

        static node *rbTreePredecessor(node *x)
        {
            if (x->left->is_nil != true)
                return (rbTreeMax(x->left));
            node *y = x->parent;
            while (y->is_nil != true && x != y->right)
            {
                x = y;
                y = y->parent;
            }
            return (y);
        }

        void rbLeftRotate(node *x)
        {
            node *y = x->right;
            x->right = y->left;

            if (y->left != _nil)
                y->left->parent = x;
            y->parent = x->parent;
            if (x->parent == _nil)
                _tree = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;

            x->parent = y;
        }

        void rbRightRotate(node *x)
        {
            node *y = x->left;
            x->left = y->right;

            if (y->right != _nil)
                y->right->parent = x;
            y->parent = x->parent;
            if (x->parent == _nil)
                _tree = y;
            else if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;
            y->right = x;

            x->parent = y;
        }

        void rbInsertFixup(node *z)
        {
            while (z->parent->is_red == true)
            {
                if (z->parent == z->parent->parent->left)
                {
                    node *y = z->parent->parent->right;
                    if (y->is_red == true)
                    {
                        z->parent->is_red = false;
                        y->is_red = false;
                        z->parent->parent->is_red = true;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->right)
                        {
                            z = z->parent;
                            rbLeftRotate(z);
                        }
                        z->parent->is_red = false;
                        z->parent->parent->is_red = true;
                        rbRightRotate(z->parent->parent);
                    }
                }
                else
                {
                    node *y = z->parent->parent->left;
                    if (y->is_red == true)
                    {
                        z->parent->is_red = false;
                        y->is_red = false;
                        z->parent->parent->is_red = true;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->left)
                        {
                            z = z->parent;
                            rbRightRotate(z);
                        }
                        z->parent->is_red = false;
                        z->parent->parent->is_red = true;
                        rbLeftRotate(z->parent->parent);
                    }
                }
            }
            _tree->is_red = false;
        }

        node *rbTreeInsert(node *z)
        {
            node *y = _nil;
            node *x = _tree;
            while (x != _nil)
            {
                if (z->val->first == x->val->first)
                    return (x);
                y = x;
                if (_comp(z->val->first, x->val->first))
                    x = x->left;
                else
                    x = x->right;
            }
            z->parent = y;
            if (y == _nil)
                _tree = z;
            else if (_comp(z->val->first, y->val->first))
                y->left = z;
            else
                y->right = z;
            z->left = _nil;
            z->right = _nil;
            z->is_red = true;
            rbInsertFixup(z);
            return (z);
        }

        node *rbTreeInsert(node *x, node *y, value_type val)
        {
            node *temp_x = x;
            node *temp_y = y;
            node *z;

            if (temp_y == _nil || temp_x != _nil || _comp(val.first, temp_y->val->first))
            {
                try
                {
                    z = createNode(val.first, val.second);
                }
                catch(...)
                {
                    throw;
                }
                temp_y->left = z;

                if (temp_y == _nil)
                    _tree = z;
            }
            else
            {
                try
                {
                    z = createNode(val.first, val.second);
                }
                catch(...)
                {
                    throw;
                }
                temp_y->right = z;
            }
            z->parent = temp_y;
            z->left = _nil;
            z->right = _nil;
            rbInsertFixup(z);
            ++_size;
            _nil->left = rbTreeMin(_tree);
            _nil->right = rbTreeMax(_tree);
            return (z);
        }

        void rbTranslpant(node *u, node *v)
        {
            if (u->parent == _nil)
                _tree = v;
            else if (u == u->parent->left)
                u->parent->left = v;
            else
                u->parent->right = v;
            v->parent = u->parent;
        }

        void rbDeleteFixup(node *x)
        {
            while (x != _tree && x->is_red == false)
            {
                if (x == x->parent->left)
                {
                    node *w = x->parent->right;
                    if (w->is_red == true)
                    {
                        w->is_red = false;
                        x->parent->is_red = true;
                        rbLeftRotate(x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->is_red == false && w->right->is_red == false)
                    {
                        w->is_red = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->right->is_red == false)
                        {
                            w->left->is_red = false;
                            w->is_red = true;
                            rbRightRotate(w);
                            w = x->parent->right;
                        }
                        w->is_red = x->parent->is_red;
                        x->parent->is_red = false;
                        w->right->is_red = false;
                        rbLeftRotate(x->parent);
                        x = _tree;
                    }
                }
                else
                {
                    node *w = x->parent->left;
                    if (w->is_red == true)
                    {
                        w->is_red = false;
                        x->parent->is_red = true;
                        rbRightRotate(x->parent);
                        w = x->parent->left;
                    }
                    if (w->right->is_red == false && w->left->is_red == false)
                    {
                        w->is_red = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->left->is_red == false)
                        {
                            w->right->is_red = false;
                            w->is_red = true;
                            rbLeftRotate(w);
                            w = x->parent->left;
                        }
                        w->is_red = x->parent->is_red;
                        x->parent->is_red = false;
                        w->left->is_red = false;
                        rbRightRotate(x->parent);
                        x = _tree;
                    }
                }
            }
            x->is_red = false;
        }

        void rbDelete(node *z)
        {
            if (z == _nil)
                return ;
            node *x;
            node *y = z;
            bool y_original_color = y->is_red;
            if (z->left == _nil)
            {
                x = z->right;
                rbTranslpant(z, z->right);
            }
            else if (z->right == _nil)
            {
                x = z->left;
                rbTranslpant(z, z->left);
            }
            else
            {
                y = rbTreeMin(z->right);
                y_original_color = y->is_red;
                x = y->right;
                if (y->parent == z)
                    x->parent = y;
                else
                {
                    rbTranslpant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                rbTranslpant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->is_red = z->is_red;
            }
            if (y_original_color == false)
                rbDeleteFixup(x);
        }

        void treeWalkCopy(node *x, node *y)
        {
            if (x->is_nil != true)
            {
                if (x->left->is_nil != true)
                {
                    y->left = cloneNode(x->left);
                    y->left->parent = y;
                }
                else
                    y->left = _nil;

                if (x->right->is_nil != true)
                {
                    y->right = cloneNode(x->right);
                    y->right->parent = y;
                }
                else
                    y->right = _nil;

                treeWalkCopy(x->left, y->left);
                treeWalkCopy(x->right, y->right);
            }
        }

        void treeWalkDelete(node* x)
        {
            if (x == _nil)
                return ;
            if (x->left != _nil)
                treeWalkDelete(x->left);
            if (x->right != _nil)
                treeWalkDelete(x->right);
            if (x == x->parent->left)
                x->parent->left = _nil;
            else if (x == x->parent->right)
                x->parent->right = _nil;
            deleteNode(x);
        }

        node *rbTreeSearch(const key_type key) const
        {
            node *x = _tree;
            while (x != _nil && key != x->val->first)
            {
                if (key < x->val->first)
                    x = x->left;
                else
                    x = x->right;
            }
            return (x);
        }

    public:
        explicit Map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _allocator(alloc), _comp(comp)
        {
            try
            {
                _nil = createNil();
            }
            catch(...)
            {
                throw;
            }
            _tree = _nil;
            _size = 0;
        };

        template <class InputIterator>
        Map (InputIterator first, InputIterator last, const key_compare& comp = key_compare(),
                const allocator_type& alloc = allocator_type())
            : _allocator(alloc), _comp(comp)
        {
            try
            {
                _nil = createNil();
            }
            catch(...)
            {
                throw;
            }
            _tree = _nil;
            _size = 0;
            while (first != last)
            {
                insert(*first);
                ++first;
            }
        };

        Map (const Map& x) : _allocator(x._allocator), _comp(x._comp), _size(x._size) {
            try
            {
                _nil = createNil();
            }
            catch(...)
            {
                throw;
            }
            if (x._tree->is_nil != true)
            {
                this->_tree = cloneNode(x._tree);
                this->_tree->parent = _nil;
            }
            else
                this->_tree = _nil;
            treeWalkCopy(x._tree, this->_tree);
            if (_tree->is_nil != true)
            {
                _nil->left = rbTreeMin(_tree);
                _nil->right = rbTreeMax(_tree);
            }
            else
            {
                _nil->left = _nil;
                _nil->right = _nil;
            }
        };

        Map& operator= (const Map& x) {
            if (this == &x)
                return (*this);
            this->clear();
            this->insert(x.begin(), x.end());
            return (*this);
        };

        ~Map() {
            treeWalkDelete(_tree);
            deleteNode(_nil);
            _size = 0;
        };

        iterator begin() {
            if (_tree == _nil)
                return (end());
            return (iterator(rbTreeMin(_tree)));
        };
        const_iterator begin() const {
            if (_tree == _nil)
                return (end());
            return (const_iterator(rbTreeMin(_tree)));
        };

        iterator end() {
            return (iterator(_nil));
        };
        const_iterator end() const {
            return (const_iterator(_nil));
        };

        reverse_iterator rbegin() {
            if (_tree == _nil)
                return (rend());
            return (reverse_iterator(rbTreeMax(_tree)));
        };
        const_reverse_iterator rbegin() const {
            if (_tree == _nil)
                return (rend());
            return (const_reverse_iterator(rbTreeMax(_tree)));
        };

        reverse_iterator rend() {
            return (reverse_iterator(_nil));
        };
        const_reverse_iterator rend() const {
            return (const_reverse_iterator(_nil));
        };

        bool empty() const {
            if (_tree == _nil)
                return (true);
            return (false);
        };

        size_type size() const {
            return (_size);
        };

        size_type max_size() const {
            return (std::numeric_limits<size_type>::max());
        };

        mapped_type& operator[] (const key_type& k) {
            value_type pr(k, mapped_type());
            return ((*((this->insert(pr)).first)).second);
        };

        Pair<iterator, bool> insert (const value_type& val) {
            Pair<iterator, bool> result;
            node *new_node;
            try
            {
                new_node = createNode(val.first, val.second);
            }
            catch(...)
            {
                throw;
            }


            node* result_node = rbTreeInsert(new_node);
            if (new_node->val == result_node->val)
            {
                result.second = true;
                ++_size;
                _nil->left = rbTreeMin(_tree);
                _nil->right = rbTreeMax(_tree);
            }
            else
            {
                deleteNode(new_node);
                result.second = false;
            }
            result.first = iterator(result_node);
            return (result);
        };

        iterator insert (iterator position, const value_type& val) {
            if (position._cur == rbTreeMin(_tree))
            {
                if (size() > 0 && _comp(val.first, position._cur->val->first))
                {
                    try
                    {
                        return (iterator(rbTreeInsert(position._cur, position._cur, val)));
                    }
                    catch(...)
                    {
                        throw;
                    }
                }
            }
            else if (position._cur == _nil)
            {
                if (_comp(rbTreeMax(_tree)->val->first, val.first))
                {
                    try
                    {
                        return (rbTreeInsert(_nil, rbTreeMax(_tree), val));
                    }
                    catch(...)
                    {
                        throw;
                    }
                }
            }
            else
            {
                iterator prev = position;
                --prev;
                if (_comp(prev._cur->val->first, val.first) && _comp(val.first, position._cur->val->first))
                {
                    try
                    {
                        if (prev._cur->right == _nil)
                            return (rbTreeInsert(_nil, prev._cur, val));
                        else
                            return (rbTreeInsert(position._cur, position._cur, val));
                    }
                    catch(...)
                    {
                        throw;
                    }
                }
            }
            return (insert(val).first);
        };

        template <class InputIterator>
        void insert (InputIterator first, InputIterator last) {
            while (first != last)
            {
                insert(*first);
                ++first;
            }
        };

        void erase (iterator position) {
            rbDelete(position._cur);
            deleteNode(position._cur);
            if (_tree == _nil)
            {
                _nil->left = _nil;
                _nil->right = _nil;
            }
            else
            {
                _nil->left = rbTreeMin(_tree);
                _nil->right = rbTreeMax(_tree);
            }
            --_size;
        };

        size_type erase (const key_type& k) {
            iterator it = find(k);
            if (it != end())
            {
                erase(it);
                return (1);
            }
            return (0);
        };

        void erase (iterator first, iterator last) {
            while (first != last)
            {
                iterator next = first;
                next++;
                erase(first);
                first = next;
            }
        };

        void swap (Map& x) {
            key_compare temp_comp = _comp;
            node*       temp_tree = _tree;
            node*       temp_nil = _nil;
            size_type   temp_size = _size;

            this->_comp = x._comp;
            this->_tree = x._tree;
            this->_nil = x._nil;
            this->_size = x._size;
            x._comp = temp_comp;
            x._tree = temp_tree;
            x._nil = temp_nil;
            x._size = temp_size;
        };

        void clear() {
            node *temp;
            try
            {
                temp = createNil();
            }
            catch(...)
            {
                throw;
            }
            treeWalkDelete(_tree);
            deleteNode(_nil);
            _nil = temp;
            _tree = _nil;
            _size = 0;
        };

        key_compare key_comp() const {
            return (_comp);
        };

        value_compare value_comp() const {
            return (value_compare(_comp));
        };

        iterator find (const key_type& k) {
            return (iterator(rbTreeSearch(k)));
        };

        const_iterator find (const key_type& k) const {
            return (const_iterator(rbTreeSearch(k)));
        };

        size_type count (const key_type& k) const {
            if (find(k) != end())
                return (1);
            return (0);
        };

        iterator lower_bound (const key_type& k) {
            node *x = _tree;
            node *y = _nil;
            while (x != _nil)
            {
                if (_comp(x->val->first, k))
                    x = x->right;
                else
                {
                    y = x;
                    x = x->left;
                }
            }
            if (y == _nil)
                return (end());
            return (iterator(y));
        };

        const_iterator lower_bound (const key_type& k) const {
            node *x = _tree;
            node *y = _nil;
            while (x != _nil)
            {
                if (_comp(x->val->first, k))
                    x = x->right;
                else
                {
                    y = x;
                    x = x->left;
                }
            }
            if (y == _nil)
                return (end());
            return (const_iterator(y));
        };

        iterator upper_bound (const key_type& k) {
            node *x = _tree;
            node *y = _nil;
            while (x != _nil)
            {
                if (_comp(k, x->val->first))
                {
                    y = x;
                    x = x->left;
                }
                else
                    x = x->right;
            }
            if (y == _nil)
                return (end());
            return (iterator(y));
        };

        const_iterator upper_bound (const key_type& k) const {
            node *x = _tree;
            node *y = _nil;
            while (x != _nil)
            {
                if (_comp(k, x->val->first))
                {
                    y = x;
                    x = x->left;
                }
                else
                    x = x->right;
            }
            if (y == _nil)
                return (end());
            return (const_iterator(y));
        };

        Pair<const_iterator,const_iterator> equal_range (const key_type& k) const {
            return (Pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
        };

        Pair<iterator,iterator> equal_range (const key_type& k) {
            return (Pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
        };

        allocator_type get_allocator() const {
            return (_allocator);
        };

        struct _Map_iterator_base
        {
            node *_cur;

            _Map_iterator_base() {};
            _Map_iterator_base(node *x) : _cur(x) {};
            _Map_iterator_base(const _Map_iterator_base& x) : _cur(x._cur) {};
            ~_Map_iterator_base() {};
            _Map_iterator_base& operator=(const _Map_iterator_base& rhs){
                this->_cur = rhs._cur;
                return (*this);
            };

            bool operator== (const _Map_iterator_base& other) const {return (this->_cur == other._cur);};
            bool operator!= (const _Map_iterator_base& other) const {return (this->_cur != other._cur);};

            node *increase(node *x)
            {
                return (rbTreeSuccessor(x));
            }

            node *decrease(node *x)
            {
                return (rbTreePredecessor(x));
            }
        };

        template <typename Tp, typename Ref, typename Ptr>
        class _Map_iterator : public _Map_iterator_base
        {
            public:
                typedef _Map_iterator<Tp, Tp&, Tp*>				iterator;
                typedef _Map_iterator<Tp, const Tp&, const Tp*>	const_iterator;
				typedef _Map_iterator<Tp, Ref, Ptr>				self;

				typedef bidirectional_iterator_tag				iterator_category;
				typedef Tp										value_type;
				typedef std::ptrdiff_t							difference_type;
				typedef Ptr										pointer;
				typedef Ref										reference;

				_Map_iterator() {};
				_Map_iterator(node *current) : _Map_iterator_base(current) {};
				_Map_iterator(const iterator& other) : _Map_iterator_base(other) {};
				~_Map_iterator() {};
				self& operator= (const iterator& rhs) {
					_Map_iterator_base::operator=(rhs);
					return (*this);
				};

				self& operator++ () {
                    if (this->_cur->is_nil == true)
                        this->_cur = this->_cur->left;
                    else
                        this->_cur = this->increase(this->_cur);
					return (*this);
				};

				self& operator++ (int) {
                    if (this->_cur->is_nil == true)
                        this->_cur = this->_cur->left;
                    else
                        this->_cur = this->increase(this->_cur);
					return (*this);
				};

				self& operator-- () {
                    if (this->_cur->is_nil == true)
                        this->_cur = this->_cur->right;
                    else
					    this->_cur = this->decrease(this->_cur);
					return (*this);
				};

				self& operator-- (int) {
                    if (this->_cur->is_nil == true)
                        this->_cur = this->_cur->right;
                    else
					    this->_cur = this->decrease(this->_cur);
					return (*this);
				};

				reference operator*() const {
					return (*this->_cur->val);
				};

				pointer operator->() const {
					return (this->_cur->val);
				};
        };
    };
}

#endif // !MAP_HPP
