/*
 *  DSA Mini-Project : Vaccine Registration System
 *  A2 Comp SY 2020-2021
 *
 *  Members -
 *  2328 - Shruti Datar
 *  2329 - Samruddhi Deode
 *  2332 - Nisha Deshmukh
 *  2336 - Yamini Dongaonkar
 */
/*
 * UserInterface.cpp
 *
 *  Created on: 05-May-2021
 *
 *  This is the source file of this project.
 *  It provides most of the user interface
 *  required for this application.
 *
 */

#include <iostream>
#include <string>
#include "citizen.h"
#include "admin.h"
using namespace std;

// Function declarations
void registration();
void user_login();
void admin_login();

CitizenQueueList cqueue; // global - The queue-list object which holds all the data of registered citizens

int main()
{
	int choice = -1;
	do
	{
		cout << "\n\t----------------------" << endl;
		cout << "\n\t MAIN MENU" << endl; // MAIN MENU
		cout << "\n\t1. Register for vaccine." << endl;
		cout << "\n\t2. Login as user." << endl;
		cout << "\n\t3. Login as Admin." << endl;
		cout << "\n\tEnter 0 to exit." << endl;
		cout << "\n\t----------------------" << endl;
		cout << "\n\t Enter choice :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			registration();
			break;
		case 2:
			user_login();
			break;
		case 3:
			admin_login();
			break;
		case 0:
			cout << "\n\t\tThank you for using App." << endl;
			break;
		default:
			cout << "\n\t\tThe choice you entered was not valid," << endl;
			cout << "\n\t\tplease enter a valid option." << endl;
			break;
		}
	} while (choice);

	return 0;
}
// ==============================================================================================

void registration()
{
	string name;
	int age, id;
	char ch, sex;
	bool comorbidities;
	cout << "\n\t Enter your name: ";
	cin >> name;
	cout << "\n\t Enter citizen ID: ";
	cin >> id;

	Citizen *found = cqueue.search(id);
	if (found != nullptr)
	{
		cout << "\n\t\tCitizen with this citizen id already registered ! " << endl;
		return;
	}

	// New registration details
	do // Validation
	{
		cout << "\n\t Enter Sex: (m/f)";
		cin >> sex;
		if (sex != 'm' && sex != 'f')
			cout << "\n\t\tInvalid Input!";
		else
			break;
	} while (true);

	cout << "\n\t Enter your age: ";
	cin >> age;
	if (Admin::age_eligibility > age)
	{
		cout << "\n\t\tSorry Cannot Register! You are underage! ";
		return;
	}

	do // Validation
	{
		cout << "\n\t Do you have any comorbidities? (y/n)";
		cin >> ch;
		if (ch != 'y' && ch != 'n')
			cout << "\n\t\tInvalid Input!";
		else
		{
			comorbidities = (ch == 'y') ? true : false;
			break;
		}
	} while (true);

	string pswd, pswd2;
	do
	{ // Password validation
		cout << "\n\t Set your password: ";
		cin >> pswd;
		cout << "\n\t Confirm password: ";
		cin >> pswd2;
		if (pswd != pswd2)
			cout << "\n\t Password does not match! Set up your password again!";
	} while (pswd != pswd2);

	Citizen *new_citizen = new Citizen(name, id, sex, age, comorbidities, pswd); // Creating citizen node
	cqueue.enqueue(new_citizen);
	cout << "\n\t\tRegistered successfully, please log in to see your details." << endl;
}

// ==============================================================================================

void user_login()
{
	/*
	 *  This function is responsible for the
	 *  login experience of a regular user (citizen)
	 */
	void admin_authorise(Citizen * user, string pass); // Function which is called when Admin authorisation is required

	int citizenID;
	cout << "\n\t Enter your Citizen ID :";
	cin >> citizenID;
	Citizen *user = cqueue.search(citizenID);
	if (user == nullptr) // User not found
	{
		cout << "\n\t\tSorry, please register first or" << endl;
		cout << "\n\t\tenter a valid Citizen ID." << endl;
		return;
	}
	else
	{
		int count = 3; // Attempts to enter the correct password
		string pswd;
		while (count != 0)
		{
			cout << "\n\t Enter your password :";
			cin >> pswd;
			if (!user->is_pswd(pswd)) // Checking the entered password
			{
				count--; // Incorrect attempt reduces the count
				cout << "\n\t\tIncorrect password." << endl;
				cout << "\n\t\t" << count << " attempts remaining." << endl;
			}
			else
				break;
		}
		if (count == 0) // Failed to enter the correct password
			return;

		cout << "\n\tLogged in successfully." << endl;

		// Display user details
		cout << "\n\t Citizen details :";
		cout << user->get_details(pswd) << endl;
		cout << "----------------------" << endl;
		// User Menu
		cout << "\t USER MENU :" << endl;
		if (user->slot_today) // Facility available only on vaccination day
			cout << "\n\t1. Confirm vaccination completion. " << endl;
		cout << "\n\tEnter 0 to logout." << endl;
		cout << "----------------------" << endl;
		bool vaccinated = false; // Used for terminating the user login after completion of vaccination
		int choice = -1;
		do
		{
			cout << "\n\t Enter choice :";
			cin >> choice;
			switch (choice)
			{

			case 1:
				if (user->slot_today)
				{
					cout << "\n\t\tPlease provide authorisation for confirming the vaccination" << endl; // This step requires admin authorisation
					admin_authorise(user, pswd);
					vaccinated = true;
				}
				else
					cout << "\n\t\tPlease enter a valid choice." << endl;
				break;
			case 0:
				cout << "\n\t\tSigned out." << endl;
				return;
			default:
				cout << "\n\t\tPlease enter a valid choice." << endl;
			}
		} while (choice != 0 && !vaccinated);
	}
}

void admin_authorise(Citizen *user, string pass)
{
	/*
	 * Helper to the user_login function, it enables
	 * Admin authorisation for de-registration of the user.
	 * Note: The parameter 'string pass' is required by the Admin::remove() function
	 * which is used here.
	 */
	Admin admin;
	int count = 3; // Attempts to enter the correct password
	string admin_pswd;
	while (count != 0)
	{
		cout << "\n\tEnter admin password :";
		cin >> admin_pswd;
		if (!admin.verify_password(admin_pswd))
		{
			count--; // Count decremented after incorrect attempt.
			cout << "\n\t\tIncorrect password." << endl;
			cout << "\n\t " << count << " attempts remaining." << endl;
		}
		else
			break;
	}
	if (count == 0) // Admin authorisation not provided
	{
		cout << "\n\tSorry, you are not authorised." << endl;
		return;
	}
	admin.remove(user, pass, cqueue); // De-registration of the user
	cout << "\n\tCongratulations on getting vaccinated !" << endl;
	cout << "\n\tYou've been signed out and your account has been removed." << endl;
	cout << "\n\tThank you for using the App" << endl;
}

// ===============================================================================================

void admin_login()
{
	/*
	 * Provides interface for Admin to execute the vaccination allotment process
	 * and some other administrative tasks.
	 */

	void display_handler(Admin & ad); // Function declaration - used in handling the display of registrations

	Admin admin;
	int count = 3; // Attepmts to enter the correct password
	string pswd;
	while (count != 0)
	{
		cout << "\n\tEnter your password :";
		cin >> pswd;
		if (!admin.verify_password(pswd))
		{
			count--; // Decrement attempts after incorrect entry
			cout << "\n\tIncorrect password!";
			cout << "\n\t" << count << " attempts remaining." << endl;
		}
		else
			break;
	}
	if (count == 0) // Login authorisation not provided
		return;
	cout << "\n\t Logged in successfully! " << endl;

	bool proc_executed;
	int choice;
	do
	{
		// Admin Menu
		cout << "\n\t--------------------------";
		cout << "\n\t ADMIN MENU " << endl;
		cout << "\n\t1.Execute time allotment process" << endl;
		cout << "\n\t2.Display registrations." << endl;
		cout << "\n\t3.Change number of available shots." << endl;
		cout << "\n\t4.Change minimum age requirement for vaccination" << endl;
		cout << "\n\tEnter 0 to logout." << endl;
		choice = -1;
		cout << "\n\tEnter your choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			proc_executed = admin.run_process(cqueue); // Refer to Admin class
			if (!proc_executed)
				cout << "\n\tCannot run allotment multiple times on the same day." << endl;
			else
				cout << "\n\t\t*** Allotment complete ***" << endl;
			break;
		case 2:
			display_handler(admin);
			break;
		case 3:
			admin.change_available_shots();
			break;
		case 4:
			admin.change_eligibility(cqueue);
			break;
		case 0:
			cout << "\n\t Signed out!" << endl;
			return;
		default:
			cout << "\n\tPlease enter a valid choice." << endl;
		}
	} while (choice != 0);
}

void display_handler(Admin &ad)
{
	int choice = -1;
	do
	{
		cout << "\n\t---------------------------";
		cout << "\n\t Display menu" << endl;
		cout << "\n\tEnter 1 for displaying n entries" << endl;
		cout << "\n\tEnter 2 for displaying citizens in today's slot" << endl;
		cout << "\n\tEnter 0 to return to main Admin Menu." << endl;
		cout << "\n\tEnter choice :";
		cin >> choice;
		switch (choice)
		{
		case 1:
			ad.display_top_n(cqueue);
			break;
		case 2:
			ad.display_today(cqueue);
			break;
		case 0:
			cout << "\n\tReturning to main Admin menu ..." << endl;
			break;
		default:
			cout << "\n\tEnter a valid input.";
			break;
		}
	} while (choice != 0);
}

// ==============================================================================================
