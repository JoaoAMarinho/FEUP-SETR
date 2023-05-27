#include <iostream>
#include <Python.h>

class AlphaBot
{
public:
    AlphaBot();
    void forward();
    void stop();
    void left();
    void right();
    void backward();
    void destroy();

private:
    PyObject *object;
};

class Remote
{
public:
    Remote();
    std::string remoteCommand();

private:
    PyObject *object;
};