#ifndef SEGMENT_TREE
#define SEGMENT_TREE

#include <vector>

template <typename T, typename node, void (*set_default_value)(node&, T), node* (*merge)(node*, node*)>
class segment_tree
{
    public:
    segment_tree(size_t N)
        : n(N)
        , tree(4 * n + 2)
        , left(4 * n + 2)
        , right(4 * n + 2)
    {
        init_left_right(1, 0, n - 1);
    }

    segment_tree(const std::vector<T>& ar)
        : n(ar.size())
        , tree(4 * n + 2)
        , left(4 * n + 2)
        , right(4 * n + 2)
    {
        init_left_right(1, 0, n - 1);
        construct_tree(1, ar);
    }

    node* range_query(int lo, int hi)
    {
        return range_query(1, lo, hi);
    }

    bool point_update(int index, T new_value)
    {
        return false;
    }

    private:
    size_t n;
    std::vector<node> tree;
    std::vector<int> left;
    std::vector<int> right;

    void init_left_right(int node_index, int lo, int hi)
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

    void construct_tree(int node_index, const std::vector<T>& ar)
    {
        int lo = left[node_index];
        int hi = right[node_index];
        if (lo == hi)
        {
            set_default_value(tree[node_index], ar[lo]);
        }
        else
        {
            construct_tree(2 * node_index, ar);
            construct_tree(2 * node_index + 1, ar);
            tree[node_index] = *merge(&tree[2 * node_index], &tree[2 * node_index + 1]);
        }
    }

    node* range_query(int node_index, int lo, int hi)
    {
        // Interval doesn't intersect at all
        if (lo > right[node_index] || hi < left[node_index])
        {
            return nullptr;
        }

        // Interval completely contained
        if (lo <= left[node_index] && hi >= right[node_index])
        {
            return &tree[node_index];
        }

        node* left_solution = range_query(2 * node_index, lo, hi);
        node* right_solution = range_query(2 * node_index + 1, lo, hi);

        if (left_solution == nullptr)
        {
            return right_solution;
        }
        if (right_solution == nullptr)
        {
            return left_solution;
        }
        return merge(left_solution, right_solution);
    }
};

#endif