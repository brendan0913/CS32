
class Animal {
    // no default constructor
public:
    Animal(string name) : m_name(name) {}
    virtual ~Animal() {};
    virtual void speak() const = 0; // pure virtual
    string name() const { return m_name; };
    virtual string moveAction() const { return "walk"; };
private:
    string m_name;
};

class Cat : public Animal {
public:
    Cat(string name) : Animal(name) {}
    virtual void speak() const { cout << "Meow"; }
    virtual ~Cat() { cout << "Destroying " << name() << " the cat" << endl; }
};

class Pig : public Animal {
public:
    Pig(string name, int pounds) : Animal(name), m_pounds(pounds) {}
    virtual void speak() const
    {
        if (m_pounds < 160) { cout << "Oink"; }
        else { cout << "Grunt"; }
    }
    virtual ~Pig() { cout << "Destroying " << name() << " the pig" << endl; }
private:
    int m_pounds;
};

class Duck : public Animal {
public:
    Duck(string name) : Animal(name) {}
    virtual void speak() const { cout << "Quack"; }
    virtual string moveAction() const { return "swim"; };
    virtual ~Duck() { cout << "Destroying " << name() << " the duck" << endl; }
};