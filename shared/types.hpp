#pragma once

union CommandID {
	struct {
		unsigned short name;
		unsigned short space;
	};

	unsigned int id;
};

typedef unsigned short MachineID;
typedef unsigned short ClientID;

struct TargetID {
	TargetID() : id(0) { }

	TargetID(unsigned int id_) : id(id_) { }

	operator unsigned int () const { return id; }

	union  {
		struct {
			MachineID machine_id;
			ClientID client_id;
		};

		unsigned int id;
	};
};
