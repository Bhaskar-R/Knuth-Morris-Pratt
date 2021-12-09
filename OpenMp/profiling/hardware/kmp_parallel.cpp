#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
set<int> cnt;

void computeLPS(string pat, int m, vector<int> &lps)
{
	int len = 0, i = 1;
	lps[0] = 0;
	while (i < m)
	{
		if (pat[i] == pat[len])
		{
			lps[i] = len + 1;
			len++;
			i++;
		}
		else
		{
			if (len != 0)
				len = lps[len - 1];
			else
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}

void kmpSearch(string txt, string pat, int start, int end, vector<int> &lps)
{
	int n = end - start + 1, m = pat.size();
	int i = start, j = 0;
	while (i < start + n - 1)
	{
		if (txt[i] == pat[j])
		{
			i++;
			j++;
		}
		else
		{
			if (j != 0)
				j = lps[j - 1];
			else
				i++;
		}
		if (j == m)
		{
			cnt.insert(i - j);
			cout << "Pattern found at Index : " << i - j << endl;
			j = lps[j - 1];
		}
	}
}

int main()
{
	double startTime, endTime;
	vector<int> NUM_THREADS = {1, 2, 4, 6, 8, 10, 12, 16, 20, 24, 32, 64};
	vector<double> rtime;
	string txt, pat;

	//Randomly generating DNA Sequence;
	vector<char> list = {'a', 'g', 'c', 't'};
	long nc;
	cout << "\nDNA Sequence is Randomly Generated\n\nEnter No. of Characters in DNA : ";
	cin >> nc;
	srand(time(0));
	{
		ofstream cout("dna.txt");
		for (long i = 0; i < nc; i++)
		{
			int random = rand() % 4;
			cout << list[random];
		}
	}

	//reading dna sequence from file
	{
		ifstream cin("dna.txt");
		cin >> txt;
	}

	cout << "Enter Pattern to be searched : ";
	cin >> pat;

	int n = txt.size();
	int m = pat.size();
	vector<int> lps(m, 0); //longest proper prefix suffix
	computeLPS(pat, m, lps);
	cout << "\nLength of DNA Sequence : " << n << endl;
	cout << "Length of Pattern      : " << m << endl
		 << endl;
	int tid, start = 0, end = n;
	for (int i = 0; i < NUM_THREADS.size(); i++)
	{
		cout << "\n\n---------- Number of Threads = " << NUM_THREADS[i] << " ----------" << endl
			 << endl;
		int len_thread = n / NUM_THREADS[i];
		int rem = n % NUM_THREADS[i];
		startTime = omp_get_wtime();

#pragma omp parallel num_threads(NUM_THREADS[i]) private(tid, start, end) shared(txt, pat, rem, len_thread, m)
		{
			int nthreads = omp_get_num_threads();
			tid = omp_get_thread_num();
			if (tid == 0)
			{
#pragma omp critical(part1)
				{
					start = tid;
					end = len_thread - 1;
					cout << "tid = " << tid << " Block : " << start << " - " << end << endl;
					kmpSearch(txt, pat, start, end, lps);
				}
			}
			else
			{
#pragma omp critical(part2)
				{
					start = (tid * len_thread) - m;
					if (start < 0)
						start = 0;
					end = (tid * len_thread) + len_thread + 1;
					if (end > n)
						end = n;
					cout << "tid = " << tid << " Block : " << start << " - " << end << endl;
					kmpSearch(txt, pat, start, end, lps);
				}
			}
		}
		if (rem != 0)
		{
			cout << "\nIn rem Block : " << (NUM_THREADS[i] + 1) * len_thread << " - " << n << endl;
			kmpSearch(txt, pat, (NUM_THREADS[i] + 1) * len_thread, n, lps);
		}
		endTime = omp_get_wtime();
		rtime.push_back(endTime - startTime);
	}
	cout << "\n\n*********** Final Results ***********\n\n";
	for (int i = 0; i < NUM_THREADS.size(); i++)
		cout << "For No. of Threads = " << NUM_THREADS[i] << " Time = " << rtime[i] << endl;
	cout << "\nTotal number of matches = " << cnt.size() << endl
		 << endl;
	int counter = 0;
	for (auto i : cnt)
	{
		counter++;
		cout << "Match - " << counter << " to '" << pat << "' found at : " << i << " - " << i + m - 1 << endl;
	}

	return 0;
}