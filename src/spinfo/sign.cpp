#include "sign.hpp"
#include "../spimp/exceptions.hpp"
#include "../spimp/utils.hpp"

const Sign Sign::EMPTY = Sign("");

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
        if (match('<')) {
            elements.push_back(SignElement(IDENTIFIER(), Sign::Kind::TYPE_PARAM));
            check('>');
        } else {
            string module = "";
            // allow the unnamed module
            if (isalpha(peek())) {
                module = IDENTIFIER();
                elements.push_back(SignElement(module, Sign::Kind::MODULE));
                while (match(':')) {
                    check(':');
                    module = IDENTIFIER();
                    elements.push_back(SignElement(module, Sign::Kind::MODULE));
                }
            } else {
                elements.push_back(SignElement(module, Sign::Kind::MODULE));
            }
            while (match('.')) elements.push_back(classOrMethodElement());
        }
        return elements;
    }

    SignElement classOrMethodElement() {
        auto name = IDENTIFIER();
        vector<string> list;
        vector<SignParam> params;
        // Check type params
        if (match('<')) {
            list = idList();
            check('>');
        }
        // Check params
        if (match('(')) {
            params = paramsElement();
            check(')');
            return {name, Sign::Kind::METHOD, list, params};
        }
        return {name, Sign::Kind::CLASS, list};
    }

    vector<SignParam> paramsElement() {
        vector<SignParam> params;
        params.push_back(paramElement());
        while (peek() == ',') {
            advance();
            params.push_back(paramElement());
        }
        params.shrink_to_fit();
        return params;
    }

    SignElement classElement() {
        auto name = IDENTIFIER();
        vector<string> list;
        // Check type params
        if (match('<')) {
            list = idList();
            check('>');
        }
        return {name, Sign::Kind::CLASS, list};
    }

    SignElement methodElement() {
        auto name = IDENTIFIER();
        vector<string> list;
        vector<SignParam> params;
        // Check type params
        if (match('<')) {
            list = idList();
            check('>');
        }
        // Check params
        check('(');
        params = paramsElement();
        check(')');
        return {name, Sign::Kind::METHOD, list, params};
    }

    SignParam paramElement() {
        vector<SignElement> elements;
        if (match('<')) {
            elements.push_back(SignElement(IDENTIFIER(), Sign::Kind::TYPE_PARAM));
            check('>');
            return SignParam(SignParam::Kind::TYPE_PARAM, Sign(elements));
        } else {
            string module = "";
            // allow the unnamed module
            if (isalpha(peek())) {
                module = IDENTIFIER();
                elements.push_back(SignElement(module, Sign::Kind::MODULE));
                while (match(':')) {
                    check(':');
                    module = IDENTIFIER();
                    elements.push_back(SignElement(module, Sign::Kind::MODULE));
                }
            } else {
                elements.push_back(SignElement(module, Sign::Kind::MODULE));
            }
            do {
                check('.');
                elements.push_back(classElement());
            } while (peek() == '.');
            if (match('(')) {
                vector<SignParam> params;
                params = paramsElement();
                check(')');
                return SignParam(SignParam::Kind::CALLBACK, Sign(elements), params);
            }
            return SignParam(SignParam::Kind::CLASS, Sign(elements));
        }
    }

    vector<string> idList() {
        vector<string> list;
        list.push_back(IDENTIFIER());
        while (peek() == ',') {
            advance();
            list.push_back(IDENTIFIER());
        }
        list.shrink_to_fit();
        return list;
    }

    char peek() {
        if (i >= text.size()) throw errors::SignatureError(text);
        int j = i;
        for (; j < text.size() && isspace(text[j]); ++j)
            ;
        return text[j];
    }

    char advance() {
        if (i >= text.size()) throw errors::SignatureError(text);
        while (isspace(text[i])) i++;
        return text[i++];
    }

    void check(char c) {
        if (advance() != c) { throw error(format("expected '%c' at col %d", c, i - 1)); }
    }

    bool match(char c) {
        if (peek() == c) {
            advance();
            return true;
        }
        return false;
    }

    string IDENTIFIER() {
        static std::set<char> specialChars = {'$', '#', '!', '@', '%', '&', '_'};
        auto start = i;
        if (isalpha(peek()) || specialChars.contains(peek())) {
            advance();
        } else {
            throw error(format("expected identifier at col %d", start));
        }
        while (isalnum(peek()) || specialChars.contains(peek())) advance();
        return text.substr(start, i - start);
    }

    errors::SignatureError error(string msg) { return errors::SignatureError(text, msg); }
};

Sign::Sign(string text) {
    SignParser parser{text};
    elements = parser.parse();
    elements.shrink_to_fit();
}

Sign::Sign(vector<SignElement> elements) : elements(elements) {}

string SignParam::toString() const {
    switch (kind) {
        case Kind::CLASS:
            return name.toString();
        case Kind::TYPE_PARAM:
            return name.toString();
        case Kind::CALLBACK: {
            vector<string> paramStrs;
            for (auto param: params) { paramStrs.push_back(param.toString()); }
            return name.toString() + join(paramStrs, ", ");
        }
    }
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
                for (auto param: params) { paramsStrs.push_back(param.toString()); }
                str.append(join(paramsStrs, ", "));
                str.append(")");
            }
            break;
        case Sign::Kind::TYPE_PARAM:
            str.append("<");
            str.append(name);
            str.append(">");
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
                case Kind::TYPE_PARAM:
                    break;
            }
        }
        str.append(element.toString());
    }
    return str;
}

Sign::Kind Sign::getKind() const { return elements.back().getKind(); }

Sign Sign::getParentModule() const {
    if (getKind() == Kind::MODULE) return Sign(slice(elements, 0, -1));
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
        if (i < 0) { return Sign::EMPTY; }
        if (elements[i].getKind() == Kind::CLASS) { return Sign(slice(elements, 0, i + 1)); }
    }
    return Sign::EMPTY;
}

bool Sign::empty() const { return getKind() == Kind::EMPTY; }

const vector<string> &Sign::getTypeParams() const { return elements.back().getTypeParams(); }

const vector<SignParam> &Sign::getParams() const { return elements.back().getParams(); }

string Sign::getName() const { return elements.back().toString(); }

Sign Sign::operator|(const Sign &sign) const {
    SignParser parser{toString() + sign.toString()};
    return Sign(parser.parse());
}

Sign Sign::operator|(const string &str) const {
    SignParser parser{toString() + str};
    return Sign(parser.parse());
}

Sign Sign::operator|(const SignElement &element) const {
    auto newElements = elements;
    newElements.push_back(element);
    SignParser parser{Sign(newElements).toString()};
    return Sign(parser.parse());
}

Sign &Sign::operator|=(const Sign &sign) {
    SignParser parser{toString() + sign.toString()};
    elements = parser.parse();
    return *this;
}

Sign &Sign::operator|=(const string &str) {
    SignParser parser{toString() + str};
    elements = parser.parse();
    return *this;
}

Sign &Sign::operator|=(const SignElement &element) {
    elements.push_back(element);
    SignParser parser{toString()};
    elements = parser.parse();
    return *this;
}
