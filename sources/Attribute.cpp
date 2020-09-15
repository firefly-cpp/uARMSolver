#include "Attribute.h"

Attribute::Attribute() {
	// TODO Auto-generated constructor stub

}

Attribute::~Attribute() {
	// TODO Auto-generated destructor stub
}

/**
 * Enter the initial values of attributes in transaction database.
 *
 * @param initial integer value of the corresponding element.
 * @return no return code.
 */
void Attribute::enter(int item)
{
	type = ATTR_NUMERICAL;
	i_val = item;
	present = true;
}

/**
 * Enter the initial values of attributes in transaction database.
 *
 * @param initial floating-point value of the corresponding element.
 * @return no return code.
 */
void Attribute::enter(double item)
{
	type = ATTR_REAL_VALUED;
	f_val = item;
	present = true;
}

/**
 * Enter the initial values of attributes in transaction database.
 *
 * @param initial string value of the corresponding element.
 * @return no return code.
 */
void Attribute::enter(string item)
{
	type = ATTR_CATEGORICAL;
	s_val = item;
	present = true;
}
