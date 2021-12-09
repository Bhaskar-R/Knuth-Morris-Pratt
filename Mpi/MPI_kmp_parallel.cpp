#include <bits/stdc++.h>
#include "mpi.h"
using namespace std;
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

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

void kmpSearch(string txt, string pat, int start, int end, vector<int> &lps, set<int> &cnt)
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
			// cout << "Pattern found at Index : " << i - j << endl;
			j = lps[j - 1];
		}
	}
}

int main(int argc, char *argv[])
{
	int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k, rc;
	string txt, pat = "agctat";
	//reading dna sequence from file
	{
		ifstream cin("dna.txt");
		cin >> txt;
	}
	//cout << "Enter Pattern to be searched : ";
	//cin >> pat;
	int n = txt.size();
	int m = pat.size();
	set<int> cnt;
	vector<int> lps(m, 0); //longest proper prefix suffix
	computeLPS(pat, m, lps);
	// cout << "\nLength of DNA Sequence : " << n << endl;
	// cout << "Length of Pattern      : " << m << endl<< endl;
	int start_search = 0, end_search = n;

	MPI_Status status;
	double start, end;
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	if (numtasks < 2)
	{
		printf("Need atleast two MPI tasks. Quiting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	char pro_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(pro_name, &name_len);
	numworkers = numtasks - 1;

	// Master Task:
	if (taskid == MASTER)
	{
		averow = n / numworkers;
		extra = n % numworkers;
		mtype = FROM_MASTER;
		kmpSearch(txt, pat, 0, averow, lps, cnt);
		for (dest = 1; dest <= numworkers; dest++)
		{
			start_search = (dest * averow) - m;
			if (start_search < 0)
				start_search = 0;
			end_search = (dest * averow) + averow + 1;
			if (end_search > n)
				end_search = n;

			MPI_Send(&start_search, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&end_search, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
		}
		if (extra != 0)
		{
			start_search = (numworkers + 1) * averow;
			if (start_search < 0)
				start_search = 0;
			kmpSearch(txt, pat, start_search, n, lps, cnt);
		}

		// receive from worker:
		mtype = FROM_WORKER;
		for (i = 1; i <= numworkers; i++)
		{
			source = i;
			MPI_Recv(&start_search, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
			MPI_Recv(&end_search, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
		}
		end = MPI_Wtime();

		cout << "\nFor No. of Threads = " << numtasks << " Time= " << end - start << endl;
	}

	// Worker task:
	if (taskid > MASTER)
	{
		mtype = FROM_MASTER;
		MPI_Recv(&start_search, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&end_search, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

		kmpSearch(txt, pat, start_search, end_search, lps, cnt);

		mtype = FROM_WORKER;
		MPI_Send(&start_search, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&end_search, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	cout << "\nTotal number of matches = " << cnt.size() << endl
		 << endl;
	int counter = 0;
	for (auto i : cnt)
	{
		counter++;
		cout << "Match - " << counter << " to '" << pat << "' found at : " << i << " - " << i + m - 1 << endl;
	}
}