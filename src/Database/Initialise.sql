DROP TABLE Events;
CREATE TABLE Events
(
	id INTEGER PRIMARY KEY,
	name TEXT NOT NULL
);

DROP TABLE Markets;
CREATE TABLE Markets
(
	id INTEGER PRIMARY KEY,
	name TEXT,
	type TEXT,
	status TEXT,
	eventDate TEXT,
	menuPath TEXT,
	eventHeirachy TEXT,
	betDelay TEXT,
	exchangeId INTEGER,
	countryCode TEXT,
	lastRefresh TEXT,
	numOfRunners INTEGER,
	numOfWinners INTEGER,
	totalMatched FLOAT,
	isBSP INTEGER,
	isTurningInPlay INTEGER
);

