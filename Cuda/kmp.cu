% % cu
#include <bits/stdc++.h>
	using namespace std;
#define M 1024

void computeLPS(string pat, int m, int lps[])
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

__global__ void kmpSearch(char *txt, char *pat, int n, int m, int lps[], int res[])
{
	int pid = blockIdx.x * blockDim.x + threadIdx.x;
	int cp;
	int start = pid, end = pid + m;
	int t = end - start + 1;
	if (pid < n - m)
	{
		int i = pid, flag = 0, j = 0;
		while (i < start + t - 1)
		{
			if (txt[i] == pat[j])
			{
				i++;
				j++;
			}
			else
			{
				if (j != 0)
					cp = lps[j - 1];
				i++;
			}
			if (j == m)
			{
				flag = 1;
				cp = lps[j - 1];
			}
		}
		res[pid] = flag;
	}
}

int main()
{
	int blocks[] = {1, 1, 1, 1, 1, 1, 1, 10, 20, 30, 40, 50, M / 8, M / 4, M / 2, M, M, M, M, M};
	int threads[] = {1, 10, 20, 30, 40, 50, M, 10, 10, 10, 10, 10, M, M, M, M / 8, M / 4, M / 2, M};

	//Randomly generating DNA Sequence;
	vector<char> list = {'a', 'g', 'c', 't'};
	int n = 1000000; //Enter number of characters
	cout << "\nDNA Sequence is Randomly Generated\n";
	srand(time(0));
	{
		ofstream cout("dna.txt");
		for (long i = 0; i < n; i++)
		{
			int random = rand() % 4;
			cout << list[random];
		}
	}
	char txt[n + 1];
	char *d_txt, *d_pat; //device copies
	int *d_lps, *d_res;
	string s, p = "agctac"; //Enter the pattern

	//reading dna sequence from file
	{
		ifstream cin("dna.txt");
		cin >> s;
	}
	strcpy(txt, s.c_str());

	int m = p.size();
	char pat[m + 1];
	strcpy(pat, p.c_str());

	cout << endl;
	cout << "DNA Sequence : " << s << endl;
	cout << "Pattern      : " << p << endl;

	int *lps;
	int res[n] = {0};
	lps = new int[m]; //longest proper prefix suffix
	computeLPS(pat, m, lps);
	cout << "\nLength of DNA Sequence : " << n << endl;
	cout << "Length of Pattern      : " << m << endl
		 << endl;

	// Allocate space for device copies of txt, pat,lps and res
	cudaMalloc((void **)&d_txt, n * sizeof(char));
	cudaMalloc((void **)&d_pat, m * sizeof(char));
	cudaMalloc((void **)&d_lps, m * sizeof(int));
	cudaMalloc((void **)&d_res, n * sizeof(int));

	cudaMemcpy(d_txt, &txt, n * sizeof(char), cudaMemcpyHostToDevice);
	cudaMemcpy(d_pat, &pat, m * sizeof(char), cudaMemcpyHostToDevice);
	cudaMemcpy(d_lps, &lps, m * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_res, &res, n * sizeof(int), cudaMemcpyHostToDevice);

	for (int k = 0; k < 19; k++)
	{
		float elapsed = 0;
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

		cudaEventRecord(start, 0);
		kmpSearch<<<blocks[k], threads[k]>>>(d_txt, d_pat, n, m, d_lps, d_res);

		// Copy result back to host
		cudaError err = cudaMemcpy(&res, d_res, n * sizeof(int), cudaMemcpyDeviceToHost);
		if (err != cudaSuccess)
			cout << "CUDA Error copying to Host :" << cudaGetErrorString(err) << endl;

		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		cudaEventElapsedTime(&elapsed, start, stop);

		cudaEventDestroy(start);
		cudaEventDestroy(stop);

		printf("Blocks = %4d and Threads per Block = %4d Time = %.5f\n", blocks[k], threads[k], elapsed);
	}

	// Cleanup
	cudaFree(d_txt);
	cudaFree(d_pat);
	cudaFree(d_lps);
	cudaFree(d_res);
	int cnt = 0;
	cout << "\n\n*********** Final Results ***********\n";
	for (int i = 0; i < n; i++)
		if (res[i] != 0)
			cnt++;
	cout << "\nTotal number of matches = " << cnt << endl
		 << endl;
	int counter = 0;
	for (int i = 0; i < n; i++)
	{
		if (res[i] != 0)
		{
			counter++;
			cout << "Match - " << counter << " to '" << pat << "' found at : " << i << " - " << i + m - 1 << endl;
		}
	}

	return 0;
}