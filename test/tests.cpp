#include <lib/bst.h>
#include <gtest/gtest.h>

TEST(BinarySearchTreeTest, IsContainer) {
    EXPECT_TRUE(bst<int>().empty());

    bst<int> tree1;
    tree1.insert(5);
    EXPECT_FALSE(tree1.empty());
    EXPECT_TRUE(tree1 == bst<int>(tree1));

    bst<int> tree2 = tree1;
    EXPECT_EQ(tree1, tree2);

    bst<int> tree3;
    tree3.insert(3);
    EXPECT_TRUE(tree1 != tree3);

    bst<int> tree4;
    tree4.insert(4);
    tree4.insert(6);
    tree3.swap(tree4);
    EXPECT_EQ(*tree4.begin(), 4);
    EXPECT_TRUE(tree3.size() == 1);
    tree3 = {1,2,3};
    EXPECT_TRUE(tree3.size() == 3);
    EXPECT_TRUE(std::is_const<typename std::remove_reference<decltype(*tree4.cbegin())>::type>::value);
}

TEST(BinarySearchTreeTest, IsAssociativeContainer) {
    bst<int> tree1({1, 2, 3, 4, 5});

    bst<int> tree2(tree1.begin(), ++++tree1.begin());
    EXPECT_EQ(*tree2.begin(), 1);
    EXPECT_EQ(*tree2.rbegin(), 2);

    bst<int> tree3(tree1);
    tree3.erase(tree3.begin());
    tree3.erase(2);
    tree3.erase(tree3.begin(),++tree3.begin());
    EXPECT_EQ(*tree3.begin(), 4);
    EXPECT_EQ(*tree3.rbegin(), 5);

    bst<int> tree4;
    tree4.insert(tree3.begin(),++tree3.begin());
    EXPECT_EQ(*tree4.begin(), 4);

    tree1.extract(5);
    tree1.extract(tree1.begin());
    EXPECT_EQ(*tree1.begin(), 2);
    EXPECT_EQ(*(----tree1.rend()), 3);
    EXPECT_EQ(*tree1.rbegin(), 4);
    EXPECT_TRUE(tree1.find(4) != tree1.end());

    tree4.merge(tree2);
    EXPECT_EQ(*tree4.begin(), 1);
    EXPECT_EQ(*(++tree4.begin()), 2);
    EXPECT_EQ(*tree4.rbegin(), 4);
    EXPECT_EQ(*tree4.lower_bound(1), 1);
    EXPECT_EQ(*tree4.upper_bound(1), 2);
}

TEST(BinarySearchTreeTest, HasBidirectionalIterator) {
    bst<int> tree;
    tree.insert(3);
    tree.insert(1);
    tree.insert(5);
    tree.insert(2);
    tree.insert(4);

    bst<int>::iterator it = tree.begin();
    ASSERT_EQ(*it, 1);

    ++it;
    ASSERT_EQ(*it, 2);

    --it;
    ASSERT_EQ(*it, 1);

    ++it;
    ASSERT_EQ(*it, 2);

    ++it;
    ASSERT_EQ(*it, 3);

    --it;
    ASSERT_EQ(*it, 2);
}

TEST(BinarySearchTreeTest, HasReverseIterator) {
    bst<int> tree;
    tree.insert(3);
    tree.insert(1);
    tree.insert(5);
    tree.insert(2);
    tree.insert(4);

    bst<int>::reverse_iterator rit = tree.rbegin();
    ASSERT_EQ(*rit, 5);

    ++rit;
    ASSERT_EQ(*rit, 4);

    --rit;
    ASSERT_EQ(*rit, 5);

    ++rit;
    ASSERT_EQ(*rit, 4);

    ++rit;
    ASSERT_EQ(*rit, 3);

    --rit;
    ASSERT_EQ(*rit, 4);
}

TEST(BinarySearchTreeTest, SupportsAllocator) {
    bst<int, std::less<int>, std::allocator<int>> tree1;
    bst<int, std::less<int>, std::allocator<int>> tree2;
    EXPECT_EQ(tree1.get_allocator(),tree2.get_allocator());
}

TEST(BinarySearchTreeTest, TraversalTests){
    bst<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(7);
    tree.insert(0);
    tree.insert(15);
    tree.insert(12);
    tree.insert(20);
    std::vector<int> pre = {10,5,0,7,15,12,20};
    std::vector<int> in = {0,5,7,10,12,15,20};
    std::vector<int> post = {0,7,5,12,20,15,10};

    std::vector<int>::iterator expected = pre.begin();
    for (auto i = tree.begin(TraversalType::PreOrder); i != tree.end(TraversalType::PreOrder);++i) {
        EXPECT_EQ(*i, *expected);
        ++expected;
    }

    expected = in.begin();
    for (auto i = tree.begin(); i != tree.end();++i) {
        EXPECT_EQ(*i, *expected);
        ++expected;
    }

    expected = post.begin();
    for (auto i = tree.begin(TraversalType::PostOrder); i != tree.end(TraversalType::PostOrder); ++i) {
        EXPECT_EQ(*i,*expected);
        ++expected;
    }

    expected = --(pre.end());
    for (auto i = --tree.end(TraversalType::PreOrder); i != --tree.begin(TraversalType::PreOrder); --i) {
        EXPECT_EQ(*i, *expected);
        --expected;
    }

    expected = --(in.end());
    for (auto i = tree.rbegin(); i != tree.rend(); ++i){
        EXPECT_EQ(*i, *expected);
        --expected;
    }

    expected = --(post.end());
    for (auto i = tree.crbegin(TraversalType::PostOrder); i != tree.crend(TraversalType::PostOrder); ++i){
        EXPECT_EQ(*i, *expected);
        --expected;
    }
}
