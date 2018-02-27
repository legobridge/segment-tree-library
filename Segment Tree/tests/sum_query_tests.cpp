#include "pch.h"
#include "segment_tree.hpp"

#include <vector>
#include <utility>
#include <random>

namespace sum_int_query
{
    // Structures and methods for testing the 
    // sum of int query form of the segment tree
    struct node
    {
        int sum;
    };

    void set_default_value( node& x, int y )
    {
        x.sum = y;
    }

    node merge( node* a, node* b )
    {
        node ans;
        ans.sum = a->sum + b->sum;
        return ans;
    }
    
    // Generates an array of n random integers ranging from 1 to n
    void fill_with_random_integers( size_t n, std::vector<int>& parameter_array )
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(1, n);
        for (int i = 0; i < (int)n; i++)
        {
            parameter_array.push_back(dis(gen));
        }
    }

    // Generates m random interval queries
    void fill_with_random_intervals( size_t n, size_t m, std::vector<std::pair<int, int>>& queries )
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis1(0, n - 1);
        for (int i = 0; i < (int)m; i++)
        {
            // Generate the lower bound of the interval
            int x = dis1(gen);
            // Make a uniform distribution within [x, n]
            std::uniform_int_distribution<> dis2(x, n - 1);
            // Generate the upper bound of the interval
            int y = dis2(gen);
            queries.push_back({x, y});
        }
    }

    // Generates m random intervals containing a given index
    void fill_with_random_intervals( size_t n, size_t m, int index, std::vector<std::pair<int, int>>& queries )
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis1(0, index);
        std::uniform_int_distribution<> dis2(index, n - 1);
        for (int i = 0; i < (int)m; i++)
        {
            // Generate the lower bound of the interval
            int x = dis1(gen);
            // Generate the upper bound of the interval
            int y = dis2(gen);
            queries.push_back({x, y});
        }
    }

    // Brute force method to solve the sum of interval problem
    void run_brute_force( const std::vector<int>& ar, 
                          const std::vector<std::pair<int, int>>& queries, 
                          std::vector<int>& results )
    {
        size_t n = ar.size();
        size_t m = queries.size();
        std::vector<int> cumulative_sum(n);
        cumulative_sum[0] = ar[0];
        for (int i = 1; i < (int)n; i++)
        {
            cumulative_sum[i] = cumulative_sum[i - 1] + ar[i];
        }
        for (int i = 0; i < (int)m; i++)
        {
            results[i] = cumulative_sum[queries[i].second] - 
                         cumulative_sum[queries[i].first] + 
                         ar[queries[i].first];
        }
    }


    // Tests for both types of constructors

    TEST( sum_int_segment_tree_constructor, size_parameter_case1 )
    {
        size_t n = 1;
        segment_tree< int, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }

    TEST( sum_int_segment_tree_constructor, size_parameter_case2 )
    {
        size_t n = 42;
        segment_tree< int, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }

    TEST( sum_int_segment_tree_constructor, size_parameter_case3 )
    {
        size_t n = 42000;
        segment_tree< int, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], NULL);
        }
    }

    TEST( sum_int_segment_tree_constructor, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<int> parameter_array(n, 0);
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST( sum_int_segment_tree_constructor, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST( sum_int_segment_tree_constructor, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<int> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }


    // Tests for range_query()

    TEST( sum_int_segment_tree_rquery, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 1;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).sum;
        }

        std::vector<int> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);
        
        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }
    }

    TEST( sum_int_segment_tree_rquery, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 420;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).sum;
        }

        std::vector<int> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);
        
        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }
    }

    TEST( sum_int_segment_tree_rquery, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);
        
        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 4200;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).sum;
        }

        std::vector<int> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }
    }


    // Test for point_update()
    /*TEST( sum_int_segment_tree_pupdate, vector_parameter_case )
    {
        size_t n = 42000;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);

        for (int index = 0; index < (int)n; index += 1000)
        {
            segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

            int lowest_value_yet = -index;
            segtree.point_update(index, lowest_value_yet);

            size_t m = 420;
            std::vector<std::pair<int, int>> queries;
            fill_with_random_intervals(n, m, index, queries);

            std::vector<int> segment_tree_results(m);
            for (int i = 0; i < (int)m; i++)
            {
                ASSERT_LE(queries[i].first, queries[i].second);
                ASSERT_LT(queries[i].first, n);
                ASSERT_LT(queries[i].second, n);
                ASSERT_GE(queries[i].first, 0);
                ASSERT_GE(queries[i].second, 0);
                ASSERT_LE(queries[i].first, index);
                ASSERT_GE(queries[i].second, index);
                segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).sum;
            }

            for (int i = 0; i < (int)m; i++)
            {
                EXPECT_EQ(segment_tree_results[i], -index);
            }
        }
    }*/
}
