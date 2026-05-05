#include "functions.h"


Pair<SIGN_UP, int> signup(string email, string username, string password1, string password2, string phone_number) {

	if (password1 != password2)						return { USER_PASSWORD_MISMATCH, -1 };	            // Passwords don't match
	if (passwordStrengthIndicator(password1) < 5)	return { USER_WEAK_PASSWORD, -1 };		            // Password too weak
	for (int i = 0; i < users.size; i++) {
		if (users[i].email == email)				return { USER_EMAIL_USED, -1 };			            // Email already used
		if (users[i].username == username)			return { USER_USERNAME_TAKEN, -1 };		            // Username already exists
	}
	for (int i = 0; i < admins.size; i++) {
		if (admins[i].email == email)				return { USER_EMAIL_USED, -1 };			            // Email already used
		if (admins[i].username == username)			return { USER_USERNAME_TAKEN, -1 };		            // Username already used
	}
	for (int i = 0; i < airports.size; i++) {
		if (airports[i].email == email)				return { USER_EMAIL_USED, -1 };			            // Email already exists
	}
	User user = { users.size, email, passwordHash(password1 + email), username, phone_number, {}, {} }; // Create user
	users.push_back(user);												                                // Store user

	return { USER_SUCCESS, users.size - 1 };            // Signed up successfully and proceed to app
}

Pair<LOGIN, int> login(string username, string password) {

	for (int i = 0; i < users.size; i++)
		if (username == users[i].username || username == users[i].email) {
			if (passwordHash(password + users[i].email) == users[i].password)
				return { USER, i };             // Logged in successfully as a user
			else    return { WRONG_PASSWORD, -1 };  // Wrong password
		}

	for (int i = 0; i < airports.size; i++)
		if (username == airports[i].email) {
			if (passwordHash(password + airports[i].email) == airports[i].password)
				return { AIRPORT, i };          // Logged in successfully as an airport
			else    return { WRONG_PASSWORD, -1 };  // Wrong password
		}

	for (int i = 0; i < admins.size; i++)
		if (username == admins[i].username || username == admins[i].email) {
			if (passwordHash(password + admins[i].email) == admins[i].password)
				return { ADMIN, i };            // Logged in successfully as an admin
			else    return { WRONG_PASSWORD, -1 };  // Wrong password
		}

	return { USER_NOT_FOUND, -1 };  // Failed to find the user in the system
}

CREATE_AIRPORT create_airport(City city, string email, string password1, string password2, int capacity) {
	if (password1 != password2)								return AIRPORT_PASSWORD_MISMATCH;                   // Passwords don't match
	if (passwordStrengthIndicator(password1) < 5)			return AIRPORT_WEAK_PASSWORD;                       // Password too weak
	for (int i = 0; i < airports.size; i++) {
		if (email == airports[i].email)						return AIRPORT_EMAIL_USED;				            // Email already exists
	}
	for (int i = 0; i < admins.size; i++) {
		if (admins[i].email == email)						return AIRPORT_EMAIL_USED;				            // Email already exists 
	}
	for (int i = 0; i < users.size; i++) {
		if (users[i].email == email)						return AIRPORT_EMAIL_USED;				            // Email already exists 
	}
	// Remove the city to avoid making a new airport again in the same city
	for (int i = 0; i < cities.size; i++)
		if (city.location == cities[i].location) {
			cities.erase(i);
			break;
		}
	Airport airport = { airports.size, city, email, passwordHash(password1 + email), capacity, {}, {}, {}, {} }; // Creat the airport
	airports.push_back(airport);                                                                                // Store the airport
	return AIRPORT_SUCCESS;
}
CREATE_ADMIN create_admin(string username, string email, string password1, string password2) {

	if (password1 != password2)						return ADMIN_PASSWORD_MISMATCH;		// Passwords don't match
	if (passwordStrengthIndicator(password1) < 5)	return ADMIN_WEAK_PASSWORD;			// Password too weak

	for (int i = 0; i < admins.size; i++) {
		if (username == admins[i].username)
			return ADMIN_USERNAME_TAKEN;		// Username already exists 
		if (email == admins[i].email)
			return ADMIN_EMAIL_USED;			// Email already exists 
	}
	for (int i = 0; i < users.size; i++) {
		if (email == users[i].email)
			return ADMIN_USERNAME_TAKEN;		// Email already exists 
		if (username == users[i].username)
			return ADMIN_EMAIL_USED;			// Username already exists 
	}
	for (int i = 0; i < airports.size; i++) {
		if (email == airports[i].email)
			return ADMIN_EMAIL_USED;			// Email already exists
	}

	Admin admin = { number_of_admins++, username, email, passwordHash(password1 + email) };		// Create admin
	admins.push_back(admin);															// Store admin

	return ADMIN_SUCCESS;
}


double base_ticket_price(int plane_ID, double distance) {

	double total_cost = distance * planes[plane_ID].fuel_price;                             // Total cost without profits
	double base_seat_cost = total_cost / planes[plane_ID].number_of_seats;                  // Cost per seat

	const double profit_constant = 1.3;

	// Final base price with profits
	return base_seat_cost * profit_constant;
}

double haversine(double lat1_deg, double lon1_deg, double lat2_deg, double lon2_deg) {
	double delta_lat = toRad(lat2_deg - lat1_deg);
	double delta_lon = toRad(lon2_deg - lon1_deg);

	double lat1_rad = toRad(lat1_deg);
	double lat2_rad = toRad(lat2_deg);

	double haversine_term = (sin(delta_lat / 2) * sin(delta_lat / 2)) +
		(cos(lat1_deg) * cos(lat2_deg)) *
		(sin(delta_lon / 2) * sin(delta_lon / 2));

	double angular_distence = 2 * atan2(sqrt(haversine_term), sqrt(1 - haversine_term)); //equation of distance

	return R * angular_distence; // Distance in km
}

bool isAcceptable(int to_airport_ID) {
	Airport& airport = airports[to_airport_ID];

	int occupied = airport.free_planes_IDs.size + airport.scheduled_planes_IDs.size + airport.pending_planes_IDs.size;
	// Current planes after adding the plane
	int current_planes = occupied + 1;

	for (int i = 0; i < airport.future_flights_IDs.size; i++) {
		Flight& f = flights[airport.future_flights_IDs[i]];

		if (f.from_airport_ID == to_airport_ID) current_planes--;	// A plane leaves, freeing a spot

		else									current_planes++;	// A plane arrives, taking a spot

		// Reject if at any point in the future the planes exceed the capacity
		if (current_planes > airport.capacity) return false;
	}

	// Checks if there is room after all scheduled events?
	return current_planes <= airport.capacity;
}

bool buy_new_plane(int airport_ID, int model_ID) {
	Airport* airport = &airports[airport_ID];
	if (!isAcceptable(airport_ID)) return false;
	airport->free_planes_IDs.push_back(planes.size);
	planes.push_back(models[model_ID]);
	planes[planes.size - 1].plane_ID = planes.size - 1;

	return true;
}

CREATE_TICKET create_ticket(int user_ID, int flight_ID, int adults, int children, Vector<Seat> seats) {
	if (users[user_ID].tickets.size == 10) return CANNOT_ADD_TICKETS;
	if (!adults || adults + children > 7) return BROKE_RULES;
	for (int i = 0; i < seats.size; i++)
		seats[i].booked = true;

	users[user_ID].tickets.push_back({ adults, children, flight_ID, seats });
	return TICKET_SUCCESS;
}

bool send_request(int plane_ID, int from_airport_ID, int to_airport_ID, sys_time<minutes> departure_date) {

	Airport* from_airport = &airports[from_airport_ID];
	Airport* to_airport = &airports[to_airport_ID];

	// Calculate distance to determine prices and estimated time
	double distance = haversine(from_airport->city.longitude, from_airport->city.latitude, to_airport->city.longitude, to_airport->city.latitude);
	double base_price = base_ticket_price(plane_ID, distance);
	minutes estimated_time = Time(distance, planes[plane_ID].speed);
	sys_time<minutes> arrival = arrival_date(departure_date, estimated_time);

	// Remove the plane from free planes and add it to pending planes
	from_airport->free_planes_IDs.erase(from_airport->free_planes_IDs.find(plane_ID));
	from_airport->pending_planes_IDs.push_back(plane_ID);

	Flight flight = {
		to_airport->pending_requests.size,
		from_airport_ID,
		to_airport_ID,
		from_airport->city.code,
		to_airport->city.code,
		base_price * first_mult, base_price * business_mult, base_price * economy_mult,    // Prices
		planes[plane_ID].number_of_seats,
		plane_ID,
		estimated_time,
		departure_date,
		arrival
	};

	airports[to_airport_ID].pending_requests.push_back(flight);
	airports[from_airport_ID].sent_pending_requests.push_back(flight);
	return true;
}

bool accept_request(int to_airport_ID, int from_airport_ID, int request_ID) {
	Flight flight = airports[to_airport_ID].pending_requests[request_ID];
	Airport& to_airport = airports[to_airport_ID];
	Airport& from_airport = airports[from_airport_ID];

	from_airport.pending_planes_IDs.erase(from_airport.pending_planes_IDs.find(flight.plane_ID));
	from_airport.scheduled_planes_IDs.push_back(flight.plane_ID);

	to_airport.pending_requests.erase(request_ID);
	from_airport.sent_pending_requests.erase(from_airport.sent_pending_requests.find(flight));

	flight.flight_ID = flights.size;
	flights.push_back(flight);

	to_airport.future_flights_IDs.push_back(flight.flight_ID);
	from_airport.future_flights_IDs.push_back(flight.flight_ID);


	return true;
}

bool deny_request(int to_airport_ID, int from_airport_ID, int request_ID) {
	Flight flight = airports[to_airport_ID].pending_requests[request_ID];
	Airport& to_airport = airports[to_airport_ID];
	Airport& from_airport = airports[from_airport_ID];
	to_airport.pending_requests.erase(request_ID);
	from_airport.sent_pending_requests.erase(from_airport.sent_pending_requests.find(flight));
	from_airport.pending_planes_IDs.erase(from_airport.pending_planes_IDs.find(flight.plane_ID));

	return true;
}

Vector<int> search_flights(string from, string to) {
	Vector<int> searched_flights;
	for (int i = 0; i < flights.size; i++) {
		if (flights[i].from == from && flights[i].to == to) {
			searched_flights.push_back(i);
		}
	}
	return searched_flights;
}

bool add_plane_to_system(string manufacturer, string model, int number_of_seats, double speed, double fuel_price) {
	Airplane new_model = { models.size, "", number_of_seats, manufacturer, model, 0, speed, fuel_price };
	models.push_back(new_model);
	return true;
}