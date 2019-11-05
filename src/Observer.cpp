class Observer 
{ 
public:
    virtual ~Observer() = default;
    virtual void Update(Subject&) = 0;
};