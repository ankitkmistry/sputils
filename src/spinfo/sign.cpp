#include "sign.hpp"
#include "../spimp/exceptions.hpp"

class SignParser {
    const string text;
    int32 i = 0;

  public:
    explicit SignParser(string text) : text(text + "\033") {}

    vector<SignElement> parse() {
        if (text.empty()) {
            return {
                    SignElement{"", Sign::Kind::EMPTY}
            };
        }
        vector<SignElement> elements;
        string module = "";
        // allow the unnamed module
        if (isalpha(peek()))
            module = IDENTIFIER();
        elements.push_back(SignElement(module, Sign::Kind::MODULE));

        while (match(':')) {
            check(':');
            module = IDENTIFIER();
            elements.push_back(SignElement(module, Sign::Kind::MODULE));
        }

        while (match('.')) {
            try {
                elements.push_back(classElement());
            } catch (const exceptions::SignatureError &) {
                // Let the control go to methodElement()
                break;
            }
        }

        if (match('.')) {
            elements.push_back(methodElement());
        }

        return elements;
    }

    SignElement classElement() {
        auto start = i;
        auto name = IDENTIFIER();
        vector<string> list;
        // Check type params
        if (match('<')) {
            list = idList();
            check('>');
        }
        if (peek() == '(') {
            // Restore and return
            i = start - 1;
            throw exceptions::SignatureError(text);
        }
        return {name, Sign::Kind::CLASS, list};
    }

    SignElement methodElement() {
        auto name = IDENTIFIER();
        vector<string> list;
        vector<Sign> params;
        // Check type params
        if (match('<')) {
            list = idList();
            check('>');
        }
        // Check params
        if (match('(')) {
            params.push_back(paramElement());
            while (peek() == ',') {
                advance();
                params.push_back(paramElement());
            }
            check(')');
        }
        return {name, Sign::Kind::METHOD, list, params};
    }

    Sign paramElement() {
        return Sign(parse());
    }

    vector<string> idList() {
        vector<string> list;
        list.push_back(IDENTIFIER());
        while (peek() == ',') {
            advance();
            list.push_back(IDENTIFIER());
        }
        return list;
    }

    char peek() {
        if (i >= text.size()) throw exceptions::SignatureError(text);
        int j = i;
        for (; j < text.size() && isspace(text[j]); ++j)
            ;
        return text[j];
    }

    char advance() {
        if (i >= text.size()) throw exceptions::SignatureError(text);
        while (isspace(text[i])) i++;
        return text[i++];
    }

    void check(char c) {
        if (advance() != c) {
            throw error(format("expected '%c' at col %d", c, i - 1));
        }
    }

    bool match(char c) {
        if (peek() == c) {
            advance();
            return true;
        }
        return false;
    }

    string IDENTIFIER() {
        auto start = i;
        if (!isalpha(advance())) {
            throw error(format("expected identifier at col %d", start));
        }
        while (isalnum(peek())) advance();
        if (start == i) {
            throw error(format("expected identifier at col %d", start));
        }
        return text.substr(start, i - start);
    }

    exceptions::SignatureError error(string msg) {
        return exceptions::SignatureError(text, msg);
    }
};

Sign::Sign(string text) {
    SignParser parser{text};
    elements = parser.parse();
}

Sign::Sign(vector<SignElement> elements)
    : elements(elements) {
}

Sign::~Sign() {}

static string join(const vector<string> list, string delimiter) {
    string text = "";
    for (int i = 0; i < list.size(); ++i) {
        text += list[i];
        if (i < list.size() - 1) text += delimiter;
    }
    return text;
}

template<typename T>
static vector<T> slice(vector<T> list, int64 start, int64 end) {
    if (start < 0) start += list.size();
    if (end < 0) end += list.size();
    if (start >= list.size() || end >= list.size()) throw std::runtime_error("slice(): index out of bounds");
    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }
    vector<T> result = {};
    for (int i = start; i < end; ++i) {
        result.push_back(list[i]);
    }
    return result;
}

string SignElement::toString() const {
    string str = name;
    switch (kind) {
        case Sign::Kind::EMPTY:
        case Sign::Kind::MODULE:
            break;
        case Sign::Kind::CLASS:
            if (!typeParams.empty()) {
                str.append("<");
                str.append(join(typeParams, ", "));
                str.append(">");
            }
            break;
        case Sign::Kind::METHOD:
            if (!typeParams.empty()) {
                str.append("<");
                str.append(join(typeParams, ", "));
                str.append(">");
            }
            if (!params.empty()) {
                str.append("(");
                vector<string> paramsStrs;
                for (auto param: params) {
                    paramsStrs.push_back(param.toString());
                }
                str.append(join(paramsStrs, ", "));
                str.append(")");
            }
            break;
    }
    return str;
}

string Sign::toString() const {
    string str;
    for (int i = 0; i < elements.size(); ++i) {
        auto element = elements[i];
        if (i > 0) {
            switch (element.getKind()) {
                case Kind::EMPTY:
                    break;
                case Sign::Kind::MODULE:
                    str.append("::");
                    break;
                case Sign::Kind::CLASS:
                case Sign::Kind::METHOD:
                    str.append(".");
                    break;
            }
        }
        str.append(element.toString());
    }
    return str;
}

Sign::Kind Sign::getKind() const {
    return elements.back().getKind();
}

Sign Sign::getParentModule() const {
    if (getKind() == Kind::MODULE)
        return slice(elements, 0, -1);
    int i = 0;
    for (auto element: elements) {
        if (element.getKind() != Kind::MODULE) break;
        i++;
    }
    return Sign(slice(elements, 0, i + 1));
}


Sign Sign::getParentClass() const {
    if (getKind() != Kind::MODULE) {
        int i = elements.size() - 2;
        if (i < 0) {
            return Sign("");
        }
        if (elements[i].getKind() == Kind::CLASS) {
            return Sign(slice(elements, 0, i + 1));
        }
    }
    return Sign("");
}

bool Sign::empty() const {
    return getKind() == Kind::EMPTY;
}

bool Sign::appendModule(string name) {
    if (elements.back().getKind() == Kind::MODULE) {
        elements.push_back(SignElement{name, Kind::MODULE});
        return true;
    }
    return false;
}

bool Sign::appendClass(string name, vector<string> typeParams) {
    if (elements.back().getKind() != Kind::METHOD) {
        elements.push_back(SignElement{name, Kind::CLASS, typeParams});
        return true;
    }
    return false;
}

bool Sign::appendMethod(string name, vector<string> typeParams, vector<Sign> params) {
    if (elements.back().getKind() != Kind::METHOD) {
        elements.push_back(SignElement{name, Kind::METHOD, typeParams, params});
        return true;
    }
    return false;
}

vector<string> Sign::getTypeParams() const {
    auto last = elements.back();
    if (last.getKind() != Kind::MODULE)
        return last.getTypeParams();
    return {};
}

vector<Sign> Sign::getParams() const {
    auto last = elements.back();
    if (last.getKind() == Kind::METHOD)
        return last.getParams();
    return {};
}

string Sign::getName() const {
    return elements.back().toString();
}
