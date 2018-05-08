#include "Eye.h"

Eye::Eye()
{
}
Eye::~Eye()
{
}

/*******************************************************************************
setup() - 	orients the camera according to values read from the driver file
arguments -	none
returns -	true
*******************************************************************************/
bool Eye::setup()
{
	W = position-look;			//in direction away from the scene...
	W.normalize();
	U = -W.cross(up);
	U.normalize();
	V = -W.cross(U);
	V.normalize();

	if (W.dot(U) != 0)
		cout << "W.U DOES NOT 'EQUAL ZERO'!: " << W.dot(U) << endl;
	if (W.dot(V) != 0)
		cout << "W.V DOES NOT 'EQUAL ZERO'!: " << W.dot(V) << endl;
	if (U.dot(V) != 0)
		cout << "U.V DOES NOT 'EQUAL ZERO'!: " << U.dot(V) << endl;

	return true;
}
