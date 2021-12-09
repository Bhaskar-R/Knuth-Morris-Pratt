#include <bits/stdc++.h>
// #include <omp.h>
using namespace std;

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

void kmpSearch(string txt, string pat)
{
	int n = txt.size(), m = pat.size();
	vector<int> lps(m, 0); //longest proper prefix suffix
	computeLPS(pat, m, lps);
	int i = 0, j = 0;
	int flag = 0;
	while (i < n)
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
			flag = 1;
			cout << "Pattern found at Index : " << i - j << endl;
			j = lps[j - 1];
		}
	}
	if (flag == 0)
		cout << "\nPattern Not found" << endl;
}

int main()
{
	string txt, pat;
	cout << "Enter Pattern to be searched : ";
	cin >> pat;
	// freopen("dna.txt", "r", stdin);
	cout << "Enter Text : ";
	cin >> txt;
	// double start = omp_get_wtime();
	kmpSearch(txt, pat);
	// double end = omp_get_wtime();
	// cout << "Time : " << end - start << endl;
	return 0;
}