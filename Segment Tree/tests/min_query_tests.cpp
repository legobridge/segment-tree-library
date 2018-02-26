#include "pch.h"
#include "segment_tree.hpp"

#include <vector>
#include <utility>
#include <random>

namespace min_int_query
{
    // Structures and methods for testing the minimum integer query form of the segment tree
    struct node
    {
        int min;
    };

    void set_default_value(node& x, int y)
    {
        x.min = y;
    }

    node* merge(node* a, node* b)
    {
        if (a->min <= b->min)
        {
            return a;
        }
        return b;
    }

    // Fixture for testing the minimum integer query form of the segment tree
    class min_int_segment_tree_constructor : public ::testing::Test
    {
    protected:
        void SetUp( int n )
        {
            segtree = new segment_tree< int, node, set_default_value, merge >(n);
        }
        void SetUp( const std::vector<int>& array )
        {
            segtree = new segment_tree< int, node, set_default_value, merge >(array);
        }
        virtual void TearDown() 
        {
            delete segtree;
        }
        segment_tree< int, node, set_default_value, merge >* segtree;
    };

    TEST_F( min_int_segment_tree_constructor, size_parameter_case1 )
    {
        size_t n = 1;
        SetUp(n);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }

    TEST_F( min_int_segment_tree_constructor, size_parameter_case2 )
    {
        size_t n = 42;
        SetUp(n);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }

    TEST_F( min_int_segment_tree_constructor, size_parameter_case3 )
    {
        size_t n = 42000;
        SetUp(n);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }


    void fill_with_random_integers(size_t n, std::vector<int>& parameter_array)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, n);
        for (int i = 0; i < (int)n; i++)
        {
            parameter_array.push_back(dis(gen));
        }
    }

    TEST_F( min_int_segment_tree_constructor, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<int> parameter_array(n, 0);
        SetUp(parameter_array);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST_F( min_int_segment_tree_constructor, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        SetUp(parameter_array);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST_F( min_int_segment_tree_constructor, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        SetUp(parameter_array);
        EXPECT_EQ(segtree->get_array_size(), n);
        std::vector<int> array = segtree->get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }
}