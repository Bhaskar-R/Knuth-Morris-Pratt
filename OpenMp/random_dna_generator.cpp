#include <bits/stdc++.h>
using namespace std;

int main()
{
	vector<char> list = {'a', 'g', 'c', 't'};
	long n;
	cout << "No. of Characters : ";
	cin >> n;
	freopen("dna.txt", "w", stdout);
	srand(time(0));
	for (long i = 0; i < n; i++)
	{
		int random = rand() % 4;
		cout << list[random];
	}
	return 0;
}