#include <iostream>
#include <typeinfo>
using namespace std;

class TDistance {
  public:
	// Use the type system to compile-time select the constructor units
	class inKilometers {};
	class inMeters {};
	class inCentimeters {};
	class inMillimeters {};
	class inMicrometers {};
	class inNanometers {};
	explicit constexpr TDistance(inNanometers, long double nm ) : TDistance(nm) {}
	explicit constexpr TDistance(inKilometers, long double km ) : TDistance(km * 1000ULL * 1000000000ULL) {}
	explicit constexpr TDistance(inMeters, long double m ) : TDistance(m * 1000000000ULL) {}
	explicit constexpr TDistance(inMillimeters, long double mm ) : TDistance(mm * 1000000000ULL / 1000ULL) {}
	explicit constexpr TDistance(inCentimeters, long double cm ) : TDistance(cm * 1000000000ULL / 100ULL) {}
	explicit constexpr TDistance(inMicrometers, long double um ) : TDistance(um * 1000000000ULL / 1000000ULL) {}

	long double km() const { return mNanometers / 1000000000000.0; }
	long double m() const { return mNanometers / 1000000000.0; }
	long double cm() const { return mNanometers / 10000000.0; }
	long double mm() const { return mNanometers / 1000000.0; }
	long double um() const { return mNanometers / 1000.0; }
	unsigned long long nm() const { return mNanometers; }

  protected:
	// Hide the unsafe constructor from external users
	constexpr TDistance(unsigned long long nm) : mNanometers{nm} {}
	// SI unit is meters, but we'd have to use a double as the
	// underlying storage if we used SI.  Instead we'll pick one
	// nanometer as being the smallest distance we can represent.
	// That would make a 32 bit storage value able to hold at most
	// 4,294,967,296 nanometers, or 4.2 meters.  Not enough.  A double
	// would use up 64-bits anyway, so let's use up 64-bits of integer.
	// That gives us 18,446,744,073,709,551,616 nanometers or 18.4
	// gigameters.  We'll call that sufficient for any practical
	// application (if you're calculating interstellar differences, then
	// probably you should be using light-speed based units anyway)
	unsigned long long mNanometers;
};

// Distance literals
constexpr TDistance operator "" _km( long double km ) {
	return TDistance(TDistance::inKilometers{}, static_cast<long double>(km));
}
constexpr TDistance operator "" _m( long double m ) {
	return TDistance(TDistance::inMeters{}, static_cast<long double>(m));
}
constexpr TDistance operator "" _cm( long double cm ) {
	return TDistance(TDistance::inCentimeters{}, static_cast<long double>(cm));
}
constexpr TDistance operator "" _mm( long double mm ) {
	return TDistance(TDistance::inMillimeters{}, static_cast<long double>(mm));
}
constexpr TDistance operator "" _um( long double um ) {
	return TDistance(TDistance::inMicrometers{}, static_cast<long double>(um));
}
constexpr TDistance operator "" _nm( long double nm ) {
	return TDistance(TDistance::inNanometers{}, static_cast<long double>(nm));
}


// ----------------------------------

// http://physics.ucr.edu/~wudka/Physics7/Notes_www/node17.html
const auto HeightOfMan = 1.8_m;
const auto NeutronStarRadius = 10.0_km;

int main()
{
	cerr
		<< "Height of a man " << HeightOfMan.m()
			<< "m" << endl
		<< "Neutron star radius "
		<< NeutronStarRadius.km()
			<< "km" << endl;

	return 0;
}
