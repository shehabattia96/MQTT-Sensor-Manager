# Unit

A Unit is an implementation of a piece of software, hardware or object. 
There is no constraint for what a unit could be.
However, a Unit must follow some rules:

- A Unit is self-contained and can only be accessed via events.

- A Unit should have an Event Handler to perform an action when it receives an event.

- An event contains the following data:

```
struct Event {
    int type;
    uint32_t epoch;
    UnitPtr parentUnit;
    void* data;
};
```