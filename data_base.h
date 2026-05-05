#pragma once

#include<nlohmann/json.hpp>
#include<iostream>
#include<fstream>
#include "structs.h"

using json = nlohmann::json;

json seat_to_JSON(const Seat& seat);
Seat JSON_to_seat(const json& seat);
json airplane_to_JSON(const Airplane& plane);
Airplane JSON_to_airplane(const json& plane);
json flight_to_JSON(const Flight& flight);
Flight JSON_to_flight(const json& flight);
json ticket_to_JSON(const Ticket& ticket);
Ticket JSON_to_ticket(const json& ticket);
json user_to_JSON(const User& user);
User JSON_to_user(const json& user);
json city_to_JSON(const City& city);
City JSON_to_city(const json& city);
json airport_to_JSON(const Airport& airport);
Airport JSON_to_airport(const json& airport);
json admin_to_JSON(const Admin& admin);
Admin JSON_to_admin(const json& admin);

void SAVE_DATA();
void LOAD_DATA();