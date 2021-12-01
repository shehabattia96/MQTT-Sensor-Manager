# Unit Interfaces

A Unit is an implementation of a piece of software, hardware or object. 
There is no constraint for what a unit could be.
However, a Unit must follow some rules:

- A Unit is self-contained and can only be accessed via events.

- An event contains the following data:

```
struct Event {
    int type;
    uint32_t epoch;
    UnitPtr parentUnit;
    void* data;
};
```

- Unit Interfaces define what a Unit is capable of.

- Every Unit Interface SHOULD `public` inherit [UnitInterface.h](./UnitInterface.h) and call its constructor.

> Note: Public inheritance is important because Units should all have the same access to Unit's methods.

The idea is to create models for every unit, to ease implementing inversion of control (IoC), such as dependency injection and event handling.
