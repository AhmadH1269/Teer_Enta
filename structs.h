#pragma once
#include "homemade-STLs.h"
#include<string>
#include<chrono>
using namespace std;
using namespace chrono;


struct Seat {
	int row;
	int column;
	bool booked;
	int tier; // 1 first class, 2 business class, 3 economy class
};

struct Airplane {
	int plane_ID;
	string current_city;
	int number_of_seats;
	string manufacturer;
	string model;
	int number_of_flights;
	double speed;			// Average speed in Km/h
	double fuel_price;		// Price per kilometer
};
extern Vector<Airplane> planes;
extern int number_of_planes;
extern Vector<Airplane> models;


struct Flight {
	//------------------STILL TO BE REVIEWED---------------------
	int flight_ID;
	int from_airport_ID;
	int to_airport_ID;
	string from;
	string to;

	// Price range
	int high_price;
	int mid_price;
	int low_price;

	int available_seats;
	int plane_ID;
	minutes estimated_time;

	sys_time<minutes> departure_date;
	sys_time<minutes> arrival_date;

	bool operator==(const Flight& other) const {
		return flight_ID == other.flight_ID;
	}
};
extern Vector<Flight> flights;
extern int number_of_flights;

struct Ticket {
	int adults;
	int children;
	int flight_ID;
	Vector<Seat> seats;	// Maximum of 14 seats per ticket
};

struct User {
	int user_ID;
	string email;
	long long password;
	string username;
	string phone_number;
	Vector<int> future_flights_IDs;
	Vector<int> flights_history_IDs;
	int seat_preference = 1; // Default
	int tier_preference = 2; // Default
	Vector<Ticket> tickets;
};
extern Vector<User> users;
extern int number_of_users;

struct City {
	string location;
	string code;
	double longitude;
	double latitude;
};
extern Vector<City> cities;

struct Airport {
	int airport_ID;
	City city;
	string email;
	long long password;
	int capacity;
	Vector<int> scheduled_planes_IDs;				// Airplane ID's
	Vector<int> free_planes_IDs;					// Airplane ID's
	Vector<int> future_flights_IDs;					// Flight ID's
	Vector<int> pending_planes_IDs;
	Vector<Flight> pending_requests;
	Vector<Flight> sent_pending_requests;
};
extern Vector<Airport> airports;
extern int number_of_airports;

struct Admin {
	int admin_ID;
	string username;
	string email;
	long long password;
};
extern Vector<Admin> admins;
extern int number_of_admins;
