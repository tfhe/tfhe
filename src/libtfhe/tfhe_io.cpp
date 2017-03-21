#include <tfhe_io.h>
#include <map>
#include <string>
#include <tfhe_generic_streams.h>

using namespace std;




/* ****************************
 * LWE params
**************************** */

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















/* ****************************
 * TLWE params
**************************** */

/**
 * This function prints the tlwe parameters to a generic stream
 */
void export_tLweParams(const Ostream& F, const TLweParams* tlweparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("TLWEPARAMS");
    props->setProperty_long("N", tlweparams->N);
    props->setProperty_long("k", tlweparams->k);
    props->setProperty_double("alpha_min", tlweparams->alpha_min);
    props->setProperty_double("alpha_max", tlweparams->alpha_max);
    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}


/**
 * This constructor function reads and creates a TLweParams from a generic stream. The result
 * must be deleted with delete_TLweParams();
 */
TLweParams* read_new_tLweParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TLWEPARAMS")) abort();
    int N = props->getProperty_long("N");
    int k = props->getProperty_long("k");
    double alpha_min = props->getProperty_double("alpha_min");
    double alpha_max = props->getProperty_double("alpha_max");
    delete_TextModeProperties(props);
    return new_TLweParams(N,k,alpha_min,alpha_max);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tLweParams_toFile(FILE* F, const TLweParams* tlweparams) { export_tLweParams(to_Ostream(F),tlweparams); }

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT void export_tLweParams_toStream(ostream& F, const TLweParams* tlweparams) { export_tLweParams(to_Ostream(F),tlweparams); }

/**
 * This constructor function reads and creates a TLWEParams from a stream. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromStream(std::istream& in) { return read_new_tLweParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TLWEParams from a File. The result
 * must be deleted with delete_TLweParams();
 */
EXPORT TLweParams* new_tLweParams_fromFile(FILE* F)  { return read_new_tLweParams(to_Istream(F)); }













/* ****************************
 * TGSW params
**************************** */

/**
 * This function prints the tGsw parameters to a generic stream
 */
void export_tGswParams(const Ostream& F, const TGswParams* tgswparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("TGSWPARAMS");
    props->setProperty_long("l", tgswparams->l);
    props->setProperty_long("Bgbit", tgswparams->Bgbit);

    // un set property pour const TLweParams* tlweparams






    print_TextModeProperties_toOStream(F, props);
    delete_TextModeProperties(props);
}


/**
 * This constructor function reads and creates a TGswParams from a generic stream. The result
 * must be deleted with delete_TGswParams();
 */
TGswParams* read_new_tGswParams(const Istream& F) {
    TextModeProperties* props = new_TextModeProperties_fromIstream(F);
    if (props->getTypeTitle() != string("TGSWPARAMS")) abort();
    int l = props->getProperty_long("l");
    int Bgbit = props->getProperty_long("Bgbit");
    
    // TLweParams* tlweparams = props->getProperty_...

    delete_TextModeProperties(props);
    return new_TGswParams(l,Bgbit,tlweparams);
}


/**
 * This function prints the tLwe parameters to a file
 */
EXPORT void export_tGswParams_toFile(FILE* F, const TGswParams* tgswparams) { export_tGswParams(to_Ostream(F),tgswparams); }

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT void export_tGswParams_toStream(ostream& F, const TGswParams* tgswparams) { export_tGswParams(to_Ostream(F),tgswparams); }

/**
 * This constructor function reads and creates a TGSWParams from a stream. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromStream(std::istream& in) { return read_new_tGswParams(to_Istream(in)); }

/**
 * This constructor function reads and creates a TGSWParams from a File. The result
 * must be deleted with delete_TGswParams();
 */
EXPORT TGswParams* new_tGswParams_fromFile(FILE* F)  { return read_new_tGswParams(to_Istream(F)); }
