#include <iostream>
#include <stdlib.h>
#include <queue>
#include <iomanip>
#include <functional>
#include "math.h"
#include "Event.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*Driver Code which will use the class Event to to organize the Customer Arrivals at a Bank,
with one one teller and processes the arrivals and then processes departure of the customer. Once the 
simulation ends, it will calculate the total transactions, the average time of the transactions 
for each customer and the avaerage wait time for each customer.*/
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


unsigned currentTime = 0; //Current time of the simulation
bool tellerAvailable = true; //Checks to see if the Teller is available or not
double waitTime = 0; //Keeps track of the wait time for each customer

// Processses an arrival event.
void processArrival(Event arrivalEvent, priority_queue<Event, vector<Event>, greater<Event>>& eventListPQueue, queue<Event>& bankQueue)
  {
      // Remove this event from the event list
     eventListPQueue.pop();
     //Assign the customer's arrival event 
     Event customer = arrivalEvent;
     
     cout<<left<< "Processing   arrival event at time: ";
     cout<<right<< setw(4)<<customer.getTime() << setw(5) << customer.getDuration() <<endl;
     //Is the Bank Queue is empty and the Bank Teller is Available then calculates Departure time 
     //Adds the Departure event into the Bank Queue, and updates the BankTeller to be false
      if(bankQueue.empty() && tellerAvailable == true)
      {
          unsigned departureTime = currentTime + customer.getDuration();
          Event newDepartureEvent(Event::departure, departureTime); 
          eventListPQueue.emplace(newDepartureEvent);
          tellerAvailable = false;
      }
      //Otherwise will add the customer to the BankQueue if the customer is not available
      else
          bankQueue.emplace(customer);
  }

//Processes Departure Event
void processDeparture(Event departureEvent, priority_queue<Event, vector<Event>, greater<Event>>& eventListPQueue, queue<Event>& bankQueue)
   {
      // Remove this event from the event list
       eventListPQueue.pop();
      cout<<left<< setw(35);
      cout<< "Processing departure event at time:";
      cout<<right<< setw(5)<< departureEvent.getTime() <<endl;
       //If the Bank Queue is not empty, a New event is created for the next customer
       //Wait time is calculated for the next customer, the Customer is removed from the Bank Queue
       //A new Departure Event is created for the Next Customer and added to the Priority Queue
       if(!bankQueue.empty())
      {
      // Customer at front of line begins transaction
         Event nextcustomer = bankQueue.front();
         waitTime += (currentTime - nextcustomer.getTime());
         bankQueue.pop();
         unsigned departureTime = currentTime + nextcustomer.getDuration();
         Event newDepartureEvent(Event::departure, departureTime); 
         eventListPQueue.emplace(newDepartureEvent);    
    }
    //Otherwise, if the BankQueue is empty then, the teller is available
     else
     {
         tellerAvailable = true;
     }
 }
 
 
 
int main()
{
   //Variable to store the arrival time of the customer
   unsigned arrivalTime = 0;
   //Variable to store the durationTime of the customer
   unsigned durationTime = 0;
   //Variable to store the transaction time of the customer
   double transactionTime = 0;
   //Variable to store the number of transactions occuring duirng the simulation
   double count = 0; 
   //Min Priority Queue to store the Events, Arrivals and Departures being processed 
   priority_queue<Event, vector<Event>, greater<Event>> eventListPQueue; 
   //A queue to store the customers in line waiting for the Bank Teller
   queue<Event> bankQueue; 
   //Event to store the departure Events
   Event departureEvent;
   
   // Create and add arrival events to event list
   while(cin >> arrivalTime >> durationTime)
  {
     Event arrivalEvent(Event::arrival, arrivalTime, durationTime);
     eventListPQueue.push(arrivalEvent); 
     
  }
  
  cout << "Simulation Begins"<<endl;
  // Event loop, which processes the Customers one at a time from the Queue 
  //and will process based on the type of the Event in the Priority Queue
  while(!eventListPQueue.empty())
  {
      
      Event newEvent = eventListPQueue.top();
      currentTime = newEvent.getTime();
      transactionTime += newEvent.getDuration();
      
      if(newEvent.getType() == Event::arrival){
         count++;
         processArrival(newEvent, eventListPQueue , bankQueue);
          
      }
      else
      
          processDeparture(newEvent , eventListPQueue , bankQueue);
          
  }
  
  cout<<"Simulation Ends"<<endl;
  cout<<endl;
  //Once the simulation ends, the average statistics are Calculated for the Simulation Statitistics
  cout<<"Simulation Statistics"<<endl;
  cout << "  Number of transactions processed: " << right<< setw(4)<<count<<endl;
  cout<< "          Average transaction time: " << right<< setw(7)<<fixed<<setprecision(2)<< (transactionTime/count)<< endl;
  cout<< "                 Average wait time: "<< right<< setw(7)<< fixed<< setprecision(2)<< (waitTime/count)<< endl;
  cout<<endl;

   return 0;
}