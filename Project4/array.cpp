//Ethan Ngo
//UID : 205416130
//Section 1A

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);


int main()
{
	string appending[3] = { "", "", "" };
	//to make sure nothing happens when appending 0 of the strings
	assert((appendToAll(appending, 0, "!")) == 0 && appending[0] == "" && appending[2] == "");

	//to make sure that appending works
	assert((appendToAll(appending, 1, "$$")) == 1 && appending[0] == "$$");

	//to make sure that appending works and that the last string wasn't affected by the other run of the function above
	assert((appendToAll(appending, 3, "%")) == 3 && appending[0] == "$$%" && appending[2] == "%");

	//no -1 for array size
	assert((appendToAll(appending, -1, "$@%")) == -1);

	string lookingup[5] = { "a", "b", "c","", "d" };
	//to make sure lookup works with different array sizes
	assert((lookup(lookingup, 5, "")) == 3);
	assert((lookup(lookingup, 1, "a")) == 0);
	assert((lookup(lookingup, 3, "b")) == 1);

	//to make sure that lookup doesn't see the values past the given array size
	assert((lookup(lookingup, 2, "c")) == -1);

	//no -1 for array size
	assert((lookup(lookingup, -1, "b")) == -1);

	string posMax[5] = { "b", "d", "a","", "e" };
	//to make sure positionOfMax works with different position values
	assert((positionOfMax(posMax, 5)) == 4);
	assert((positionOfMax(posMax, 3)) == 1);

	//no 0 or -1 for array size
	assert((positionOfMax(posMax, -1)) == -1);
	//this is because that would mean no interesting elements
	assert((positionOfMax(posMax, 0)) == -1);

	string left[5] = { "a", "b", "c","", "d" };
	//to test rotateLeft with different array sizes to ensure not all strings are affected if they are not meant to be touched
	assert((rotateLeft(left, 5, 1)) == 1 && left[0] == "a" && left[2] == ""); //now {"a", "c","", "d", "b"}
	assert((rotateLeft(left, 4, 2)) == 2 && left[2] == "d"); //now {"a", "c","d", "", "b"}

	//no -1 for array size
	assert((rotateLeft(left, -1, 2)) == -1);
	//no size of 0 because the pos will always be greater than or equal to the array size and you can't rotate left like that
	assert((rotateLeft(left, 0, 0)) == -1);

	string runs[9] = { "m", "m", "r", "a", "f", "f", "f", "m", "m" };
	//to test countRuns with different array sizes
	assert(countRuns(runs, 9) == 5);
	assert(countRuns(runs, 5) == 4);
	assert(countRuns(runs, 2) == 1);

	//to test a size of 0, which has a run of 1, because "" is a string a consecutive elements
	assert(countRuns(runs, 0) == 1);

	//no -1 for array size
	assert(countRuns(runs, -1) == -1);

	string f[5] = { "a", "b", "c","", "d" };
	//to test flip with different array sizes
	assert(flip(f, 5) == 5 && f[0] == "d" && f[3] == "b"); //now {"d", "", "c", "b", "a"}
	assert(flip(f, 3) == 3 && f[0] == "c" && f[1] == ""); //now {"c", "", "d", "b", "a"}

	//to test that flip doesn't affect the strings if the array size is 0 because you are flipping all 0 elements
	assert(flip(f, 0) == 0 && f[0] == "c" && f[3] == "b");

	//no -1 for array size
	assert(flip(f, -1) == -1);

	string d1[7] = { "a", "", "f", "m", "r", "n", "d" };
	string d2[6] = { "a", "", "f", "d", "m", "r" };
	//to test differ with different array sizes and if one runs out before the other, return the smaller value if they were the same
	//up until that point
	assert(differ(d1, 7, d2, 6) == 3);
	assert(differ(d1, 2, d2, 3) == 2);

	//to test to make sure that if one of the values is 0, it'll just return 0 because that is the position of the only similar
	//elements for both arrays
	assert(differ(d1, 0, d2, 3) == 0);
	assert(differ(d1, 2, d2, 0) == 0);

	//no -1 for array size
	assert(differ(d1, -1, d2, 6) == -1);
	assert(differ(d1, 2, d2, -1) == -1);

	string sub1[6] = { "g", "ma", "n", "mi", "a", "l" };
	string sub2[3] = { "ma", "n", "mi" };
	//to test that subsequence works and if the elements of the second array is not contiguous somewhere in the other array, return -1
	assert(subsequence(sub1, 6, sub2, 3) == 1);
	assert(subsequence(sub1, 3, sub2, 3) == -1);

	//to test that a size of 0 is a subsequence of any array, at position 0
	assert(subsequence(sub1, 3, sub2, 0) == 0);
	assert(subsequence(sub1, 0, sub2, 3) == 0);

	//no -1 for array size
	assert(subsequence(sub1, -1, sub2, 3) == -1);
	assert(subsequence(sub1, 3, sub2, -1) == -1);

	string any[6] = { "g", "ma", "n", "mi", "a", "l" };
	string any1[4] = { "d", "a", "mi", "ma" };
	//to test lookupAny with varying array sizes to make sure it works and return -1 when an element is not in the other array
	assert(lookupAny(any, 6, any1, 4) == 1);
	assert(lookupAny(any, 6, any1, 1) == -1);

	//to test that looking up an array with no interesting elements would return -1
	assert(lookupAny(any, 0, any1, 1) == -1);
	assert(lookupAny(any, 6, any1, 0) == -1);

	//no -1 for array size
	assert(lookupAny(any, -1, any1, 4) == -1);
	assert(lookupAny(any, 6, any1, -1) == -1);

	string s[6] = { "d", "l", "m", "r", "f", "a" };
	string s1[4] = { "ma", "n", "l", "mi" };
	//to test separate
	assert(separate(s, 6, "g") == 3);

	//to test separate with a separator value that is one of the elements
	assert(separate(s1, 4, "mi") == 2);

	//to test when 0 is the size of the array
	assert(separate(s1, 0, "mi") == 0);

	//no -1 for array size
	assert(separate(s, -1, "g") == -1);

	cout << "All tests succeeded" << endl;
}

int appendToAll(string a[], int n, string value) {
	if (n < 0)
		return -1;
	for (int i = 0; i < n; i++) {
		a[i] += value;
	}
	return n;
}

int lookup(const string a[], int n, string target) {
	if (n < 0)
		return -1;
	for (int i = 0; i < n; i++) {
		if (a[i] == target)
			return i;
	}
	return -1;
}

int positionOfMax(const string a[], int n) {
	if (n <= 0)
		return -1;
	int latestInOrder = 0;
	for (int i = 1; i < n; i++) {
		if (a[i] > a[latestInOrder])
			latestInOrder = i;
	}
	return latestInOrder;
}

int rotateLeft(string a[], int n, int pos) {
	if (n <= 0)
		return -1;
	string temp = a[pos];
	for (int i = pos; i < n - 1; i++) {
		a[i] = a[i + 1];
	}
	a[n - 1] = temp;
	return pos;
}

int countRuns(const string a[], int n) {
	if (n < 0)
		return -1;
	int count = 1;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (a[i] != a[j]) {
				count++;
				break;
			}
			i++;
		}
	}
	return count;
}

int flip(string a[], int n) {
	if (n < 0)
		return -1;
	string temp;
	for (int i = 0; i < ((n + 1) / 2); i++) {
		temp = a[i];
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
	return n;
}
int differ(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0)
		return -1;
	bool check = (n1 <= n2);
	int count;
	if (check)
		count = n1;
	else
		count = n2;
	for (int i = 0; i < count; i++) {
		if (a1[i] != a2[i])
			return i;
	}

	if (!check)
		return n2;
	else
		return n1;
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0)
		return -1;
	if (n2 == 0 || n1 == 0)
		return 0;
	for (int i = 0; i < n1; i++) {
		if (a2[0] == a1[i]) {
			for (int j = 0; j < n2; j++) {
				if (a2[j] != a1[i + j] || (i+j >= n1))
					return -1;
			}
			return i;
		}
	}
	return 0;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2) {
	if (n1 <= 0 || n2 <= 0)
		return -1;
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < n2; j++) {
			if (a1[i] == a2[j])
				return i;
		}
	}
	return -1;
}

int separate(string a[], int n, string separator) {
	if (n < 0)
		return -1;
	string temp;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (a[i] > a[j]) {
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
	for (int k = 0; k < n; k++) {
		if (a[k] >= separator) {
			return k;
		}
	}
	return n;
}
