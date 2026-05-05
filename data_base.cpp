#include "data_base.h"

json seat_to_JSON(const Seat& seat) {
	return {
		{"row", seat.row},
		{"column", seat.column},
		{"booked", seat.booked},
		{"tier", seat.tier}
	};
}
Seat JSON_to_seat(const json& seat) {
	return {
		seat["row"],
		seat["column"],
		seat["booked"],
		seat["tier"]
	};
}

json airplane_to_JSON(const Airplane& plane) {
	return {
		{"plane_ID", plane.plane_ID},
		{"current_city", plane.current_city},
		{"number_of_seats", plane.number_of_seats},
		{"manufacturer", plane.manufacturer},
		{"model", plane.model},
		{"number_of_flights", plane.number_of_flights },
		{"speed", plane.speed},
		{"fuel_price", plane.fuel_price }
	};
}
Airplane JSON_to_airplane(const json& plane) {
	return {
		plane["plane_ID"],
		plane["current_city"],
		plane["number_of_seats"],
		plane["manufacturer"],
		plane["model"],
		plane["number_of_flights"],
		plane["speed"],
		plane["fuel_price"]
	};
}

json flight_to_JSON(const Flight& flight) {
	return {
		{"flight_ID", flight.flight_ID},
		{"from_airport_ID", flight.from_airport_ID},
		{"to_airport_ID", flight.to_airport_ID},

		{"from", flight.from},
		{"to", flight.to},

		{"high_price", flight.high_price},
		{"mid_price", flight.mid_price},
		{"low_price", flight.low_price},

		{"available_seats", flight.available_seats},
		{"plane_ID", flight.plane_ID},
		{"estimated_time", flight.estimated_time.count()},
		{"departure_date", flight.departure_date.time_since_epoch().count()},
		{"arrival_date", flight.arrival_date.time_since_epoch().count()}
	};
}
Flight JSON_to_flight(const json& flight) {
	return {
		flight["flight_ID"],
		flight["from_airport_ID"],
		flight["to_airport_ID"],
		flight["from"],
		flight["to"],
		flight["high_price"],
		flight["mid_price"],
		flight["low_price"],
		flight["available_seats"],
		flight["plane_ID"],
		minutes(flight["estimated_time"]),
		sys_time<minutes>(minutes(flight["departure_date"])),
		sys_time<minutes>(minutes(flight["arrival_date"]))
	};
}

json ticket_to_JSON(const Ticket& ticket) {
	json seats = json::array();
	for (int i = 0; i < ticket.seats.size; i++)
		seats.push_back(seat_to_JSON(ticket.seats[i]));

	return {
		{"adults", ticket.adults},
		{"children", ticket.children},
		{"flight_ID", ticket.flight_ID},
		{"seats", seats}
	};
}
Ticket JSON_to_ticket(const json& ticket) {
	Vector<Seat> seats;
	for (int i = 0; i < ticket["seats"].size(); i++)
		seats.push_back(JSON_to_seat(ticket["seats"][i]));

	return {
		ticket["adults"],
		ticket["children"],
		ticket["flight_ID"],
		seats
	};
}

json user_to_JSON(const User& user) {
	json future_flights = json::array();
	for (int i = 0; i < user.future_flights_IDs.size; i++)
		future_flights.push_back(user.future_flights_IDs[i]);

	json flights_history = json::array();
	for (int i = 0; i < user.flights_history_IDs.size; i++)
		flights_history.push_back(user.flights_history_IDs[i]);

	json tickets = json::array();
	for (int i = 0; i < user.tickets.size; i++)
		tickets.push_back(ticket_to_JSON(user.tickets[i]));

	return {
		{"user_ID", user.user_ID},
		{"email", user.email},
		{"password", user.password},
		{"username", user.username},
		{"phone_number", user.phone_number},
		{"future_flights", future_flights},
		{"flights_history", flights_history},
		{"seat_preference", user.seat_preference},
		{"tier_preference", user.tier_preference},
		{"tickets", tickets}
	};
}
User JSON_to_user(const json& user) {
	Vector<int> future_flights;
	for (int i = 0; i < user["future_flights"].size(); i++)
		future_flights.push_back(user["future_flights"][i]);

	Vector<int> flights_history;
	for (int i = 0; i < user["flights_history"].size(); i++)
		flights_history.push_back(user["flights_history"][i]);

	Vector<Ticket> tickets;
	for (int i = 0; i < user["tickets"].size(); i++)
		tickets.push_back(JSON_to_ticket(user["tickets"][i]));

	return {
		user["user_ID"],
		user["email"],
		user["password"],
		user["username"],
		user["phone_number"],
		future_flights,
		flights_history,
		user["seat_preference"],
		user["tier_preference"],
		tickets
	};
}

json city_to_JSON(const City& city) {
	return {
		{"city", city.location},
		{"code", city.code},
		{"longitude", city.longitude},
		{"latitude", city.latitude}
	};
}
City JSON_to_city(const json& city) {
	return {
		city["city"],
		city["code"],
		city["longitude"],
		city["latitude"]
	};
}

json airport_to_JSON(const Airport& airport) {
	json scheduled_planes = json::array();
	for (int i = 0; i < airport.scheduled_planes_IDs.size; i++)
		scheduled_planes.push_back(airport.scheduled_planes_IDs[i]);

	json free_planes = json::array();
	for (int i = 0; i < airport.free_planes_IDs.size; i++)
		free_planes.push_back(airport.free_planes_IDs[i]);

	json future_flights = json::array();
	for (int i = 0; i < airport.future_flights_IDs.size; i++)
		future_flights.push_back(airport.future_flights_IDs[i]);

	json pending_planes = json::array();
	for (int i = 0; i < airport.pending_planes_IDs.size; i++)
		pending_planes.push_back(airport.pending_planes_IDs[i]);

	json pending_requests = json::array();
	for (int i = 0; i < airport.pending_requests.size; i++)
		pending_requests.push_back(flight_to_JSON(airport.pending_requests[i]));

	json sent_requests = json::array();
	for (int i = 0; i < airport.sent_pending_requests.size; i++)
		sent_requests.push_back(flight_to_JSON(airport.sent_pending_requests[i]));

	return {
		{"airport_ID", airport.airport_ID},
		{"city", city_to_JSON(airport.city)},
		{"email", airport.email},
		{"password", airport.password},
		{"capacity", airport.capacity},
		{"scheduled_planes_IDs", scheduled_planes},
		{"free_planes_IDs", free_planes},
		{"future_flights_IDs", future_flights},
		{"pending_planes", pending_planes},
		{"pending_requests", pending_requests},
		{"sent_requests", sent_requests}
	};
}
Airport JSON_to_airport(const json& airport) {
	Vector<int> scheduled_planes;
	for (int i = 0; i < airport["scheduled_planes_IDs"].size(); i++)
		scheduled_planes.push_back(airport["scheduled_planes_IDs"][i]);

	Vector<int> free_planes;
	for (int i = 0; i < airport["free_planes_IDs"].size(); i++)
		free_planes.push_back(airport["free_planes_IDs"][i]);

	Vector<int> future_flights;
	for (int i = 0; i < airport["future_flights_IDs"].size(); i++)
		future_flights.push_back(airport["future_flights_IDs"][i]);

	Vector<int> pending_planes;
	for (int i = 0; i < airport["pending_planes"].size(); i++)
		pending_planes.push_back(airport["pending_planes"][i]);

	Vector<Flight> pending_request;
	for (int i = 0; i < airport["pending_requests"].size(); i++)
		pending_request.push_back(JSON_to_flight(airport["pending_requests"][i]));

	Vector<Flight> sent_request;
	for (int i = 0; i < airport["sent_requests"].size(); i++)
		sent_request.push_back(JSON_to_flight(airport["sent_requests"][i]));

	return {
		airport["airport_ID"],
		JSON_to_city(airport["city"]),
		airport["email"],
		airport["password"],
		airport["capacity"],
		scheduled_planes,
		free_planes,
		future_flights,
		pending_planes,
		pending_request,
		sent_request
	};
}

json admin_to_JSON(const Admin& admin) {
	return {
		{"admin_ID", admin.admin_ID},
		{"username", admin.username},
		{"email", admin.email},
		{"password", admin.password}
	};
}
Admin JSON_to_admin(const json& admin) {
	return {
		admin["admin_ID"],
		admin["username"],
		admin["email"],
		admin["password"]
	};
}

// Save Data
void SAVE_DATA() {
	json save;
	json save_cities;

	// Save Airplanes
	save["airplanes"] = json::array();
	for (int i = 0; i < planes.size; i++)
		save["airplanes"].push_back(airplane_to_JSON(planes[i]));

	// Save Models
	save["models"] = json::array();
	for (int i = 0; i < models.size; i++)
		save["models"].push_back(airplane_to_JSON(models[i]));

	// Save Flights
	save["flights"] = json::array();
	for (int i = 0; i < flights.size; i++)
		save["flights"].push_back(flight_to_JSON(flights[i]));

	// Save Users
	save["users"] = json::array();
	for (int i = 0; i < users.size; i++)
		save["users"].push_back(user_to_JSON(users[i]));

	// Overwrite Cities
	save_cities["cities"] = json::array();
	for (int i = 0; i < cities.size; i++)
		save_cities["cities"].push_back(city_to_JSON(cities[i]));

	// Save Airports
	save["airports"] = json::array();
	for (int i = 0; i < airports.size; i++)
		save["airports"].push_back(airport_to_JSON(airports[i]));

	// Save Admins
	save["admins"] = json::array();
	for (int i = 0; i < admins.size; i++)
		save["admins"].push_back(admin_to_JSON(admins[i]));

	// Dump all data into the file
	fstream out;
	out.open("data.json", ios::out);
	if (out.is_open())
		out << save.dump(4);
	else cerr << "Failed to save data\n";
	out.close();
	ofstream cities_out("cities.json");
	if (cities_out.is_open())
		cities_out << save_cities.dump();
	else cerr << "Failed to save data\n";
}
// Load Data
void LOAD_DATA() {
	json load;
	ifstream in("data.json");
	if (!in.is_open()) { cerr << "Failed to open data file\n"; abort(); }

	in >> load;
	in.close();

	json load_cities;
	ifstream cities_in("cities.json");
	if (!cities_in.is_open()) { cerr << "Failed to open cities file\n"; abort(); }

	cities_in >> load_cities;
	cities_in.close();

	cout << "LOADING AIRPLANES..." << endl;
	for (int i = 0; i < load["airplanes"].size(); i++)
		planes.push_back(JSON_to_airplane(load["airplanes"][i]));

	cout << "LOADING AIRPLANES MODELS..." << endl;
	for (int i = 0; i < load["models"].size(); i++)
		models.push_back(JSON_to_airplane(load["models"][i]));

	cout << "LOADING FLIGHTS..." << endl;
	for (int i = 0; i < load["flights"].size(); i++)
		flights.push_back(JSON_to_flight(load["flights"][i]));

	cout << "LOADING USERS..." << endl;
	for (int i = 0; i < load["users"].size(); i++)
		users.push_back(JSON_to_user(load["users"][i]));

	cout << "LOADING CITIES..." << endl;
	for (int i = 0; i < load_cities["cities"].size(); i++)
		cities.push_back(JSON_to_city(load_cities["cities"][i]));

	cout << "LOADING AIRPORTS..." << endl;
	for (int i = 0; i < load["airports"].size(); i++)
		airports.push_back(JSON_to_airport(load["airports"][i]));

	cout << "LOADING ADMINS..." << endl;
	for (int i = 0; i < load["admins"].size(); i++)
		admins.push_back(JSON_to_admin(load["admins"][i]));
}

//int main() {
//	LOAD_DATA();
//	cout << admins[0].password;
//
//}