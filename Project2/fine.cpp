//Ethan Ngo
//UID : 205416130
//Section 1H

#include <iostream>
#include <string>
using namespace std;

int main() {
	/*constant variables that set the base fine, the percentage multiplier for the first 40 thousand
	and an amount above 250 thousand*/
	const double BASE_FINE = 20.0, FIRST_40K = 0.66, ABOVE_250K = 0.14;
	/*constant variables that will be used to check what "bracket" the amount paid is in (less than 40 thousand,
	between 40 thousand and 250 thousand, and more than 250 thosand)*/
	const double FIRST_CHECK = 40, SECOND_CHECK = 250;
	//badOutput specifically will be set to a string if the user inputs something incorrectly
	string defendantName, fakeAthlete, badOutput;
	/*next210K specifically is the percent multiplier that can't be constant because it can change depending on if
	they faked being an athlete or not*/
	double amountPaid, totalFine, next210K = 0.1;

	//user inputs
	cout << "Defendant: ";
	getline(cin, defendantName);

	cout << "Amount paid (in thousands): ";
	cin >> amountPaid;
	cin.ignore(10000, '\n');

	cout << "Fake athlete? (y/n): ";
	getline(cin, fakeAthlete);

	//checking the inputs to see if any inputs are incorrect and setting badOutput to a string if it is the case
	if (defendantName == "") {
		badOutput = "You must enter a defendant name.";
	}
	else if (amountPaid < 0) {
		badOutput = "The amount paid must not be negative.";
	}
	else if (fakeAthlete != "y" && fakeAthlete != "n") {
		badOutput = "You must enter y or n.";
	}
	else {
		badOutput = "";
	}

	//changing the percent multiplier if the defendant faked being an athlete
	if (fakeAthlete == "y") next210K = 0.22;

	//calculating the total fine
	totalFine = BASE_FINE; //setting total fine to the base fine regardless of the amount paid
	if ((FIRST_CHECK - amountPaid) >= 0) { //will execute if the amount paid is less than 40 thousand
		totalFine += amountPaid * FIRST_40K; //adding the (amount paid * 0.66) to the total fine
	}
	else { //will execute if the amount paid is more than 40 thousand
		if ((SECOND_CHECK - amountPaid) >= 0) { //will execute if the amount paid is less than 250 thousand
			totalFine += FIRST_CHECK * FIRST_40K; //adding (40,000 * 0.66) to the total fine
			totalFine += (amountPaid - FIRST_CHECK) * next210K; //adding (remaining amount * (0.1 or 0.22)) to the total fine
		}
		else { //will execute if the amount paid is more than 250 thousand
			totalFine += FIRST_CHECK * FIRST_40K; //adding (40,000 * 0.66) to the total fine
			totalFine += (SECOND_CHECK - FIRST_CHECK) * next210K; //adding (210,000 * (0.1 or 0.22)) to the total fine
			totalFine += (amountPaid - SECOND_CHECK) * ABOVE_250K; //adding (remaining amount * 0.14) to the total fine
		}
	}

	//one decimal point
	cout.setf(ios::fixed);
	cout.precision(1);

	//final output with an "error" message or the suggested fine if everything was input correctly
	cout << "---";
	cout << endl;

	if (badOutput != "") {
		cout << badOutput;
	}
	else {
		cout << "The suggested fine for " << defendantName << " is $" << totalFine << " thousand.";
	}
}