#ifndef TFHE_GENERIC_STREAMS_H
#define TFHE_GENERIC_STREAMS_H

///@file
///@brief This file declares tools and constantsused for Input/Output serialization

#include <string>

/*
 * Types for the different samples
 * LWE 42: n Torus32 (a), 1 Torus32 (b), 1 double (current_variance)
 * TLWE 84: (k+1)N Torus32 (a), 1 double (current_variance)
 * TGSW 168: (k+1)l TLWE 
 */
const int32_t LWE_SAMPLE_TYPE_UID = 42;
const int32_t TLWE_SAMPLE_TYPE_UID = 84;
const int32_t TLWE_SAMPLE_FFT_TYPE_UID = 83;
const int32_t TGSW_SAMPLE_TYPE_UID = 168;
const int32_t TGSW_SAMPLE_FFT_TYPE_UID = 167;
/*
 * Types for the different keys
 * LWE 42: n Torus32 (a), 1 Torus32 (b), 1 double (current_variance)
 * TLWE 84: (k+1)N Torus32 (a), 1 double (current_variance)
 * TGSW 168: (k+1)l TLWE 
 */
const int32_t LWE_KEY_TYPE_UID = 43;
const int32_t TLWE_KEY_TYPE_UID = 85;
const int32_t TGSW_KEY_TYPE_UID = 169;
const int32_t LWE_KEYSWITCH_KEY_TYPE_UID = 200;
const int32_t LWE_BOOTSTRAPPING_KEY_TYPE_UID = 201;

/**
 * This is a generic Istream wrapper: supports getLine() and feof()
 */
class Istream {
public:
    /**
     * this function reads a line from the stream F, and stores the result in the string reps
     * '\r' characters are consumed and ignored,
     * '\n' stops the reading, and is consumed but not included in the result
     */
    virtual void getLine(std::string &reps) const =0;

    virtual void fread(void *data, size_t bytes) const =0;

    /**
     * tests for the end of the inputstream
     */
    virtual bool feof() const =0;

    virtual ~Istream() {};
};

class CIstream; //subclass specialized for C FILE* streams
class StdIstream; //subclass specialized for C++ istream streams


/**
 * This is a generic Ostream wrapper: supports fputs()
 */
class Ostream {
public:
    /**
     * prints a string in the outputstream. No endline character is added.
     */
    virtual void fputs(const std::string &s) const =0;

    virtual void fwrite(const void *data, size_t bytes) const =0;

    virtual ~Ostream() {};
};


class COstream; //subclass specialized for C FILE* streams
class StdOstream; //subclass specialized for C++ ostream streams


COstream to_Ostream(FILE *F);

StdOstream to_Ostream(std::ostream &out);

CIstream to_Istream(FILE *F);

StdIstream to_Istream(std::istream &in);


/**
 * This is a property map: a title and a set of name-value properties.
 * For convenience, this class accepts string, int64_ts (longs) or doubles for 
 * the property values, with implicit conversion between them.
 */
class TextModeProperties {
public:
    virtual const std::string &getTypeTitle() const =0;

    virtual void setTypeTitle(const std::string &title)=0;

    virtual const std::string &getProperty(const std::string &name) const =0;

    virtual double getProperty_double(const std::string &name) const =0;

    virtual int64_t getProperty_int64_t(const std::string &name) const =0;

    virtual void setProperty(const std::string &name, const std::string &value)=0;

    virtual void setProperty_double(const std::string &name, double value)=0;

    virtual void setProperty_int64_t(const std::string &name, int64_t value)=0;

    virtual ~TextModeProperties() {}
};

/**
 * This constructor function reads a property map from a generic stream.
 */
TextModeProperties *new_TextModeProperties_fromIstream(const Istream &in);

/**
 * This constructor function creates a blank property map.
 */
TextModeProperties *new_TextModeProperties_blank();

/**
 * This destructor must be called to release a TextModeProperties
 * instance.
 */
void delete_TextModeProperties(TextModeProperties *ptr);

/**
 * Prints a property map to a generic Ostream.
 */
void print_TextModeProperties_toOStream(const Ostream &out, const TextModeProperties *props);


//-----------------------------------------------------------------
// And now, the specialization details of the stream subclasses
//-----------------------------------------------------------------

/**
 * Specialization of the Istream wrapper around std::istream
 */
class StdIstream : public Istream {
    std::istream &in;
public:
    StdIstream(std::istream &in) : in(in) {}

    virtual void getLine(std::string &reps) const;

    virtual void fread(void *data, size_t bytes) const;

    virtual bool feof() const;

    virtual ~StdIstream() {};
};

/**
 * Specialization of the Istream wrapper around FILE*
 */
class CIstream : public Istream {
    FILE *F;
public:
    CIstream(FILE *F) : F(F) {}

    virtual void getLine(std::string &reps) const;

    virtual void fread(void *data, size_t bytes) const;

    virtual bool feof() const;

    virtual ~CIstream() {};
};


/**
 * Specialization of the Ostream wrapper around std::istream
 */
class StdOstream : public Ostream {
    std::ostream &out;
public:
    StdOstream(std::ostream &out) : out(out) {}

    virtual void fputs(const std::string &s) const;

    virtual void fwrite(const void *data, size_t bytes) const;

    virtual ~StdOstream() {};
};

/**
 * Specialization of the Ostream wrapper around FILE*
 */
class COstream : public Ostream {
    FILE *F;
public:
    COstream(FILE *F) : F(F) {}

    virtual void fputs(const std::string &s) const;

    virtual void fwrite(const void *data, size_t bytes) const;

    virtual ~COstream() {};
};


#endif // TFHE_GENERIC_STREAMS_H

