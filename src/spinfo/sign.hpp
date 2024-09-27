#ifndef UTILS_SIGN_HPP_
#define UTILS_SIGN_HPP_

#include "../spimp/common.hpp"
#include "../spimp/format.hpp"

class SignElement;
class SignParam;

/// Represents a signature
class Sign final {
    friend class SignParser;

  public:
    /// Describes the kind of the signature
    enum class Kind {
        /// Signature is empty
        EMPTY,
        /// Signature refers to a module
        MODULE,
        /// Signature refers to a class
        CLASS,
        /// Signature refers to a method
        METHOD,
        /// Signature refers to a type param
        TYPE_PARAM
    };

  private:
    /// The signature elements
    vector<SignElement> elements;

  public:
    /**
     * Creates a signature object
     * @param text the text of the signature
     */
    Sign(string text);

    /**
     * Creates a signature object
     * @param elements the elements of the signature
     */
    explicit Sign(vector<SignElement> elements);

    ~Sign() = default;

    const vector<SignElement> &getElements() const { return elements; }

    vector<SignElement> &getElements() { return elements; }

    /**
     * @return true if the signature is empty, false otherwise
     */
    bool empty() const;

    /**
     * @return the name of the signature
     */
    string getName() const;

    /**
     * @return the kind of the signature
     */
    Kind getKind() const;

    /**
     * @return the type params of the signature if exists, otherwise returns an empty array
     */
    const vector<string> &getTypeParams() const;

    /**
     * @return the params of the signature if exists, otherwise returns an empty array
     */
    const vector<SignParam> &getParams() const;

    /**
     * @return the signature of the parent module
     */
    Sign getParentModule() const;

    /**
     * @return the signature of the parent class if exists, otherwise returns an empty sign
     */
    Sign getParentClass() const;

    /**
     * Appends a copy of this signature and another signature
     * @param sign the other signature to be appended
     * @return the appended signature
     */
    Sign operator|(const Sign &sign) const;

    /**
     * Appends a copy of this signature and a string
     * @param str the string to be appended
     * @return the appended signature
     */
    Sign operator|(const string &str) const;

    /**
     * Appends a copy of this signature and a SignElement
     * @param element the element to be appended
     * @return the appended signature
     */
    Sign operator|(const SignElement &element) const;

    /**
     * Appends this signature with another signature
     * @param sign the signature to be appended
     * @return this signature after append
     */
    Sign &operator|=(const Sign &sign);

    /**
     * Appends this signature and a string
     * @param str the string to be appended
     * @return this signature after append
     */
    Sign &operator|=(const string &str);

    /**
     * Appends this signature and a SignElement
     * @param element the element to be appended
     * @return this signature after append
     */
    Sign &operator|=(const SignElement &element);

    /**
     * @return the string representation of the sign
     */
    string toString() const;

    static const Sign EMPTY;
};

class SignParam final {
    friend class SignParser;

  public:
    enum class Kind {
        /// Paramater refers to a class
        CLASS,
        /// Parameter refers to a type param
        TYPE_PARAM,
        /// Parameter refers to a callback
        CALLBACK,
    };

  private:
    Kind kind;
    Sign name;
    vector<SignParam> params;

  public:
    SignParam(Kind kind, const Sign &name, const vector<SignParam> &params = {}) : kind(kind), name(name), params(params) {}

    Kind getKind() const { return kind; }

    const Sign &getName() const { return name; }

    const vector<SignParam> &getParams() const { return params; }

    string toString() const;
};

class SignElement final {
    friend class SignParser;

  private:
    string name;
    Sign::Kind kind;
    vector<string> typeParams;
    vector<SignParam> params;

  public:
    SignElement(string name, Sign::Kind kind, vector<string> typeParams = {}, vector<SignParam> params = {})
        : name(name), kind(kind), params(params), typeParams(typeParams) {}

    ~SignElement() = default;

    const string &getName() const { return name; }

    Sign::Kind getKind() const { return kind; }

    const vector<SignParam> &getParams() const { return params; }

    const vector<string> &getTypeParams() const { return typeParams; }

    string toString() const;
};

#endif /* UTILS_SIGN_HPP_ */
