#ifndef TFHE_GENERIC_STREAMS_H
#define TFHE_GENERIC_STREAMS_H

#include <string>

/**
 * This is a generic Istream wrapper: supports getLine() and feof()
 */
class Istream {
    public:
	/**
	 * this function reads a line from the stream F, and stores the result in the string reps
	 * '\r' characters are ignored, 
	 * '\n' stops the reading, and is not included in the result
	 */
	virtual void getLine(std::string& reps) const=0;
	/**
	 * tests for the end of the inputstream
	 */
	virtual bool feof() const=0;
	virtual ~Istream() {};
};

/**
 * This is a generic Ostream wrapper: supports fputs()
 */
class Ostream {
    public:
	/**
	 * prints a string in the outputstream. No endline character is added.
	 */
	virtual void fputs(const std::string& s) const=0;
	virtual ~Ostream() {};
};

/**
 * This is a property map: a title and a set of name-value properties.
 * For convenience, this class accepts string, long or doubles for 
 * the property values, with implicit conversion between them.
 */
class TextModeProperties {
    public:
	virtual const std::string& getTypeTitle() const =0;
	virtual void setTypeTitle(const std::string& title)=0;

	virtual const std::string& getProperty(const std::string& name) const =0;
	virtual double getProperty_double(const std::string& name) const=0;
	virtual long getProperty_long(const std::string& name) const=0;
	virtual void setProperty(const std::string& name, const std::string& value)=0;
	virtual void setProperty_double(const std::string& name, double value)=0;
	virtual void setProperty_long(const std::string& name, long value)=0;
	virtual ~TextModeProperties() {}
};

/**
 * This constructor function reads a property map from a generic stream.
 */
TextModeProperties* new_TextModeProperties_fromIstream(const Istream& in);

/**
 * This constructor function creates a blank property map.
 */
TextModeProperties* new_TextModeProperties_blank();

/**
 * This destructor must be called to release a TextModeProperties
 * instance.
 */
void delete_TextModeProperties(TextModeProperties* ptr);

/**
 * Prints a property map to a generic Ostream.
 */
void print_TextModeProperties_toOStream(const Ostream& out, const TextModeProperties* props);

#endif // TFHE_GENERIC_STREAMS_H

