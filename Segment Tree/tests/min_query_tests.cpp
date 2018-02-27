#include "pch.h"
#include "segment_tree.hpp"

#include <vector>
#include <utility>
#include <random>
#include <climits>

namespace min_int_query
{
    // Structures and methods for testing the 
    // minimum integer query form of the segment tree
    struct node
    {
        int min;
    };

    void set_default_value( node& x, int y )
    {
        x.min = y;
    }

    node* merge( node* a, node* b )
    {
        if (a->min <= b->min)
        {
            return a;
        }
        return b;
    }
    
    // Generates an array of n random integers ranging from 1 to n
    void fill_with_random_integers( size_t n, std::vector<int>& parameter_array )
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, n);
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

    // Brute force method to solve RMQ problem
    void run_brute_force( const std::vector<int>& ar, 
                          const std::vector<std::pair<int, int>>& queries, 
                          std::vector<int>& results )
    {
        size_t n = ar.size();
        size_t m = queries.size();
        std::vector<std::vector<int>> minInInterval(n, std::vector<int>(n));
        for (int i = 0; i < (int)n; i++)
        {
            minInInterval[i][i] = ar[i];
        }
        for (int i = 0; i < (int)n; i++)
        {
            for (int j = i + 1; j < (int)n; j++)
            {
                if (ar[j] < minInInterval[i][j - 1])
                {
                    minInInterval[i][j] = ar[j];
                }
                else
                {
                    minInInterval[i][j] = minInInterval[i][j - 1];
                }
            }
        }
        for (int i = 0; i < (int)m; i++)
        {
            results[i] = minInInterval[queries[i].first][queries[i].second];
        }
    }

    // Root-N method to solve the RMQ problem
    void run_root_N_method( const std::vector<int>& ar,
                            const std::vector<std::pair<int, int>>& queries,
                            std::vector<int>& results )
    {
        size_t n = ar.size();
        size_t m = queries.size();
        int rootN = (int)sqrt(n);
        size_t numOfIntervals = n / rootN;
        if (n % rootN != 0)
        {
            numOfIntervals++;
        }
        std::vector<int> minInInterval(numOfIntervals, INT_MAX);

        for (int i = 0; i < (int)n; i++)
        {
            if (ar[i] < minInInterval[i / rootN])
            {
                minInInterval[i / rootN] = ar[i];
            }
        }

        for (int i = 0; i < (int)m; i++)
        {
            int lo = queries[i].first;
            int hi = queries[i].second;
            int min = INT_MAX;
            while (lo % rootN != 0 && lo <= hi)
            {
                if (ar[lo] < min)
                {
                    min = ar[lo];
                }
                lo++;
            }
            while ((hi + 1) % rootN != 0 && hi >= lo)
            {
                if (ar[hi] < min)
                {
                    min = ar[hi];
                }
                hi--;
            }
            if (lo <= hi)
            {
                int index = lo / rootN;
                int end = hi / rootN;
                while (index <= end)
                {
                    if (minInInterval[index] < min)
                    {
                        min = minInInterval[index];
                    }
                    index++;
                }
            }
            results[i] = min;
        }
    }


    // Tests for both types of constructors

    TEST( min_int_segment_tree_constructor, size_parameter_case1 )
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

    TEST( min_int_segment_tree_constructor, size_parameter_case2 )
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

    TEST( min_int_segment_tree_constructor, size_parameter_case3 )
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

    TEST( min_int_segment_tree_constructor, vector_parameter_case1 )
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

    TEST( min_int_segment_tree_constructor, vector_parameter_case2 )
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

    TEST( min_int_segment_tree_constructor, vector_parameter_case3 )
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

    TEST( min_int_segment_tree_rquery, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);

        size_t m = 1;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second))->min;
        }

        std::vector<int> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }
    }

    TEST( min_int_segment_tree_rquery, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);

        size_t m = 42000;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second))->min;
        }

        std::vector<int> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }

        std::vector<int> root_N_results(m);
        run_root_N_method(parameter_array, queries, root_N_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(root_N_results[i], segment_tree_results[i]) << i;
        }
    }

    TEST( min_int_segment_tree_rquery, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<int> parameter_array;
        fill_with_random_integers(n, parameter_array);

        size_t m = 42000;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        segment_tree< int, node, set_default_value, merge > segtree(parameter_array);

        std::vector<int> segment_tree_results(m);
        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second))->min;
        }

        std::vector<int> root_N_results(m);
        run_root_N_method(parameter_array, queries, root_N_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(root_N_results[i], segment_tree_results[i]);
        }
    }


    // Test for point_update()
    TEST( min_int_segment_tree_pupdate, vector_parameter_case )
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
                segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second))->min;
            }

            for (int i = 0; i < (int)m; i++)
            {
                EXPECT_EQ(segment_tree_results[i], -index);
            }
        }
    }
}