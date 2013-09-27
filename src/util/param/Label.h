#ifndef UTIL_LABEL_H
#define UTIL_LABEL_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <iostream>
#include <string>
#include <string>

namespace Util
{

   /**
   * A label string in a file format.
   *
   * The operator >> for a label checks if the expected label was found.
   * The operator << outputs the expected label.
   *
   * The constructor takes a parameter isRequired that determines whether
   * the label must be matched (isRequired == true), or if it is optional
   * (isRequired == false). If the input value read by the >> operator does
   * not match the expected value and isRequired is true, the >> operator 
   * will print an error message to the Log::file() and then throw an 
   * Exception. If the input value does not match and isRequired is false,
   * the >> operator stores the input value in a string buffer, and will 
   * compare it to subsequent values until a match is found.
   *
   * \ingroup Param_Module
   */
   class Label
   {

   public:

      /// Width of label field in file output format.
      static const int LabelWidth  = 20;

      /**
      * Constructor.
      *
      * \param label      label string that precedes value in file format
      * \param isRequired Is this label a required entry? (true by default)
      */
      explicit Label(const char* label, bool isRequired = true);

      /**
      * Copy constructor.
      *
      * \param other Label object being copied.
      */
      Label(const Label& other);

      /**
      * Destructor.
      */
      ~Label();

      /**
      * Return label string.
      */
      std::string string() const;

   private:

      /// Is this label a required entry ? (passed to constructor).
      bool isRequired_;

      /// Expected label (passed to constructor).
      std::string label_;

   // Static members:

      /// Did the previous input match the label? (initialized true).
      static bool isMatch_;

      /// Most recent input value from istream (initialized empty).
      static std::string input_;

   //friends:

      friend std::istream& operator>>(std::istream& in, Label label);
      friend std::ostream& operator<<(std::ostream& out, Label label);

   };

   /**
   * Extractor for Label.
   *
   * \param in    input stream
   * \param label Label to be read from file
   */ 
   std::istream& operator>>(std::istream& in, Label label);

   /**
   * Inserter for Label.
   *
   * \param out   output stream
   * \param label Label to be written to file
   */ 
   std::ostream& operator<<(std::ostream& out, Label label);

} 
#endif
