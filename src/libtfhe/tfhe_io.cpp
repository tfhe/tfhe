#include <tfhe_io.h>
#include <map>
#include <string>

using namespace std;

class MapTextModeProperties: public TextModeProperties {
    map<string, string> data;
    string title;
    public:
        MapTextModeProperties() {}
	virtual const std::string& getTypeTitle() const {return title; }
	virtual const std::string& getProperty(const std::string& name) const { return data.at(name); }
	virtual void setTypeTitle(const std::string& title) { this->title=title; }
	virtual void setProperty(const std::string& name, const std::string& value) { data[name]=value; }
	const map<string, string>& getMap() const { return data; }
	virtual ~MapTextModeProperties() {}
};


void getline(FILE* F, string& reps) {
    reps.clear();
    for (int c = fgetc(F); c!=EOF; c = fgetc(F)) {
	if (c=='\r') continue;
	if (c=='\n') return;
	reps.push_back(c);
    }
}

bool feof(istream& in) { return (bool) in; }

void fputs(ostream& out, const string& line) {
    out << line;
}

void fputs(FILE* F, const string& line) {
    fputs(F,line.c_str());
}


template<typename STREAM>
TextModeProperties* new_TextModeProperties(STREAM& F) {
    MapTextModeProperties* reps = new MapTextModeProperties();
    string line;
    string endDelimitor;
    bool content_started=false;
    for (getline(F, line); !feof(F); getline(F, line)) {
	int n = line.length();
	if (!line.compare(0,11,"-----BEGIN ") && !line.compare(n-5,5,"-----")) {
	    string titleType = line.substr(11, n-16);
	    reps->setTypeTitle(titleType);
	    endDelimitor=string("-----END ")+titleType+string("-----");
	    content_started=true;
	    continue;
	}
	if (!content_started) continue; //ignore anything before body
	if (line == endDelimitor) {
	    return reps;
	}
	size_t pos = line.find(": ");
	if (pos==string::npos) { cerr << "ignoring " << line << endl; continue; }
	string name = line.substr(0,pos);
	string value = line.substr(pos+2);
	cerr << "prop: " << name << "->" << value << endl;
	reps->setProperty(name, value);
    }
    cerr << "error reading text object" << endl;
    return NULL;
}

template<typename STREAM>
void print_TextModeProperties(STREAM& F, const TextModeProperties* properties) {
    const MapTextModeProperties* props = (const MapTextModeProperties*) properties;
    fputs(F, string("-----BEGIN ")+props->getTypeTitle()+string("-----\n"));
    for (auto it: props->getMap()) {
	fputs(F, it.first + string(": ") + it.second+"\n");
    }
    fputs(F, string("-----END ")+props->getTypeTitle()+string("-----\n"));
}

TextModeProperties* new_TextModeProperties_fromFile(FILE* F) { return new_TextModeProperties(F); }
TextModeProperties* new_TextModeProperties_fromStream(std::istream& F) { return new_TextModeProperties(F); }
TextModeProperties* new_TextModeProperties_blank() { return new MapTextModeProperties(); }
void delete_TextModeProperties(TextModeProperties* ptr) { delete ptr; }
void print_TextModeProperties_toFile(FILE* F, const TextModeProperties* props) { print_TextModeProperties(F, props); }
void print_TextModeProperties_toStream(ostream& F, const TextModeProperties* props) { print_TextModeProperties(F, props); }


template<typename STREAM>
void export_lweParams(STREAM& F, const LweParams* lweparams) {
    TextModeProperties* props = new_TextModeProperties_blank();
    props->setTypeTitle("LWEPARAMS");
    props->setProperty("n", to_string(lweparams->n));
    props->setProperty("alpha_min", to_string(lweparams->alpha_min));
    props->setProperty("alpha_max", to_string(lweparams->alpha_max));
    print_TextModeProperties(F, props);
    delete_TextModeProperties(props);
}

template<typename STREAM>
const LweParams* read_new_lweParams(STREAM& F) {
    TextModeProperties* props = new_TextModeProperties(F);
    if (props->getTypeTitle() != string("LWEPARAMS")) abort();
    int n = stoi(props->getProperty("n"));
    double alpha_min = stoi(props->getProperty("alpha_min"));
    double alpha_max = stoi(props->getProperty("alpha_max"));
    delete_TextModeProperties(props);
    return new_LweParams(n,alpha_min,alpha_max);
}


/**
 * This function prints the lwe parameters to a file
 */
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams) { export_lweParams(F,lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT void export_lweParams_toStream(ostream& F, const LweParams* lweparams) { export_lweParams(F,lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT const LweParams* new_lweParams_fromStream(std::istream& in) { return read_new_lweParams(in); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT const LweParams* new_lweParams_fromFile(FILE* F)  { return read_new_lweParams(F); }

