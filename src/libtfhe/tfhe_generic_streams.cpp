#include <iostream>
#include <tfhe_io.h>
#include <map>
#include <string>
#include <cstdint>
#include <cinttypes>
#include <tfhe_generic_streams.h>
#include <cstdlib>

using namespace std;


/**
 * implementation of a property map (backed by a map of strings)
 */
class MapTextModeProperties : public TextModeProperties {
    map<string, string> data;
    string title;
public:
    MapTextModeProperties() {}

    virtual const std::string &getTypeTitle() const {
        return title;
    }

    virtual const std::string &getProperty(const std::string &name) const {
        return data.at(name);
    }

    virtual void setTypeTitle(const std::string &title) {
        this->title = title;
    }

    virtual void setProperty(const std::string &name, const std::string &value) {
        data[name] = value;
    }

    virtual double getProperty_double(const std::string &name) const {
        return stold(getProperty(name));
    }

    virtual int64_t getProperty_int64_t(const std::string &name) const {
        return stol(getProperty(name));
    }

    virtual void setProperty_double(const std::string &name, double value) {
        char buf[64];
        sprintf(buf, "%.8lf", value);
        setProperty(name, buf);
    }

    virtual void setProperty_int64_t(const std::string &name, int64_t value) {
        char buf[64];
        sprintf(buf, "%10" PRId64, value);
        setProperty(name, buf);
    }

    const map<string, string> &getMap() const {
        return data;
    }

    virtual ~MapTextModeProperties() {}
};


void StdIstream::getLine(string &reps) const { std::getline(in, reps); }

void StdIstream::fread(void *data, size_t bytes) const { in.read((char *) data, bytes); }

bool StdIstream::feof() const { return !((bool) in); }

void CIstream::getLine(string &reps) const {
    reps.clear();
    for (int32_t c = fgetc(F); c != EOF; c = fgetc(F)) {
        if (c == '\r') continue;
        if (c == '\n') return;
        reps.push_back(c);
    }
}

void CIstream::fread(void *data, size_t bytes) const {
    size_t read = std::fread(data, 1, bytes, F);
    if (read != bytes) abort();
}

bool CIstream::feof() const { return std::feof(F); }


void StdOstream::fputs(const string &s) const { out << s; }

void StdOstream::fwrite(const void *data, size_t bytes) const { out.write((char *) data, bytes); }

void COstream::fputs(const string &s) const {
    std::fputs(s.c_str(), F);
}

void COstream::fwrite(const void *data, size_t bytes) const { std::fwrite(data, 1, bytes, F); }

CIstream to_Istream(FILE *F) { return CIstream(F); }

StdIstream to_Istream(std::istream &in) { return StdIstream(in); }

COstream to_Ostream(FILE *F) { return COstream(F); }

StdOstream to_Ostream(std::ostream &out) { return StdOstream(out); }

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
TextModeProperties *new_TextModeProperties_fromIstream(const Istream &F) {
    MapTextModeProperties *reps = new MapTextModeProperties();
    string line;
    string endDelimitor;
    bool content_started = false;
    for (F.getLine(line); !F.feof(); F.getLine(line)) {
        int32_t n = line.length();
        if (!line.compare(0, 11, "-----BEGIN ") && !line.compare(n - 5, 5, "-----")) {
            string titleType = line.substr(11, n - 16);
            reps->setTypeTitle(titleType);
            endDelimitor = string("-----END ") + titleType + string("-----");
            content_started = true;
            //cerr << "Object: " << titleType << endl;
            continue;
        }
        if (!content_started) {
            cerr << "ignoring: " << line << endl;
            continue;
        }  //ignore anything before body
        if (line == endDelimitor) {
            //cerr << "EndObject: " << reps->getTypeTitle() << endl;
            return reps;
        }
        size_t pos = line.find(": ");
        if (pos == string::npos) {
            cerr << "ignoring: " << line << endl;
            continue;
        }
        string name = line.substr(0, pos);
        string value = line.substr(pos + 2);
        //cerr << "prop: " << name << "->" << value << endl;
        reps->setProperty(name, value);
    }
    //cerr << "error reading text object" << endl;
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
void print_TextModeProperties_toOStream(const Ostream &F, const TextModeProperties *properties) {
    const MapTextModeProperties *props = (const MapTextModeProperties *) properties;
    F.fputs(string("-----BEGIN ") + props->getTypeTitle() + string("-----\n"));
    for (auto it: props->getMap()) {
        F.fputs(it.first + string(": ") + it.second + "\n");
    }
    F.fputs(string("-----END ") + props->getTypeTitle() + string("-----\n"));
}

TextModeProperties *new_TextModeProperties_blank() {
    return new MapTextModeProperties();
}

void delete_TextModeProperties(TextModeProperties *ptr) {
    delete ptr;
}

