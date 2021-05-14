/*
 * 	Members -
 *  2328 - Shruti Datar
 *  2329 - Samruddhi Deode
 *  2332 - Nisha Deshmukh
 *  2336 - Yamini Dongaonkar
 */

/*
 * dateutil.h
 *
 *  Created on: 11-May-2021
 *
 *  Includes some helpers for handling dates.
 *  Used in Admin class in admin.h, in the run_process function
 */

#ifndef DATEUTIL_H_
#define DATEUTIL_H_

#include <string>
using namespace std;

bool isLeap(int yr)
{
	/*
	 * Returns true if the year is a leap year
	 */
	return yr % 4 == 0 && yr % 100 != 0;
}
string date_string(int day, int month, int year)
{
	/*
	 * Returns the string form of a date
	 */
	return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
}
string get_next_date(int day, int month, int year)
{
	/*
	 * Returns the next date in string form, given input
	 * today's day, month and year
	 */
	int new_day, new_month, new_year;
	enum months
	{
		Jan = 1,
		Feb,
		Mar,
		Apr,
		May,
		Jun,
		Jul,
		Aug,
		Sep,
		Oct,
		Nov,
		Dec
	};
	bool ordinary = false;
	new_year = year;
	switch (day)
	{
	case 28:
		if (month == Feb && !isLeap(year)) // 28th Feb, non leap year
		{
			new_month = Mar;
			new_day = 1;
		}
		else
			ordinary = true;
		break;
	case 29:
		if (month == Feb) // 29th Feb, leap year
		{
			new_month = Mar;
			new_day = 1;
		}
		else
			ordinary = true;
		break;
	case 30:
		if (month == Apr || month == Jun || month == Sep || month == Nov) // 30 day months
		{
			new_month = month + 1;
			new_day = 1;
		}
		else
			ordinary = true;
		break;
	case 31:
		if (month == Dec) // 31st Dec
		{
			new_month = Jan;
			new_year++;
			new_day = 1;
		}
		else // Other 31 day months
		{
			new_month = month + 1;
			new_day = 1;
		}
		break;
	default:
		ordinary = true;
		break;
	}

	if (ordinary) // Month does not change; day is incremented by 1
	{
		new_month = month;
		new_day = day + 1;
	}

	return date_string(new_day, new_month, new_year);
}
#endif /* DATEUTIL_H_ */
