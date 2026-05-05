#pragma once

#define _USE_MATH_DEFINES       // To add PI constant from cmath library
#include<cmath>
#include "enums.h"
#include "structs.h"
#include "password-handling.h"

Pair<SIGN_UP, int> signup(string email, string username, string password1, string password2, string phone_number);
Pair<LOGIN, int> login(string username, string password);
CREATE_ADMIN create_admin(string username, string email, string password1, string password2);
CREATE_AIRPORT create_airport(City city, string email, string password1, string password2, int capacity);
bool buy_new_plane(int airport_ID, int model_ID);
CREATE_TICKET create_ticket(int user_ID, int flight_ID, int adults, int children, Vector<Seat> seats);
double base_ticket_price(int plane_ID, double distance);
inline sys_time<minutes> arrival_date(sys_time<minutes> departure_date, minutes estimated_time) {
    return departure_date + estimated_time;
}
bool isAcceptable(int to_airport_ID);

inline double toRad(double degree) {
    return degree * M_PI / 180.0; // Converts degrees to radians
}
double haversine(double lat1, double lon1, double lat2, double lon2);
inline minutes Time(double distance, double speed) {
    double hours = distance / speed;
    return minutes(long long(hours * 60));
}
Vector<int> search_flights(string from, string to);

bool send_request(int plane_ID, int from_airport_ID, int to_airport_ID, sys_time<minutes> departure_date);
bool accept_request(int to_airport_ID, int from_airport_ID, int request_ID);
bool deny_request(int to_airport_ID, int from_airport_ID, int request_ID);
bool add_plane_to_system(string manufacturer, string model, int number_of_seats, double speed, double fuel_price);