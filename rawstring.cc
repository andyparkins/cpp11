#include <iostream>
#include <chrono>

auto const QUINE = R"***(
// QUINE will be above this line

auto const BEFORE_QUINE =
	"#include <iostream>\n#include<chrono>\n"
	"auto const QUINE = R\"***(";
auto const AFTER_QUINE = ")***\";";

int main()
{
	std::cout << BEFORE_QUINE << QUINE << AFTER_QUINE << QUINE;
	std::cout << "// Quine timestamp : "
		<< std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now()) << std::endl;
}
)***";
// QUINE will be above this line

auto const BEFORE_QUINE =
	"#include <iostream>\n\n"
	"auto const QUINE = R\"***(";
auto const AFTER_QUINE = ")***\";";

int main()
{
	std::cout << BEFORE_QUINE << QUINE << AFTER_QUINE << QUINE;
	std::cout << "// Quine timestamp : "
		<< std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now()) << std::endl;
}
