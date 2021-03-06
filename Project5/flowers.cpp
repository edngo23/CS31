#pragma warning(disable:6262)
#define _CRT_SECURE_NO_DEPRECATE

#include "utilities.h"
#include <iostream>
#include <cstring>
using namespace std;

const int MAXINPUT = 100;
const int MAXWORDS = 10'000;
const char wordFile[] = "twowords.txt";

void totalPlay(const char words[][MAXWORDLEN + 1], int nWords, int rounds);
int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
int calc(const char words[][MAXWORDLEN + 1], const char input[MAXINPUT], int wordnum, bool flower);
bool testInput(const char words[][MAXWORDLEN + 1], const char input[MAXINPUT]);
int main() {
	char wordList[MAXWORDS][MAXWORDLEN + 1];
	int numWords = getWords(wordList, MAXWORDS, wordFile);
	if (numWords < 1) {
		cout << "No words were loaded, so I can't play the game.";
		return 0;
	}
	int rounds;
	cout << "How many rounds do you want to play? ";
	cin >> rounds;
	cin.ignore(10000, '\n');

	if (rounds < 1) {
		cout << "The number of rounds must be positive.";
		return 0;
	}

	cout.setf(ios::fixed);
	cout.precision(2);

	totalPlay(wordList, numWords, rounds);
}

void totalPlay(const char words[][MAXWORDLEN + 1], int nWords, int rounds) {
	int tries, minScore = 1'000'000'000, maxScore = 0, wordnum;
	double totalScore = 0;
	for (int i = 1; i <= rounds; i++) {
		wordnum = randInt(0, nWords - 1);
		cout << endl << "Round " << i << endl;
		cout << "The mystery word is " << strlen(words[wordnum]) <<" letters long." << endl;
		tries = playOneRound(words, nWords, wordnum);
		if (tries == 1)
			cout << "You got it in 1 try." << endl;
		else
			cout << "You got it in " << tries << " tries." << endl;
		totalScore += tries;
		if (tries < minScore)
			minScore = tries;
		if (tries > maxScore)
			maxScore = tries;
		cout << "Average: " << totalScore/i <<", minimum: " << minScore << ", maximum: " << maxScore << endl;
	}
}

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum) {
	int counter = 0;
	if (nWords < 1 || wordnum < 0 || wordnum >= nWords)
		return -1;
	char trialWord[MAXINPUT];
	while (trialWord != words[wordnum]) {
		cout << "Trial word: ";
		cin.getline(trialWord, 100);
		if (testInput(words, trialWord)) {
			counter++;
			if (!strcmp(trialWord, words[wordnum]))
				break;
			else
				cout << "Flowers: " << calc(words, trialWord, wordnum, true) << ", Bees: " << calc(words, trialWord, wordnum, false)
					 << endl;
		}
	}
	return counter;
}

int calc(const char words[][MAXWORDLEN + 1], const char input[MAXINPUT], int wordnum, bool flower) {
	int flowerCount = 0;
	int beeCount = 0;
	for (int i = 0; i < MAXWORDLEN - 1; i++) {
		if (input[i] == words[wordnum][i]) {
			flowerCount++;
		}
		else {
			for (int j = 0; j < MAXWORDLEN - 1; j++) {
				if (input[i] == words[wordnum][j]) {
					beeCount++;
					break;
				}
			}
		}
	}
	if (flower)
		return flowerCount;
	else
		return beeCount;
}

bool testInput(const char words[][MAXWORDLEN + 1], const char input[MAXINPUT]) {
	bool lowerCaseLetter = true;
	bool inWordList;
	if (strlen(input) >= 4 && strlen(input) <= 6){
		for (int i = 0; i < strlen(input); i++) {
			if (!islower(input[i]))
				lowerCaseLetter = false;
		}
		if (!lowerCaseLetter) {
			cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
			return false;
		}
	}
	else {
		cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
		return false;
	}
	for (int i = 0; i < MAXWORDS; i++) {
		inWordList = true;
		for (int j = 0; j < MAXWORDLEN - 1; j++) {
			if (input[j] != words[i][j]) {
				inWordList = false;
				break;
			}
		}
		if (inWordList)
			break;
	}
	if (!inWordList) {
		cout << "I don't know that word." << endl;
		return false;
	}
	return true;
}
