#include "module.h"

AlphaBot::AlphaBot()
{
    PyObject *module_name, *module, *dict, *python_class;

    // Initializes the Python interpreter
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    // Load the module object
    module = PyImport_ImportModule("AlphaBot2");
    if (module == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }
    Py_DECREF(module);

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "AlphaBot2");
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }
    Py_DECREF(dict);

    // Creates an instance of the class
    if (PyCallable_Check(python_class))
    {
        object = PyObject_CallObject(python_class, nullptr);
        Py_DECREF(python_class);
    }
    else
    {
        std::cout << "Cannot instantiate the Python class" << std::endl;
        Py_DECREF(python_class);
    }
}

void AlphaBot::forward()
{
    PyObject_CallMethod(object, "forward", nullptr);
}

void AlphaBot::stop()
{
    PyObject_CallMethod(object, "stop", nullptr);
}

void AlphaBot::left()
{
    PyObject_CallMethod(object, "left", nullptr);
}

void AlphaBot::right()
{
    PyObject_CallMethod(object, "right", nullptr);
}

void AlphaBot::backward()
{
    PyObject_CallMethod(object, "backward", nullptr);
}

void AlphaBot::destroy()
{
    PyObject_CallMethod(object, "stop", nullptr);
    Py_Finalize(); // Clean up and finalize the Python interpreter
}

Remote::Remote()
{
    PyObject *module_name, *module, *dict, *python_class;

    // Initializes the Python interpreter
    setenv("PYTHONPATH", ".", 1);
    Py_Initialize();

    // Load the module object
    module = PyImport_ImportModule("Remote");
    if (module == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }
    Py_DECREF(module);

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "Remote");
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }
    Py_DECREF(dict);

    // Creates an instance of the class
    if (PyCallable_Check(python_class))
    {
        object = PyObject_CallObject(python_class, nullptr);
        Py_DECREF(python_class);
    }
    else
    {
        std::cout << "Cannot instantiate the Python class" << std::endl;
        Py_DECREF(python_class);
    }
}

std::string Remote::remoteCommand()
{
    PyObject *result = PyObject_CallMethod(object, "remoteCommand", nullptr);
    PyObject *strPy = PyObject_Str(result);
    Py_DECREF(result);
    if (strPy != nullptr)
    {
        const char *strValue = PyUnicode_AsUTF8(strPy);
        Py_DECREF(strPy);

        if (strValue != nullptr)
        {
            std::string str = strValue;
            if (str != "None")
            {
                std::cout << strValue << "" << std::endl;
                return strValue;
            }
        }
    }
    return "None";
}
