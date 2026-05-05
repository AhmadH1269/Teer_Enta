#pragma once

const double R = 6371.0;                // Earth radius in km
const double first_mult = 2.0;          // First class multiplier
const double business_mult = 1.5;       // Bussiness class multiplier
const double economy_mult = 1.0;        // Economy class multiplier

enum CREATE_TICKET {
    CANNOT_ADD_TICKETS,
    BROKE_RULES,
    TICKET_SUCCESS
};

enum CREATE_ADMIN {
    ADMIN_SUCCESS,
    ADMIN_PASSWORD_MISMATCH,
    ADMIN_WEAK_PASSWORD,
    ADMIN_USERNAME_TAKEN,
    ADMIN_EMAIL_USED
};

enum CREATE_AIRPORT {
    AIRPORT_SUCCESS,
    AIRPORT_PASSWORD_MISMATCH,
    AIRPORT_WEAK_PASSWORD,
    AIRPORT_LOCATION_EXISTS,
    AIRPORT_EMAIL_USED
};

enum LOGIN {
    USER,
    AIRPORT,
    ADMIN,
    WRONG_PASSWORD,
    USER_NOT_FOUND
};

enum SIGN_UP {
    USER_SUCCESS,
    USER_PASSWORD_MISMATCH,
    USER_WEAK_PASSWORD,
    USER_USERNAME_TAKEN,
    USER_EMAIL_USED
};