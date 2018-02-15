#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <cassert>
#include <climits>

void runBruteForce(const std::vector<int>& ar, const std::vector<std::pair<int, int>>& queries, std::vector<int>& results)
{
	int n = ar.size();
	int m = queries.size();
	std::vector<std::vector<int>> minInInterval(n, std::vector<int>(n));
	for (int i = 0; i < n; i++)
	{
		minInInterval[i][i] = i;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (ar[j] < ar[minInInterval[i][j - 1]])
			{
				minInInterval[i][j] = j;
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
	int n = ar.size();
	int m = queries.size();
	int rootN = (int) sqrt(n);
	int numOfIntervals = n / rootN;
	if (n % rootN != 0)
	{
		numOfIntervals++;
	}
	std::vector<int> minInInterval(numOfIntervals, -1);

	for (int i = 0; i < n; i++)
	{
		if (minInInterval[i / rootN] == -1 || ar[i] < ar[minInInterval[i / rootN]])
		{
			minInInterval[i / rootN] = i;
		}
	}

	for (int i = 0; i < m; i++)
	{
		int lo = queries[i].first;
		int hi = queries[i].second;
		int min = INT_MAX;
		int minIndex = -1;
		while (lo % rootN != 0)
		{
			if (ar[lo] < min)
			{
				min = ar[lo];
				minIndex = lo;
			}
			lo++;
		}
		while ((hi + 1) % rootN != 0)
		{
			if (ar[hi] < min)
			{
				min = ar[hi];
				minIndex = hi;
			}
			hi--;
		}
		int index = lo / rootN;
		int end = hi / rootN;
		while (index <= end)
		{
			if (ar[minInInterval[index]] < min)
			{
				min = ar[minInInterval[index]];
				minIndex = minInInterval[index];
			}
			index++;
		}
		results[i] = minIndex;
	}
}

bool resultsAreConcordant(const std::vector<int>& results1, const std::vector<int>& results2)
{
	int m1 = results1.size();
	int m2 = results2.size();
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

	/*std::cout << "Processing queries using a segment tree..." << std::endl;
	runRMQOnSegmentTree(ar, queries, segmentTreeResults);
	std::cout << "Done." << std::endl;*/

	std::cout << "Processing queries using brute force..." << std::endl;
	runBruteForce(ar, queries, bruteForceResults);
	std::cout << "Done." << std::endl;

	std::cout << "Processing queries using root N method (refer to documentation for more information)..." << std::endl;
	runRootNMethod(ar, queries, rootNMethodResults);
	std::cout << "Done." << std::endl;

	if (resultsAreConcordant(bruteForceResults, rootNMethodResults))
	{
		std::cout << "Check passed" << std::endl;
	}
	else
	{
		std::cout << "Check failed" << std::endl;
		for (int i = 0; i < m; i++)
		{
			std::cout << bruteForceResults[i] << " " << rootNMethodResults[i] << std::endl;
		}
	}

	return 0;
}