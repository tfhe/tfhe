#include <tfhe_io.h>
#include <map>
#include <string>

using namespace std;

/**
 * implementation of a property map (backed by a map of strings)
 */
class MapTextModeProperties: public TextModeProperties {
    map<string, string> data;
    string title;
    public:
        MapTextModeProperties() {}
	virtual const std::string& getTypeTitle() const {
	    return title; 
	}
	virtual const std::string& getProperty(const std::string& name) const {
	    return data.at(name); 
	}
	virtual void setTypeTitle(const std::string& title) {
	    this->title=title; 
	}
	virtual void setProperty(const std::string& name, const std::string& value) {
	    data[name]=value; 
	}
	virtual double getProperty_double(const std::string& name) const { 
	    return stold(getProperty(name)); 
	}
	virtual long getProperty_long(const std::string& name) const { 
	    return stol(getProperty(name)); 
	}
	virtual void setProperty_double(const std::string& name, double value) {
	    char buf[64]; sprintf(buf,"%.8lf",value); setProperty(name,buf); 
	}
	virtual void setProperty_long(const std::string& name, long value) {
	    char buf[64]; sprintf(buf,"%ld",value); setProperty(name,buf); 
	}
	const map<string, string>& getMap() const {
	    return data; 
	}
	virtual ~MapTextModeProperties() {}
};



class StdIstream : public Istream {
    istream& in;
    public:
    StdIstream(istream& in): in(in) {}
    virtual void getLine(string& reps) const { std::getline(in, reps); }
    virtual bool feof() const { return ! ((bool) in); }
    virtual ~StdIstream() {};
};

class CIstream : public Istream {
    FILE* F;
    public:
    CIstream(FILE* F): F(F) {}
    virtual void getLine(string& reps) const { 
	reps.clear();
	for (int c = fgetc(F); c!=EOF; c = fgetc(F)) {
	    if (c=='\r') continue;
	    if (c=='\n') return;
	    reps.push_back(c);
	}
    }
    virtual bool feof() const { return std::feof(F); }
    virtual ~CIstream() {};
};


class StdOstream : public Ostream{
    ostream& out;
    public:
    StdOstream(ostream& out): out(out) {}
    virtual void fputs(const string& s) const { out << s; }
    virtual ~StdOstream() {};
};

class COstream : public Ostream {
    FILE* F;
    public:
    COstream(FILE* F): F(F) {}
    virtual void fputs(const string& s) const { 
	std::fputs(s.c_str(), F);
    }
    virtual ~COstream() {};
};


/**
 * This function parses a file, and returns its content as a
 * new TextModeProperty instance.
 * The instance must be subsequently destroyed with delete_TextModeProperties
 *
 * The format of the serialized data looks like:
 * -----BEGIN LWEPARAMS-----
 * alpha_max: 0.30000000
 * alpha_min: 0.10000000
 * n: 500
 * -----END LWEPARAMS-----  
 */
TextModeProperties* new_TextModeProperties_fromIstream(const Istream& F) {
    MapTextModeProperties* reps = new MapTextModeProperties();
    string line;
    string endDelimitor;
    bool content_started=false;
    for (F.getLine(line); !F.feof(); F.getLine(line)) {
	int n = line.length();
	if (!line.compare(0,11,"-----BEGIN ") && !line.compare(n-5,5,"-----")) {
	    string titleType = line.substr(11, n-16);
	    reps->setTypeTitle(titleType);
	    endDelimitor=string("-----END ")+titleType+string("-----");
	    content_started=true;
	    cerr << "Object: " << titleType << endl;
	    continue;
	}
	if (!content_started) { cerr << "ignoring: " << line << endl; continue;}  //ignore anything before body 
	if (line == endDelimitor) {
	    cerr << "EndObject: " << reps->getTypeTitle() << endl;
	    return reps;
	}
	size_t pos = line.find(": ");
	if (pos==string::npos) { cerr << "ignoring: " << line << endl; continue; }
	string name = line.substr(0,pos);
	string value = line.substr(pos+2);
	cerr << "prop: " << name << "->" << value << endl;
	reps->setProperty(name, value);
    }
    cerr << "error reading text object" << endl;
    return NULL;
}

/**
 * This serializes a TextModeProperty instance.
 *
 * The format of the serialized data looks like:
 * -----BEGIN LWEPARAMS-----
 * alpha_max: 0.30000000
 * alpha_min: 0.10000000
 * n: 500
 * -----END LWEPARAMS-----  
 */
void print_TextModeProperties_toOStream(const Ostream& F, const TextModeProperties* properties) {
    const MapTextModeProperties* props = (const MapTextModeProperties*) properties;
    F.fputs(string("-----BEGIN ")+props->getTypeTitle()+string("-----\n"));
    for (auto it: props->getMap()) {
	F.fputs(it.first + string(": ") + it.second+"\n");
    }
    F.fputs(string("-----END ")+props->getTypeTitle()+string("-----\n"));
}

TextModeProperties* new_TextModeProperties_blank() { 
    return new MapTextModeProperties(); 
}

void delete_TextModeProperties(TextModeProperties* ptr) { 
    delete ptr; 
}


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
EXPORT void export_lweParams_toFile(FILE* F, const LweParams* lweparams) { export_lweParams(COstream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT void export_lweParams_toStream(ostream& F, const LweParams* lweparams) { export_lweParams(StdOstream(F),lweparams); }

/**
 * This constructor function reads and creates a LWEParams from a stream. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromStream(std::istream& in) { return read_new_lweParams(StdIstream(in)); }

/**
 * This constructor function reads and creates a LWEParams from a File. The result
 * must be deleted with delete_lweParams();
 */
EXPORT LweParams* new_lweParams_fromFile(FILE* F)  { return read_new_lweParams(CIstream(F)); }

