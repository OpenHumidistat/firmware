#ifndef HUMIDISTAT_CONFIGPAR_H
#define HUMIDISTAT_CONFIGPAR_H

#define XSTR(s) STR(s)
#define STR(s) #s
#define WIDTH 7
#define NUM_DIGITS WIDTH - 2
#define NUM_DECIMALS 4

/// A class for storing references to variables of various types (uint8_t, uint16_t, or double).
class ConfigPar {
public:
	enum class ConfigParType {
		ui8,
		ui16,
		d,
	};
	// The nested struct is in order to have an overloaded constructor for the type and the union, but avoid
	// having to strcopy the label
	struct Var {
		const ConfigParType type;
		union {
			uint8_t *const ui8;
			uint16_t *const ui16;
			double *const d;
		};

		///@{
		/// Constructor.
		/// \param par Pointer to the variable
		Var(uint8_t *par) : type(ConfigParType::ui16), ui8(par) {}
		Var(uint16_t *par) : type(ConfigParType::ui16), ui16(par) {}
		Var(double *par) : type(ConfigParType::d), d(par) {}
		///@}

		/// Default constructor, initializes with nullptr.
		Var() : type(ConfigParType::ui8), ui8(nullptr) {}
	} var;

	char label[10];

	/// Add delta to the variable.
	/// \param delta Amount to add
	void adjust(int16_t delta) const;

	/// Print "label: value" to string. Automatically allocates string on the heap. Make sure to delete it immediately
	/// afterwards.
	/// \return Pointer to char string
	char *asprint() const;

	/// Get magnitude (number of digits before the decimal separator) of variable
	/// \return magnitude
	uint8_t magnitude() const;
};


#endif //HUMIDISTAT_CONFIGPAR_H
