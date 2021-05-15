# DSA Mini-Project : Vaccine Registration System

## A2 Comp SY 2020-2021

## Members -
      2328 - Shruti Datar
      2329 - Samruddhi Deode
      2332 - Nisha Deshmukh
      2336 - Yamini Dongaonkar

## Problem Statement: 
      C++ Program to manage the roll out of Covid-19 vaccine where an eligible user can
      register for his vaccine dose, be allotted his timeslot, take the vaccine, and leave after he/she is done.


## Following are the functionalities that we have implemented:
      1. Registration of a new User.
      2. Assign priority to the user upon registration and enqueue him.
      3. Run time slot allotment process.
      4. Dequeue the User upon vaccination.
      5. Login functionality for the User and Administrator.
      6. Get self data (for user)
      7. Get data of any citizen (for admin)


## The code has been divided into 3 major components :
      1. The Citizen class which holds the data of the registered citizens and CitizenQueueList,
         the encompassing data structure which maintains priority and smoothens the allotment process.
      2. The Admin class provides the functionalities required by a system administrator,
         including running the allotment process, and changing certain parameters which influence the
         allotment, including age eligibility and vaccine availability. 
         The class also has some limited measure of privilege on citizen data.
      3. The main source file contains the user interface to the application and facilitates registration,
         and citizen and admin login. 


## Some of the important functions of our code are as follows:

### 1. registration(): 
    The user is prompted to enter his personal details. 
    Here we check if the user is eligible for the vaccination depending upon his age. 
    If he is eligible, the user is passed onto the enqueue function.


### 2. enqueue(): 
    The user is enqueued according to his vaccination priority. 
    get_priority() is a helper function which calculates the priority of the user depending upon his age
    and whether or not he has any comorbidities.
    Eg.A person with comorbidities and the highest age amongst the current registered users will have
    the highest priority and will be appended to the front.


### 3.  admin_login(): 
    Admin is prompted to enter his password and upon authentication has access to 
    various functionalities of the Admin Class. 
    He can set an available_slots variable depending on the vaccines that the vaccination center 
    is going to receive and also change the age eligibility for the vaccination depending on 
    the government guidelines. 
    The most important functionality of the Admin class is run_process().


### 4. run_process(): 
    Using the ctime library, we have calculated the date on which the process is being run.
    Citizens are allotted slots only for the next day. 
    This function makes use of helper functions from the dateutil header files to calculate the next date.


### 5. user_login(): 
    This function enables the user to login and check his allotted time slot. 
    He is expected to log in on the day of his vaccination and confirm his vaccination. 
    Upon being vaccinated he is dequeued. 
    If a user fails to do so, he is demoted to the end of the queue and his priority decremented. 
    This process is implemented with the help of dequeue() and remove() functions respectively.


### 6.Extra:
    Other functionalities of the Admin and the Citizen class are looking up the list of citizens who are  
    supposed to get vaccinated today and looking up the top 'n' entries from the vaccination queue.
   
   
