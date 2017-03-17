#include <tfhe_io.h>
#include <map>
#include <string>
#include <tfhe_generic_streams.h>

using namespace std;

/**
 * This function prints the lwe parameters to a generic stream
 */
void export_lweParams(const Ostream& F, const LweParams* lweparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("LWEPARAMS");
    props->setProperty_long("n", lweparams->n);
    props->setProperty_double("alpha_min", lweparams->alpha_min);
    props->setProperty_double("alpha_max", lweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}

/**
 * This constructor function reads and creates a LWEParams from a generic stream. The result
 * must be deleted with delete_lweParams();
 */
LweParams* read_new_lweParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("LWEPARAMS")) abort();
    int n = props->getProperty_long("n");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_LweParams(n,alpha_min,alpha_max);
}


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams) { export_lweParams(to_Ostream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT void export_lweParams_toStream(ostream& F, const LweParams* lweparams) { export_lweParams(to_Ostream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromStream(std::istream& in) { return read_new_lweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromFile(FILE* F)  { return read_new_lweParams(to_Istream(F)); }

