#include <ratio>
#include <iostream>
#include <cstdint>
using namespace std;

// http://en.wikipedia.org/wiki/Fundamental_unit
template<int _Mass = 0,
	int _Distance = 0,
	int _Duration = 0,
	int _Current = 0,
	int _Temperature = 0,
	int _Luminosity = 0,
	int _Count = 0>
struct unitDimension {
	const int mass = _Mass;
	const int distance = _Distance;
	const int duration = _Duration;
	// Not sure why charge isn't the fundamental
	const int current = _Current;
	const int temperature = _Temperature;
	const int luminosity = _Luminosity;
	// moles are, strictly, a count, and while SI have defined it as a
	// unit (but see the criticisms here
	// <http://en.wikipedia.org/wiki/Mole_%28unit%29#The_mole_as_a_unit>),
	// we might find it useful to have a count available rather than a
	// mole.  For example, silicon deposited resistances are defined
	// "per-square" -- i.e. unitless, like moles.  Other examples can
	// be found here <http://en.wikipedia.org/wiki/Dimensionless_quantity>
	const int count = _Count;
};
// All physical units can be defined by the product of these dimensions
// raised to various powers.  The fundamentals are obvious,
using Mass = unitDimension<1,0,0,0,0,0,0>;
using Distance = unitDimension<0,1,0,0,0,0,0>;
using Duration = unitDimension<0,0,1,0,0,0,0>;
// Then we have the compound dimensions:
// distance per duration
using Velocity = unitDimension<0,1,-1,0,0,0,0>;
// distance per duration per duration
using Acceleration = unitDimension<0,1,-2,0,0,0,0>;
// time^4 * current^2 per square-meter per kilogram
using Capacitance = unitDimension<-1,-2,4,2,0,0,0>;


// Stolen the idea from chrono::duration
template<typename _Dimension, typename _Rep, typename _Scale = ratio<1>>
struct physicalUnit {
	typedef _Dimension dimension;
	typedef _Rep rep;
	typedef _Scale scale;

	// Compile time checks
//	static_assert(__is_ratio<_Scale>::value, "scale must be a ratio");
	static_assert(_Scale::num > 0, "scale must have positive numerator");
	static_assert(_Scale::den != 0, "scale must have non-zero denominator");

	// Construction/destruction/copy
	constexpr physicalUnit() = delete;
	constexpr explicit physicalUnit(const rep &_r) :
		r(static_cast<rep>(_r)) {}
	constexpr physicalUnit(const physicalUnit& O) :
		r(O.count()) {}
	~physicalUnit() = default;
	physicalUnit& operator=(const physicalUnit&) = default;

	// Store the value
	constexpr rep count() const { return r; }

  protected:
	rep r;
};

template<typename _Rep, typename _Scale = ratio<1>>
struct physicalDistance : public physicalUnit<Distance, _Rep,_Scale> {
	using typename physicalUnit<Distance, _Rep,_Scale>::dimension;
	using typename physicalUnit<Distance, _Rep,_Scale>::rep;
	using typename physicalUnit<Distance, _Rep,_Scale>::scale;

	constexpr physicalDistance(const rep &_r) :
		physicalUnit<dimension, rep, scale>(_r) {}
};
template<typename _Rep, typename _Scale = ratio<1>>
struct physicalMass : public physicalUnit<Mass, _Rep,_Scale> {};

// Metric
typedef physicalDistance<int64_t, nano> nanometers;
typedef physicalDistance<int64_t, micro> micrometers;
typedef physicalDistance<int64_t, milli> millimeters;
typedef physicalDistance<int64_t> meters;
typedef physicalDistance<int64_t, kilo> kilometers;

typedef physicalMass<int64_t, micro> micrograms;
typedef physicalMass<int64_t, milli> milligrams;
typedef physicalMass<int64_t> grams;
typedef physicalMass<int64_t, kilo> kilograms;

// Imperial
// 25.4mm = 1 inch, or 0.0254m = 1 inch = 254/100
typedef physicalDistance<int64_t, ratio<254,100>> inches;
// 36 inches in 1 yard or 36*0.0254 m = 1 yard
typedef physicalDistance<int64_t, ratio<36*254,100>> yards;
// 1760 yards in 1 mile or 1760*36*0.254 m = 1 mile
typedef physicalDistance<int64_t, ratio<1760*36*254,100>> miles;

// ---- User defined literals

// Distance literals
constexpr kilometers operator "" _km( unsigned long long km ) {
	return kilometers(static_cast<kilometers::rep>(km));
}
//constexpr kilometers operator "" _km( int km ) {
//	return kilometers(static_cast<kilometers::rep>(km));
//}

int main()
{
//	cerr << "yocto = " << yocto::num << " / " << yocto::den << endl;
//	cerr << "zepto = " << zepto::num << " / " << zepto::den << endl;
	cerr << "atto  = " << atto::num << " / " << atto::den << endl;
	cerr << "femto = " << femto::num << " / " << femto::den << endl;
	cerr << "pico  = " << pico::num << " / " << pico::den << endl;
	cerr << "nano  = " << nano::num << " / " << nano::den << endl;
	cerr << "micro = " << micro::num << " / " << micro::den << endl;
	cerr << "milli = " << milli::num << " / " << milli::den << endl;
	cerr << "centi = " << centi::num << " / " << centi::den << endl;
	cerr << "deci  = " << deci::num << " / " << deci::den << endl;
	cerr << "deca  = " << deca::num << " / " << deca::den << endl;
	cerr << "hecto = " << hecto::num << " / " << hecto::den << endl;
	cerr << "kilo  = " << kilo::num << " / " << kilo::den << endl;
	cerr << "mega  = " << mega::num << " / " << mega::den << endl;
	cerr << "giga  = " << giga::num << " / " << giga::den << endl;
	cerr << "tera  = " << tera::num << " / " << tera::den << endl;
	cerr << "peta  = " << peta::num << " / " << peta::den << endl;
	cerr << "exa   = " << exa::num << " / " << exa::den << endl;
//	cerr << "zetta = " << zetta::num << " / " << zetta::den << endl;
//	cerr << "yotta = " << yotta::num << " / " << yotta::den << endl;

	cerr << "kilometers = " << (22_km).count() << " km" << endl;

	return 0;
}
