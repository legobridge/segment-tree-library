#ifndef SEGMENT_TREE
#define SEGMENT_TREE

#include <vector>
#include <boost/optional.hpp>

template <  typename        T,
            typename        node,
            void            (*set_default_value)(node&, T),
            node           (*merge)(node*, node*)    >
class segment_tree
{
public:
    segment_tree( size_t N )
        : ar_size(N)
        , ar(ar_size)
        , tree(4 * ar_size + 2)
        , left(4 * ar_size + 2)
        , right(4 * ar_size + 2)
    {
        init_left_right(1, 0, ar_size - 1);
    }

    segment_tree( const std::vector<T>& init_ar )
        : ar_size(init_ar.size())
        , ar(init_ar)
        , tree(4 * ar_size + 2)
        , left(4 * ar_size + 2)
        , right(4 * ar_size + 2)
    {
        init_left_right(1, 0, ar_size - 1);
        construct_tree(1);
    }

    void construct_tree( const std::vector<T>& init_ar )
    {
        ar = init_ar;
        construct_tree(1);
    }

    size_t get_array_size()
    {
        return ar_size;
    }

    std::vector<T>& get_array()
    {
        return ar;
    }

    node range_query( int lo, int hi )
    {
        return *range_query(1, lo, hi);
    }

    void point_update( int index, T new_value )
    {
        ar[index] = new_value;
        update_tree_over_point(1, index);
    }

private:
    size_t ar_size;
    std::vector<T> ar;
    std::vector<node> tree;
    std::vector<int> left;
    std::vector<int> right;

    void init_left_right( int node_index, int lo, int hi )
    {
        left[node_index] = lo;
        right[node_index] = hi;

        if (lo != hi)
        {
            int mid = lo + (hi - lo) / 2;
            init_left_right(2 * node_index, lo, mid);
            init_left_right(2 * node_index + 1, mid + 1, hi);
        }
    }

    void construct_tree( int node_index )
    {
        int lo = left[node_index];
        int hi = right[node_index];
        if (lo == hi)
        {
            set_default_value(tree[node_index], ar[lo]);
        }
        else
        {
            construct_tree(2 * node_index);
            construct_tree(2 * node_index + 1);
            tree[node_index] = merge(&tree[2 * node_index], &tree[2 * node_index + 1]);
        }
    }

    boost::optional<node> range_query( int node_index, int lo, int hi )
    {
        // Interval doesn't intersect at all
        if (lo > right[node_index] || hi < left[node_index])
        {
            return boost::none;
        }

        // Interval completely contained
        if (lo <= left[node_index] && hi >= right[node_index])
        {
            return tree[node_index];
        }

        // Interval partially intersects
        boost::optional<node> left_solution = range_query(2 * node_index, lo, hi);
        boost::optional<node> right_solution = range_query(2 * node_index + 1, lo, hi);
        if (!right_solution && !left_solution)
        {
            return boost::none;
        }
        if (!left_solution && right_solution)
        {
            return *right_solution;
        }
        if (!right_solution && left_solution)
        {
            return *left_solution;
        }
        node ls = *left_solution;
        node rs = *right_solution;
        return merge(&ls, &rs);
    }

    void update_tree_over_point( int node_index, int index )
    {
        // Interval doesn't contain index
        if (index > right[node_index] || index < left[node_index])
        {
            return;
        }

        // Interval has converged to index
        if (left[node_index] == right[node_index])
        {
            set_default_value(tree[node_index], ar[index]);
            return;
        }

        // Interval contains index
        update_tree_over_point(2 * node_index, index);
        node* left_solution = &tree[2 * node_index];
        update_tree_over_point(2 * node_index + 1, index);
        node* right_solution = &tree[2 * node_index + 1];
        tree[node_index] = merge(left_solution, right_solution);
    }
};

#endif
