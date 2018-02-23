#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <cassert>
#include <climits>

#include "segment_tree.hpp"

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

void runRMQOnSegmentTree(const std::vector<int>& ar, const std::vector<std::pair<int, int>>& queries, std::vector<int>& results)
{
    size_t n = ar.size();
    segment_tree <int, node, set_default_value, merge> st(ar);
    size_t m = queries.size();
    for (int i = 0; i < m; i++)
    {
        results[i] = (st.range_query(queries[i].first, queries[i].second))->min;
    }
    st.point_update(n - 1, -1);
}

void runBruteForce(const std::vector<int>& ar, const std::vector<std::pair<int, int>>& queries, std::vector<int>& results)
{
	size_t n = ar.size();
    size_t m = queries.size();
	std::vector<std::vector<int>> minInInterval(n, std::vector<int>(n));
	for (int i = 0; i < n; i++)
	{
		minInInterval[i][i] = ar[i];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
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
	for (int i = 0; i < m; i++)
	{
		results[i] = minInInterval[queries[i].first][queries[i].second];
	}
}

void runRootNMethod(const std::vector<int>& ar, const std::vector<std::pair<int, int>>& queries, std::vector<int>& results)
{
    size_t n = ar.size();
    size_t m = queries.size();
	int rootN = (int) sqrt(n);
    size_t numOfIntervals = n / rootN;
	if (n % rootN != 0)
	{
		numOfIntervals++;
	}
	std::vector<int> minInInterval(numOfIntervals, INT_MAX);

	for (int i = 0; i < n; i++)
	{
		if (ar[i] < minInInterval[i / rootN])
		{
			minInInterval[i / rootN] = ar[i];
		}
	}

	for (int i = 0; i < m; i++)
	{
		int lo = queries[i].first;
		int hi = queries[i].second;
		int min = INT_MAX;
		while (lo % rootN != 0)
		{
			if (ar[lo] < min)
			{
				min = ar[lo];
			}
			lo++;
		}
		while ((hi + 1) % rootN != 0)
		{
			if (ar[hi] < min)
			{
				min = ar[hi];
			}
			hi--;
		}
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
		results[i] = min;
	}
}

bool resultsAreConcordant(const std::vector<int>& results1, const std::vector<int>& results2)
{
    size_t m1 = results1.size();
    size_t m2 = results2.size();
	assert(m1 == m2);
	for (int i = 0; i < m1; i++)
	{
		if (results1[i] != results2[i])
		{
			return false;
		}
	}
	return true;
}

int main()
{
	int n;
	std::cout << "Enter number of elements" << std::endl;
	std::cin >> n;
	std::vector<int> ar(n);
	std::cout << "Enter " << n << " space-separated elements" << std::endl;
	for (int i = 0; i < n; i++)
	{
		std::cin >> ar[i];
	}
	int m;
	std::cout << "Enter number of queries" << std::endl;
	std::cin >> m;
	std::vector<std::pair<int, int>> queries(m);
	std::cout << "Enter " << m << " (0-indexed) intervals" << std::endl;
	for (int i = 0; i < m; i++)
	{
		std::cin >> queries[i].first >> queries[i].second;
		if (queries[i].first < 0 || queries[i].second >= n || queries[i].first > queries[i].second)
		{
			std::cout << "Invalid interval entered, please try again" << std::endl;
			i--;
		}
	}

	std::vector<int> segmentTreeResults(m);
	std::vector<int> bruteForceResults(m);
	std::vector<int> rootNMethodResults(m);

	std::cout << "Processing queries using a segment tree..." << std::endl;
	runRMQOnSegmentTree(ar, queries, segmentTreeResults);
	std::cout << "Done." << std::endl;

	std::cout << "Processing queries using brute force..." << std::endl;
	runBruteForce(ar, queries, bruteForceResults);
	std::cout << "Done." << std::endl;

	std::cout << "Processing queries using Root-N Method..." << std::endl;
	runRootNMethod(ar, queries, rootNMethodResults);
	std::cout << "Done." << std::endl;

    if (resultsAreConcordant(bruteForceResults, segmentTreeResults))
    {
        std::cout << "Brute force check passed" << std::endl;
    }
    else
    {
        std::cout << "Brute force check failed" << std::endl;
        for (int i = 0; i < m; i++)
        {
            std::cout << bruteForceResults[i] << " " << segmentTreeResults[i] << std::endl;
        }
    }

    if (resultsAreConcordant(segmentTreeResults, rootNMethodResults))
    {
        std::cout << "Root-N Method check passed" << std::endl;
    }
    else
    {
        std::cout << "Root-N Method check failed" << std::endl;
        for (int i = 0; i < m; i++)
        {
            std::cout << rootNMethodResults[i] << " " << segmentTreeResults[i] << std::endl;
        }
    }

	return 0;
}