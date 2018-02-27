#include "pch.h"
#include "segment_tree.hpp"

#include <string>
#include <vector>
#include <utility>
#include <random>

namespace max_string_query
{
    // Structures and methods for testing the lexicographically
    // maximum string query form of the segment tree
    struct node
    {
        std::string max;
    };

    void set_default_value( node& x, std::string y )
    {
        x.max = y;
    }

    node merge( node* a, node* b )
    {
        if (a->max >= b->max)
        {
            return *a;
        }
        return *b;
    }
    
    // Generates an array of n random strings composed of lowercase letters
    // Each string is arbitrarily defined to be 6 characters long
    void fill_with_random_strings( size_t n, std::vector<std::string>& parameter_array )
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(97, 122);
        for (int i = 0; i < (int)n; i++)
        {
            std::string s;
            for (int j = 0; j < 6; j++)
            {
                s += (char)dis(gen);
            }
            parameter_array[i]= s;
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

    // Brute force method to solve the problem of finding
    // the lexicographically largest string in a range
    void run_brute_force( const std::vector<std::string>& ar, 
                          const std::vector<std::pair<int, int>>& queries, 
                          std::vector<std::string>& results )
    {
        size_t n = ar.size();
        size_t m = queries.size();
        std::vector<std::vector<std::string>> max_in_interval(n, std::vector<std::string>(n));
        for (int i = 0; i < (int)n; i++)
        {
            max_in_interval[i][i] = ar[i];
        }
        for (int i = 0; i < (int)n; i++)
        {
            for (int j = i + 1; j < (int)n; j++)
            {
                if (ar[j] > max_in_interval[i][j - 1])
                {
                    max_in_interval[i][j] = ar[j];
                }
                else
                {
                    max_in_interval[i][j] = max_in_interval[i][j - 1];
                }
            }
        }
        for (int i = 0; i < (int)m; i++)
        {
            results[i] = max_in_interval[queries[i].first][queries[i].second];
        }
    }

    // Root-N method to solve the problem of finding
    // the lexicographically largest string in a range
    void run_root_N_method( const std::vector<std::string>& ar,
                            const std::vector<std::pair<int, int>>& queries,
                            std::vector<std::string>& results )
    {
        size_t n = ar.size();
        size_t m = queries.size();
        int rootN = (int)sqrt(n);
        size_t numOfIntervals = n / rootN;
        if (n % rootN != 0)
        {
            numOfIntervals++;
        }
        std::vector<std::string> max_in_interval(numOfIntervals, "");

        for (int i = 0; i < (int)n; i++)
        {
            if (max_in_interval[i / rootN] == "" || ar[i] > max_in_interval[i / rootN])
            {
                max_in_interval[i / rootN] = ar[i];
            }
        }

        for (int i = 0; i < (int)m; i++)
        {
            int lo = queries[i].first;
            int hi = queries[i].second;
            std::string max = "";
            while (lo % rootN != 0 && lo <= hi)
            {
                if (max == "" || ar[lo] > max)
                {
                    max = ar[lo];
                }
                lo++;
            }
            while ((hi + 1) % rootN != 0 && hi >= lo)
            {
                if (max == "" || ar[hi] > max)
                {
                    max = ar[hi];
                }
                hi--;
            }
            if (lo <= hi)
            {
                int index = lo / rootN;
                int end = hi / rootN;
                while (index <= end)
                {
                    if (max_in_interval[index] > max)
                    {
                        max = max_in_interval[index];
                    }
                    index++;
                }
            }
            results[i] = max;
        }
    }


    // Tests for both types of constructors

    TEST( max_string_segment_tree_constructor, size_parameter_case1 )
    {
        size_t n = 1;
        segment_tree< std::string, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
    }

    TEST( max_string_segment_tree_constructor, size_parameter_case2 )
    {
        size_t n = 42;
        segment_tree< std::string, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
    }

    TEST( max_string_segment_tree_constructor, size_parameter_case3 )
    {
        size_t n = 42000;
        segment_tree< std::string, node, set_default_value, merge > segtree(n);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
    }

    TEST( max_string_segment_tree_constructor, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<std::string> parameter_array(n);
        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST( max_string_segment_tree_constructor, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);
        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }

    TEST( max_string_segment_tree_constructor, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);
        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);
        EXPECT_EQ(segtree.get_array_size(), n);
        std::vector<std::string> array = segtree.get_array();
        EXPECT_EQ(array.size(), n);
        for (int i = 0; i < (int)array.size(); i++)
        {
            EXPECT_EQ(array[i], parameter_array[i]);
        }
    }


    // Tests for range_query()

    TEST( max_string_segment_tree_rquery, vector_parameter_case1 )
    {
        size_t n = 1;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);
        
        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 1;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<std::string> segment_tree_results(m);

        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
        }

        for (int i = 0; i < (int)m; i++)
        {
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).max;
        }

        std::vector<std::string> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }
    }

    TEST( max_string_segment_tree_rquery, vector_parameter_case2 )
    {
        size_t n = 42;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);
        
        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 420;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<std::string> segment_tree_results(m);

        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
        }

        for (int i = 0; i < (int)m; i++)
        {
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).max;
        }

        std::vector<std::string> brute_force_results(m);
        run_brute_force(parameter_array, queries, brute_force_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(brute_force_results[i], segment_tree_results[i]);
        }

        std::vector<std::string> root_N_results(m);
        run_root_N_method(parameter_array, queries, root_N_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(root_N_results[i], segment_tree_results[i]) << i;
        }
    }

    TEST( max_string_segment_tree_rquery, vector_parameter_case3 )
    {
        size_t n = 42000;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);

        segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);

        size_t m = 42000;
        std::vector<std::pair<int, int>> queries;
        fill_with_random_intervals(n, m, queries);

        std::vector<std::string> segment_tree_results(m);

        for (int i = 0; i < (int)m; i++)
        {
            ASSERT_LE(queries[i].first, queries[i].second);
            ASSERT_LT(queries[i].first, n);
            ASSERT_LT(queries[i].second, n);
            ASSERT_GE(queries[i].first, 0);
            ASSERT_GE(queries[i].second, 0);
        }

        for (int i = 0; i < (int)m; i++)
        {
            segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).max;
        }

        std::vector<std::string> root_N_results(m);
        run_root_N_method(parameter_array, queries, root_N_results);

        for (int i = 0; i < (int)m; i++)
        {
            EXPECT_EQ(root_N_results[i], segment_tree_results[i]);
        }
    }


    // Test for point_update()
    TEST( max_string_segment_tree_pupdate, vector_parameter_case )
    {
        size_t n = 42000;
        std::vector<std::string> parameter_array(n);
        fill_with_random_strings(n, parameter_array);

        for (int index = 0; index < (int)n; index += 1000)
        {
            segment_tree< std::string, node, set_default_value, merge > segtree(parameter_array);

            std::string highest_value = "zzzzzz";
            segtree.point_update(index, highest_value);

            size_t m = 420;
            std::vector<std::pair<int, int>> queries;
            fill_with_random_intervals(n, m, index, queries);

            std::vector<std::string> segment_tree_results(m);
            for (int i = 0; i < (int)m; i++)
            {
                ASSERT_LE(queries[i].first, queries[i].second);
                ASSERT_LT(queries[i].first, n);
                ASSERT_LT(queries[i].second, n);
                ASSERT_GE(queries[i].first, 0);
                ASSERT_GE(queries[i].second, 0);
                ASSERT_LE(queries[i].first, index);
                ASSERT_GE(queries[i].second, index);
            }

            for (int i = 0; i < (int)m; i++)
            {
                segment_tree_results[i] = (segtree.range_query(queries[i].first, queries[i].second)).max;
            }

            for (int i = 0; i < (int)m; i++)
            {
                EXPECT_EQ(segment_tree_results[i], "zzzzzz");
            }
        }
    }
}
