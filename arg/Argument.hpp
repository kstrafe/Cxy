/*
Copyright 2013, 2014 Kevin Robert Stravers

This file is part of TTL.

TTL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TTL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TTL.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ARGUMENT_HPP_INCLUDED
#define ARGUMENT_HPP_INCLUDED

// Headers
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>


namespace ttl
{

	////////////////////////////////////////////////////////////
	/// \brief Argument Parser and Sorter
	///
	////////////////////////////////////////////////////////////
	class Argument
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		////////////////////////////////////////////////////////////
		Argument();

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking traditional arguments.
		///
		/// Same scheme as the standard int main ( int argc, char *argv[]).
		/// In this case, a ** is necessary as the language does not allow
		/// the declaration of an array. Regardless, arrays degenerate
		/// into pointers.
		///
		/// \param argc The count of arguments.
		/// \param argv The array of C-strings.
		///
		////////////////////////////////////////////////////////////
		Argument(const std::size_t argc, char **argv);

		////////////////////////////////////////////////////////////
		/// \brief Constructor taking a string, separated entries
		/// by whitespace.
		///
		/// \param argument a parsable string.
		///
		////////////////////////////////////////////////////////////
		Argument(const std::string &argument);

		////////////////////////////////////////////////////////////
		/// \brief Destructor.
		///
		////////////////////////////////////////////////////////////
		~Argument();

		////////////////////////////////////////////////////////////
		/// \brief For passing arguments into the parser.
		///
		/// \param argc The count of arguments.
		/// \param argv The array of C-strings.
		///
		////////////////////////////////////////////////////////////
		void pass(const std::size_t argc, char **argv);

		////////////////////////////////////////////////////////////
		/// \brief For passing arguments into the parser.
		///
		/// \param argument a parsable string.
		///
		////////////////////////////////////////////////////////////
		void pass(const std::string &argument);

		////////////////////////////////////////////////////////////
		/// \brief Query whether a multi-character flag is passed.
		///
		/// \param flag The flag to check.
		/// MUST be a multi-char flag (no need to prepend --).
		/// \return whether it is passed or not.
		///
		////////////////////////////////////////////////////////////
		bool isPassed(const std::string &flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Query whether a single-character flag is passed.
		///
		/// \param flag The flag to check.
		/// \return whether it is passed or not.
		///
		////////////////////////////////////////////////////////////
		bool isPassed(const char flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Get the argument assosciated with the flag.
		///
		/// \param flag The flag to get the assosciated argument from.
		/// MUST prepend - or -- depending on the argument.
		/// \return the assosciated argument.
		///
		////////////////////////////////////////////////////////////
		const std::string &getArgument(const std::string &flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Get the argument assosciated with the flag.
		///
		/// \param flag The flag to get the assosciated argument from.
		/// \return the assosciated argument.
		///
		////////////////////////////////////////////////////////////
		const std::string &getArgument(const char flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Get the path (first argument string).
		///
		/// \return The *argv[0], first string entry.
		///
		////////////////////////////////////////////////////////////
		const std::string &getPath() const;

		////////////////////////////////////////////////////////////
		/// \brief Specify which flags are inert.
		///
		/// \param flag The flag to inertify.
		/// MUST prepend - or -- depending on the argument.
		///
		////////////////////////////////////////////////////////////
		void setInert(const std::string &flag);

		////////////////////////////////////////////////////////////
		/// \brief Specify which flags are inert.
		///
		/// \param flag The flag to inertify.
		///
		////////////////////////////////////////////////////////////
		void setInert(const char flag);

		////////////////////////////////////////////////////////////
		/// \brief Specify which flags are inert.
		///
		/// \param flags The flags to inertify.
		///
		////////////////////////////////////////////////////////////
		void setInert(const std::initializer_list<const char> flags);

		////////////////////////////////////////////////////////////
		/// \brief Specify which flags are inert.
		///
		/// \param flags The flags to inertify.
		///
		////////////////////////////////////////////////////////////
		void setInert(const std::initializer_list<const std::string> flags);

		////////////////////////////////////////////////////////////
		/// \brief Query whether a flag is inert.
		///
		/// \param flag The flag to check for inertness.
		/// MUST prepend - or -- depending on the argument.
		/// \return whether the flag is inert or not.
		///
		////////////////////////////////////////////////////////////
		bool isInert(const std::string &flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Query whether a flag is inert.
		///
		/// \param flag The flag to check for inertness.
		/// \return whether the flag is inert or not.
		///
		////////////////////////////////////////////////////////////
		bool isInert(const char flag) const;

		////////////////////////////////////////////////////////////
		/// \brief Fetch the operand according to ID-number.
		///
		/// \param id The index in the operand array.
		/// \return the string containing the operand.
		///
		////////////////////////////////////////////////////////////
		const std::string &getOperand(const std::size_t id) const;

		////////////////////////////////////////////////////////////
		/// \brief Fetch the operand according to ID-number.
		///
		/// \param id The index in the operand array.
		/// \return the string containing the operand.
		///
		////////////////////////////////////////////////////////////
		std::string &getOperand(const std::size_t id);

		////////////////////////////////////////////////////////////
		/// \brief Get the current operand population.
		///
		/// \return The current operand population.
		///
		////////////////////////////////////////////////////////////
		std::size_t getOperandCount() const;

		////////////////////////////////////////////////////////////
		/// \brief Argument dumping output.
		///
		/// \param out the output stream to direct to.
		/// \param argument the Argument object to read from.
		/// \return the same std::ostream as put in.
		///
		////////////////////////////////////////////////////////////
		friend ::std::ostream &operator<<(::std::ostream &out, const ttl::Argument &argument);


	private:

		std::string m_path; ///< Holds the path string (argv[0]).
		std::map<std::string, std::string> m_flags_and_parameters; ///< Flags paired with parameters.
		std::vector<std::string> m_operands; ///< Collection of operands.
		std::set<std::string> m_inert_flags; ///< Collection of inert flags, set for speed of lookup.

		typedef
				std::pair<decltype(m_flags_and_parameters)::iterator, bool>
				InsertReturn;
		typedef
				std::stack<InsertReturn>
				TheUnset;

		const std::string &getArgumentInternal(const std::string &flag) const;

		void setArgumentOfUnsetUninertFlag
		(
				const std::string &argument,
				TheUnset &unset_flags
		);

		void setSingleCharFlags
		(
				const std::string &argument,
				TheUnset &unset_flags
		);

		std::string parseEqualArgument
		(
				const std::string &argument,
				std::size_t x
		);

		void parseEqualArgument
		(
				const std::string &argument,
				TheUnset &unset_flags
		);

		void setMultiCharFlag
		(
				const std::string &argument,
				TheUnset &unset_flags
		);

		void internalSetInert
		(
				const std::string &flag
		);


	public:

		////////////////////////////////////////////////////////////
		/// \brief Get an iterator to the flags paired with their parameters.
		///
		/// \return the iterator to the flags paired with their parameters.
		///
		////////////////////////////////////////////////////////////
		auto getFlagsAndParameters() const -> const decltype(m_flags_and_parameters) &;

	};

} // Namespace ttl

#endif // ARGUMENT_HPP_INCLUDED


////////////////////////////////////////////////////////////
/// \class Argument
///
/// # Arg++ #
///
/// A nice and clean C++ class for handling the work of parsing input arguments.
/// Sorts the arguments correctly and binds them together with flags.
/// This allows the programmer to quickly identify and implement different command-line input.
/// At the same time; all _dirty_ code is abstracted away.
///
/// ## Command Line Conventions ##
/// ------------------------------
///
/// Maybe this'll be useful if you're not very known with command-line commands.
/// http://publib.boulder.ibm.com/tividd/td/tec/SC32-1232-00/en_US/HTML/ecormst15.htm
///
///
/// ## Tiny Tutorial ##
/// -------------------
///
/// We read the argument string from left to right.
///
/// ### Path ###
/// ------------
///
/// The path that the program is being run from is always fetched and stored separately.
/// It is returned by the "getPath()" method, and is always the first string of char *argv[].
///
/// ### Flag ###
/// ------------
///
/// A flag is a program option. Options are defined by:
/// * - one dash; single-character option
/// * -- two dashes; multiple-character option
///
/// ### Argument ###
/// ----------------
///
/// An argument is a piece of data that belongs to a flag.
/// Method 1: "-o a" would make "a" the argument to option "-o". This method uses a whitespace as separator.
/// Method 2: "-o=a" would make "a" the argument to option "-o". This method uses a '=' as separator.
/// An argument will always be bound to the last unset, _non-inert_ flag.
/// (We get back to that later)
///
/// ### Flag Chaining ###
/// ---------------------
///
/// A single-characted flag can be chained:
///   "-a -b -c" becomes "-abc"
///
/// ### Option-Argument Mirroring ###
/// ---------------------------------
///
/// Arguments will be applied to the last unset, _non-inert_ flag such that in:
///   "-q -abc uvw dre xyz nmr"
/// uvw will be bound to "-c"
/// dre will be bound to "-b"
/// xyz will be bound to "-a"
/// and nmr will be bound to "-q"
///
/// ### Inertia ###
/// ---------------
///
/// Flags that are inert will not be filled in by arguments:
///   Let flags "-a", "-b", "-c" exist.
///   Let flag "-b" be inert.
///   "-abc some argument"
/// Will bind "some" to "-c" and "argument" to "-a".
///
/// ### Operands ###
/// ----------------
///
/// Operands are flagless arguments. They are created when:
/// 1. There are no more flags to bind to
/// or
/// 2. We have given anything behind "--" (Flag Terminator)
/// 3. Operands are sorted by order and can be retrieved using getOperand(std::size_t).
/// 4. getOperandCount() returns the amount of operands. getOperand does not check bounds.
///
/// ### Flag Termination ###
/// ------------------------
///
/// All flag-starters ('-' and "--") will be ignored after issueing a "--" followed by nothing:
///   Let flags "-a", "-b", "-c" exist.
///   Let flag "-b" be inert.
///   "-ab data -- -c dr"
/// Will bind "data" to "-a"
/// Will add "-c" and "dr" to a collection of operands.
///
///
/// ## About ##
/// -----------
///
/// C++ argument parsing utility class.
/// Written in pure C++.
/// Conforms with the GNU Utility Syntax guidelines. One exception: "-o foo" is NOT "-ofoo" due to "option-argument mirroring".
/// More info: http://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html
///
////////////////////////////////////////////////////////////
