/*
 *  Members -
 *  2328 - Shruti Datar
 *  2329 - Samruddhi Deode
 *  2332 - Nisha Deshmukh
 *  2336 - Yamini Dongaonkar
 */

/*
 * citizen.h
 *
 *  Created on: 06-May-2021
 *
 *  Includes the Citizen and CitizenQueueList class for
 *  storing the data of the citizens.
 */

#ifndef CITIZEN_H_
#define CITIZEN_H_

#include <string>
using namespace std;

// ==========================================================================================
// Class declarations

class Citizen
{
	int citizen_id; // Personal details (data)
	string name;
	int age;
	char sex;
	bool comorbidities;

	string slot_date;

	// Implementation details
	int priority; // For the calculation details, see the get_priority function of this class
	string password;
	bool demoted; // True if the citizen has been relegated to the end of the queue-list for missing their alloted slot.
	Citizen *next = nullptr;

public:
	// Visibility flags - Modified only by running allotment process
	bool visible;	 // True when citizen can see their slot
	bool slot_today; // True when the slot allocated is today

	Citizen(string nm, int cit_id, char sx, int ag, bool comor, string pswd)
	{
		/*
    		 * Constructor
    		 */
		citizen_id = cit_id;
		name = nm;
		age = ag;
		sex = sx;
		comorbidities = comor;

		slot_date = "Not Alloted Yet.";

		visible = false;
		slot_today = false;

		password = pswd;
		priority = (visible * 10000) + (comorbidities * 1000) + age;
		demoted = false;
	}

	int get_priority();
	bool is_pswd(string pswd);
	string get_details(string pswd);

	friend class Admin; // class Admin in "Admin.h" header
	friend class CitizenQueueList;
};

class CitizenQueueList
{
	Citizen *rear;
	Citizen *front;
	void display_top_n(unsigned n);
	void display_today();
	void remove(Citizen *user, string pswd);
	Citizen *dequeue();

public:
	CitizenQueueList()
	{
		/*
    	 * Constructor
    	 */
		front = nullptr;
		rear = nullptr;
	}

	void enqueue(Citizen *p);
	Citizen *search(int citizenID);
	int get_min_priority();

	friend class Admin;
};

// ===================================================================================================================

// Citizen implementation
int Citizen::get_priority()
{
	/*
	 * PRIORITY CALCULATION :
	 * The rules of priority calculation are :
	 * 1.If a citizen has been allocated a slot, they cannot be overriden
	 *   by someone who has not been allocated a slot.
	 * 2.Citizens with comorbidities have a higher priority than those without.
	 * 3.Older citizens have higher priority.
	 * 4.Demoted citizens' priorities depend on the priority of the last person in the queue-list
	 * 	 when they were demoted and on their 'visible' member.
	 *
	 * This function updates and returns the priority of a citizen.
	 *
	 */
	if (!demoted)
		priority = (visible * 10000) + (comorbidities * 1000) + age;
	else
		priority = (visible * 10000) + priority;
	return priority;
}

bool Citizen::is_pswd(string pswd)
{
	/*
	 * Verifies the citizen's password.
	 * Returns true if the password matches.
	 */
	return password == pswd;
}

string Citizen::get_details(string pswd)
{
	/*
	 * Returns string representation of the citizen's details.
	 * Protected by password.
	 */
	string details;
	if (is_pswd(pswd))
	{
		details += "\n\n\tCitizenID : ";
		details += to_string(citizen_id);
		details += "\n\n\t";
		details += "Name : ";
		details += name;
		details += "\n\n\t";
		details += "Age : ";
		details += to_string(age);
		details += "\n\n\t";
		details += "Sex : ";
		details += sex;
		details += "\n\n\t";
		details += "Slot Date : ";
		details += slot_date;
		details += "\n\n\t";
		return details;
	}
	return "Access Denied.";
}

// =====================================================================================================
// CitizenQueueList implementation

Citizen *CitizenQueueList::search(int citizenID)
{
	/*
	 *  Returns the pointer to the citizen if found,
	 *  otherwise returns null.
	 */
	Citizen *cur = front;
	while (cur != nullptr)
	{
		if (cur->citizen_id == citizenID)
			break;
		cur = cur->next;
	}
	return cur;
}

void CitizenQueueList::enqueue(Citizen *new_citizen)
{
	/*
	 * Adds citizen to the queue-list as per priority.
	 */
	if (front == nullptr) //First registration
	{
		front = new_citizen;
		rear = new_citizen;
	}
	else
	{
		Citizen *cur = front;
		Citizen *prev = nullptr;

		while (cur != nullptr && cur->get_priority() >= new_citizen->get_priority()) // Finding the location for insertion
		{
			prev = cur;
			cur = cur->next;
		}
		if (prev == nullptr) // Appending to the head
		{
			new_citizen->next = front;
			front = new_citizen;
		}
		else if (cur == nullptr) //Appending to the rear
		{
			prev->next = new_citizen;
			rear = new_citizen;
		}
		else // Appending in the middle
		{
			prev->next = new_citizen;
			new_citizen->next = cur;
		}
	}
}

Citizen *CitizenQueueList::dequeue()
{
	/*
	 * Removes and returns the front of the queue-list
	 */
	Citizen *top = front;
	if (front == rear)
	{
		front = nullptr;
		rear = nullptr;
	}
	else
	{
		front = front->next;
		top->next = nullptr;
	}
	return top;
}

void CitizenQueueList::remove(Citizen *user, string pswd)
{
	/*
	 * Allows the removal of a person from the queue-list.
	 * Can only be accessed with the person's consent.
	 */
	if (!user->is_pswd(pswd)) // Prevents malicious calls of this function from affecting the queue-list, enforcing the person's consent.
		return;

	Citizen *cur = front;
	Citizen *prev = nullptr;
	while (cur->citizen_id != user->citizen_id && cur != nullptr)
	{ // Finding the person in the queue-list
		prev = cur;
		cur = cur->next;
	}
	if (prev == nullptr) // Removing from the front
		front = cur->next;
	else // Removing from other positions
		prev->next = cur->next;
	if (rear == cur) // Removing from rear
		rear = prev;
	cur->next = nullptr;
	delete (cur);
}

void CitizenQueueList::display_top_n(unsigned n)
{
	/*
	 * Displays CitizenID, Name and Slot Details
	 * of the top n entries of the queue-list if they exist.
	 */
	Citizen *user = front;
	unsigned count;
	for (count = 1; count <= n && user != nullptr; count++)
	{
		cout << "\n\t" << count << ")\t CitizenID: " << user->citizen_id << "\n\t\t Name:" << user->name << "\n\t\t Slot Date:" << user->slot_date << endl;
		user = user->next;
	}
	if (count - 1 < n)
		cout << "Only " << (count - 1) << " entries in queue-list" << endl;
}

void CitizenQueueList::display_today()
{
	/*
	 * Displays CitizenID, Name and Slot Details of
	 * citizens with today's slot.
	 */
	Citizen *user = front;
	int count = 1;
	while (user != NULL && user->slot_today)
	{
		cout << "\n\t" << count << ")\t CitizenID:" << user->citizen_id << "\n\t\t Name:" << user->name << "\n\t\t Slot Date:" << user->slot_date << endl;
		user = user->next;
		count++;
	}
	if (count == 1)
		cout << "\n\tNo citizens in today's slot." << endl;
}

int CitizenQueueList::get_min_priority()
{
	/*
	 * Returns the priority of the rear element (the lowest priority).
	 * Useful for moving citizens to the bottom of the queue-list.
	 * Used in allotment process in Admin class.
	 */
	return rear->get_priority();
}

// ===================================================================================================
#endif /* CITIZEN_H_ */
