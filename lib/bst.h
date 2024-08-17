#pragma once
#include <memory>


enum class TraversalType {
  InOrder,
  PreOrder,
  PostOrder
};

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class bst {
 public:
  using key_type = Key;
  using value_type = Key;
  using key_compare = Compare;
  using value_compare = Compare;
  using size_type = std::size_t;
  using reference = Key&;
  using const_reference = const Key&;
  using pointer = Key*;
  using const_pointer = const Key*;
  struct node;
  using node_type = node*;
  using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
  using alloc_traits = std::allocator_traits<allocator_type>;

  struct node {
	value_type data;
	node_type left;
	node_type right;
	node_type parent;

	node(const_reference data)
		: data(data)
		, left(nullptr)
		, right(nullptr)
		, parent(nullptr)
	{}

	node_type get_largest(node_type root) const {
	  node_type node = root;
	  while (node->right) {
		node = node->right;
	  }
	  return node;
	}

	node_type get_smallest(node_type root) const {
	  node_type node = root;
	  while (node->left) {
		node = node->left;
	  }
	  return node;
	}

	node_type get_min_node() {
	  if (!this) {
		return nullptr;
	  }
	  node_type node = this;
	  while (node->left) {
		node = node->left;
	  }
	  return node;
	}

	node_type get_max_node() {
	  if (!this) {
		return nullptr;
	  }
	  node_type node = this;
	  while (node->right) {
		node = node->right;
	  }
	  return node;
	}

	node_type get_max_leaf() {
	  if (!this) {
		return nullptr;
	  }
	  node_type node = this;
	  while (node->left || node->right) {
		if (node->right) {
		  node = node->right;
		} else {
		  node = node->left;
		}
	  }
	  return node;
	}

	node_type get_min_leaf() {
	  if (!this) {
		return nullptr;
	  }
	  node_type node = this;
	  while (node->left || node->right) {
		if (node->left) {
		  node = node->left;
		} else {
		  node = node->right;
		}
	  }
	  return node;
	}
  };

  template<bool IsConst>
  class base_iterator {
   public:
	using iterator_type = typename std::conditional<IsConst, typename bst<value_type>::const_iterator, typename bst<value_type>::iterator>::type;
	base_iterator(node_type root, node_type current, TraversalType type = TraversalType::InOrder)
		: root_(root)
		, current_(current)
		, traversal_type_(type)
	{}

	const_reference operator*() const {
	  return current_->data;
	}

	iterator_type& operator++() {
	  switch (traversal_type_) {
		case TraversalType::InOrder:
		  if (!current_) {
			current_ = current_->get_smallest(root_);
			break;
		  }
		  if (current_->right) {
			current_ = current_->right->get_min_node();
		  } else {
			while (current_->parent && current_->parent->right == current_) {
			  current_ = current_->parent;
			}
			current_ = current_->parent;
		  }
		  break;
		case TraversalType::PreOrder:
		  if (current_->left) {
			current_ = current_->left;
		  } else if (current_->right) {
			current_ = current_->right;
		  } else {
			while (current_->parent && (!current_->parent->right || current_->parent->right == current_)) {
			  current_ = current_->parent;
			}
			current_ = current_->parent ? current_->parent->right : nullptr;
		  }
		  break;
		case TraversalType::PostOrder:
		  if (current_->parent == nullptr) {
			current_ = current_->parent;
			break;
		  }
		  if (current_ == current_->parent->right) {
			current_ = current_->parent;
		  } else {
			if (current_->parent->right){
			  current_ = current_->parent->right->get_min_leaf();
			} else {
			  current_->parent;
			}
		  }
		  break;
	  }
	  return *this;
	}

	iterator_type operator++(int) {
	  iterator temp = *this;
	  ++(*this);
	  return temp;
	}

	iterator_type& operator--() {
	  switch (traversal_type_) {
		case TraversalType::InOrder:
		  if (current_ == nullptr) {
			current_ = current_->get_largest(root_);
			break;
		  }
		  if (current_->left != nullptr) {
			current_ = current_->left->get_max_node();
		  } else {
			node_type parent = current_->parent;
			while (parent != nullptr && current_ == parent->left) {
			  current_ = parent;
			  parent = parent->parent;
			}
			current_ = parent;
		  }
		  break;
		case TraversalType::PreOrder:
		  if (current_ == nullptr) {
			current_ = root_->get_max_leaf();
			break;
		  }
		  if (current_->parent && current_->parent->left == current_) {
			current_ = current_->parent;
		  }
		  else if (current_->parent && current_->parent->right == current_){
			current_ = current_->parent->left ? current_->parent->left->get_max_node()  : current_->parent;
		  }
		  else {
			current_ = nullptr;
		  }
		  break;


		case TraversalType::PostOrder:
		  if (current_ == nullptr) {
			current_ = root_;
			break;
		  }
		  if (current_->right != nullptr) {
			current_ = current_->right;
		  } else if (current_->left != nullptr) {
			current_ = current_->left;
		  } else {
			if (current_->parent) {
			  node_type parent = current_->parent;
			  while (parent && (parent->left == nullptr || current_ == parent->left)) {
				current_ = parent;
				parent = current_->parent;
			  }
			  if (parent) {
				current_ = parent->left;
			  } else {
				current_ = nullptr;
				break;
			  }
			} else {
			  current_ = current_->parent;
			  break;
			}
		  }
	  }
	  return *this;
	}

	iterator_type operator--(int) {
	  iterator temp = *this;
	  --(*this);
	  return temp;
	}

	bool operator!=(const base_iterator& other) const {
	  return current_ != other.current_;
	}

	bool operator==(const base_iterator& other) const {
	  return current_ == other.current_;
	}

   private:
	node_type root_;
	node_type current_;
	TraversalType traversal_type_;
  };

  using iterator = base_iterator<false>;
  using const_iterator = base_iterator<true>;

  template<bool IsConst>
  class base_reverse_iterator {
   public:
	using iterator_type = typename base_iterator<IsConst>::iterator_type;
	using reverse_iterator_type = typename std::conditional<IsConst, typename bst<value_type>::const_reverse_iterator, typename bst<value_type>::reverse_iterator>::type;

	base_reverse_iterator(iterator_type iter)
		: current(iter)
	{}

	const_reference operator*() const {
	  return *current;
	}

	reverse_iterator_type& operator++() {
	  --current;
	  return *this;
	}

	reverse_iterator_type operator++(int) {
	  reverse_iterator temp = *this;
	  --current;
	  return temp;
	}

	reverse_iterator_type& operator--() {
	  ++current;
	  return *this;
	}

	reverse_iterator_type operator--(int) {
	  reverse_iterator_type temp = *this;
	  ++current;
	  return temp;
	}

	bool operator==(const reverse_iterator_type& other) {
	  return this->current == other.current;
	}

	bool operator!=(const reverse_iterator_type& other) {
	  return this->current != other.current;
	}

   private:
	iterator_type current;
  };

  using const_reverse_iterator = base_reverse_iterator<true>;
  using reverse_iterator = base_reverse_iterator<false>;

  bst()
	  : root_(nullptr)
	  , size_(0)
  {}

  bst(const bst<value_type>& other)
	  : size_(other.size_)
	  , compare_(other.compare_)
	  , allocator_(alloc_traits::select_on_container_copy_construction(other.allocator_)) {
	root_ = copy(other.root_, nullptr, allocator_);
  }

  bst(const std::initializer_list<value_type> il) {
	root_ = nullptr;
	size_ = 0;
	for (auto it = il.begin(); it != il.end(); ++it) {
	  insert(*it);
	}
  }

  bst(iterator begin, iterator end) {
	root_ = nullptr;
	size_ = 0;
	for (auto it = begin; it != end; ++it) {
	  insert(*it);
	}
  }

  bst& operator=(const bst<value_type>& other) {
	if (this == &other) {
	  return *this;
	}

	allocator_type new_alloc = alloc_traits::propagate_on_container_copy_assignment::value
							   ? other.allocator_ : allocator_;
	node_type new_root = copy(other.root_, nullptr, new_alloc);
	clear();
	root_ = new_root;
	allocator_ = new_alloc;
	size_ = other.size_;
	compare_ = other.compare_;

	return *this;
  }

  bst& operator=(const std::initializer_list<value_type> il) {
	bst tmp;
	for (auto it = il.begin(); it != il.end(); ++it) {
	  tmp.insert(*it);
	}
	clear();
	*this = tmp;
	return *this;
  }

  bool operator==(const bst<value_type>& other) const {
	if (size_ != other.size_) {
	  return false;
	}

	iterator lhs = this->begin();
	iterator rhs = other.begin();

	while (lhs != end() && rhs != other.end() && *lhs == *rhs) {
	  ++lhs;
	  ++rhs;
	}
	return lhs == end() && rhs == other.end();
  }

  bool operator!=(const bst<value_type>& other) const {
	return !(*this == other);
  }

  iterator begin(TraversalType type = TraversalType::InOrder) const {
	switch (type) {
	  case TraversalType::InOrder:
		return iterator(root_, root_->get_min_node(), TraversalType::InOrder);
	  case TraversalType::PreOrder:
		return iterator(root_, root_, TraversalType::PreOrder);
	  case TraversalType::PostOrder:
		return iterator(root_, root_->get_min_leaf(), TraversalType::PostOrder);
	}
	return iterator(root_, nullptr, type);
  }

  iterator end(TraversalType type = TraversalType::InOrder) const {
	return iterator(root_, nullptr, type);
  }

  reverse_iterator rbegin(TraversalType type = TraversalType::InOrder) const {
	switch (type) {
	  case TraversalType::InOrder:
		return reverse_iterator(iterator(root_, root_->get_max_node(), TraversalType::InOrder));
	  case TraversalType::PreOrder:
		return reverse_iterator(iterator(root_, root_->get_max_leaf(), TraversalType::PreOrder));
	  case TraversalType::PostOrder:
		return reverse_iterator(iterator(root_, root_, TraversalType::PostOrder));
	}
	return reverse_iterator(iterator(root_, nullptr, type));
  }

  reverse_iterator rend(TraversalType type = TraversalType::InOrder) const {
	return reverse_iterator(iterator(root_, nullptr, type));
  }

  const_iterator cbegin(TraversalType type = TraversalType::InOrder) const {
	switch (type) {
	  case TraversalType::InOrder:
		return const_iterator(root_, root_->get_min_node(), TraversalType::InOrder);
	  case TraversalType::PreOrder:
		return const_iterator(root_, root_, TraversalType::PreOrder);
	  case TraversalType::PostOrder:
		return const_iterator(root_, root_->get_max_leaf(), TraversalType::PostOrder);
	}
	return const_iterator(root_, nullptr, type);
  }

  const_iterator cend(TraversalType type = TraversalType::InOrder) const {
	return const_iterator(root_, nullptr, type);
  }

  const_reverse_iterator crbegin(TraversalType type = TraversalType::InOrder) const {
	switch (type) {
	  case TraversalType::InOrder:
		return const_reverse_iterator(const_iterator(root_, root_->get_max_node(), TraversalType::InOrder));
	  case TraversalType::PreOrder:
		return const_reverse_iterator(const_iterator(root_, root_->get_max_leaf(), TraversalType::PreOrder));
	  case TraversalType::PostOrder:
		return const_reverse_iterator(const_iterator(root_, root_, TraversalType::PostOrder));
	}
	return const_reverse_iterator(const_iterator(root_, nullptr, type));
  }

  const_reverse_iterator crend(TraversalType type = TraversalType::InOrder) const {
	return const_reverse_iterator(const_iterator(root_, nullptr, type));
  }

  void swap(bst<value_type>& other) {
	if constexpr (alloc_traits::propagate_on_container_swap::value) {
	  std::swap(allocator_, other.allocator_);
	  bst tmp = *this;
	  *this = other;
	  other = tmp;
	}
  }

  size_type size() const {
	return size_;
  }

  size_type max_size() const {
	return alloc_traits::max_size(allocator_);
  }

  bool empty() const {
	return (size_ == 0);
  }

  Allocator get_allocator() const {
	return allocator_;
  }

  key_compare key_comp() const {
	return compare_;
  }

  key_compare value_comp() const {
	return compare_;
  }

  void insert(iterator begin, iterator end) {
	for (iterator i = begin; i != end; ++i){
	  insert(*i);
	}
  }

  node_type extract (iterator target) {
	return extract(*target);
  }

  node_type extract (value_type value) {
	node_type extracted_node = find_node(value, root_);
	if (extracted_node) {
	  remove_node(extracted_node);
	}
	return extracted_node;
  }

  size_type erase(value_type value) {
	node_type removed_node = find_node(value, root_);
	if (removed_node) {
	  remove_node(removed_node);
	  return 1;
	}
	return 0;
  }

  iterator erase(iterator target) {
	node_type removed_node = find_node(*target, root_);
	if (removed_node) {
	  remove_node(removed_node);
	  ++target;
	  return target;
	}
	return end();
  }

  iterator erase(iterator begin, iterator end) {
	while(begin != end){
	  begin = erase(begin);
	}
	return end;
  }

  void merge(bst& other) {
	for (const auto& element : other) {
	  auto it = find(element);
	  if (it == end()) {
		insert(element);
	  }
	}
  }

  iterator find(value_type value) const {
	node_type target = find_node(value, root_);
	return iterator(root_, target);
  }

  bool contains(value_type value) const {
	return find_node(value, root_) != nullptr;
  }

  size_type count(value_type value) const {
	return find_node(value, root_) == nullptr ? 0 : 1;
  }

  iterator upper_bound(value_type value) const {
	node_type current = root_;
	node_type upper_bound_node = nullptr;

	while (current != nullptr) {
	  if (compare_(value, current->data)) {
		upper_bound_node = current;
		current = current->left;
	  } else {
		current = current->right;
	  }
	}
	return iterator(root_, upper_bound_node);
  }

  iterator lower_bound(value_type value) const {
	node_type current = root_;
	node_type lower_bound_node = nullptr;

	while (current != nullptr) {
	  if (compare_(value, current->data) || value == current->data) {
		lower_bound_node = current;
		current = current->left;
	  } else {
		current = current->right;
	  }
	}
	return iterator(root_, lower_bound_node);
  }

  void insert(const_reference x) {
	insert(root_, nullptr, x);
  }

  void clear () {
	clear(root_);
  }

  ~bst() {
	clear();
  }

 private:
  void insert(node_type& root, node_type parent, const_reference x) {
	if (root == nullptr) {
	  root = alloc_traits::allocate(allocator_, 1);
	  alloc_traits::construct(allocator_, root, x);
	  root->parent = parent;
	  ++size_;
	  return;
	}
	if (compare_(x, root->data)) {
	  insert(root->left, root, x);
	} else if (!compare_(x, root->data)) {
	  insert(root->right, root, x);
	}
  }

  void remove_node(node_type& node) {
	if (node->left == nullptr) {
	  node_type temp = node->right;
	  if (temp) temp->parent = node->parent;
	  if (node->parent) {
		if (node->parent->left == node) {
		  node->parent->left = temp;
		} else {
		  node->parent->right = temp;
		}
	  } else {
		root_ = temp;
	  }
	  alloc_traits::destroy(allocator_, node);
	  alloc_traits::deallocate(allocator_, node, 1);
	  --size_;
	  node = temp;
	} else if (node->right == nullptr) {
	  node_type temp = node->left;
	  if (temp) temp->parent = node->parent;
	  if (node->parent) {
		if (node->parent->left == node) {
		  node->parent->left = temp;
		} else {
		  node->parent->right = temp;
		}
	  } else {
		root_ = temp;
	  }
	  alloc_traits::destroy(allocator_, node);
	  alloc_traits::deallocate(allocator_, node, 1);
	  --size_;
	  node = temp;
	} else {
	  node_type temp = next(node);
	  remove_node(temp);
	  node->data = temp->data;
	  if (temp->right) {
		remove_node(temp->right);
	  } else {
		if (temp->parent) {
		  if (temp->parent->left == temp) {
			temp->parent->left = nullptr;
		  } else {
			temp->parent->right = nullptr;
		  }
		}
		alloc_traits::destroy(allocator_, temp);
		alloc_traits::deallocate(allocator_, temp, 1);
		--size_;
	  }
	}
  }

  node_type next(node_type node) const {
	node_type target = nullptr;
	value_type key = node->data;
	while (node != nullptr) {
	  if (node->data > key) {
		target = node;
		node = node->left;
	  } else {
		node = node->right;
	  }
	}
	return target;
  }

  node_type find_node(value_type value, node_type current_node) const {
	while (current_node != nullptr) {
	  if (value == current_node->data) {
		break;
	  } else if (compare_(value, current_node->data)) {
		current_node = current_node->left;
	  } else {
		current_node = current_node->right;
	  }
	}
	return current_node;
  }

  node_type copy(node_type src, node_type parent, auto Alloc) {
	if (!src) {
	  return nullptr;
	}

	node_type new_node = alloc_traits::allocate(Alloc, 1);
	alloc_traits::construct(Alloc, new_node, src->data);
	new_node->parent = parent;
	new_node->left = copy(src->left, new_node, Alloc);
	new_node->right = copy(src->right, new_node, Alloc);
	return new_node;
  }

  void clear(node_type& root) {
	if (root) {
	  clear(root->left);
	  clear(root->right);
	  alloc_traits::destroy(allocator_, root);
	  alloc_traits::deallocate(allocator_, root, 1);
	  root = nullptr;
	}
  }
  node_type root_;
  size_type size_;
  key_compare compare_;
  allocator_type allocator_;
};
