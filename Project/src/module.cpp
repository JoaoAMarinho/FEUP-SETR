#include "module.h"

AlphaBot::AlphaBot()
{
    PyObject *module_name, *dict, *python_class;

    // Load the module object
    module_name = PyImport_ImportModule("AlphaBot2");
    if (module_name == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module_name);
    Py_DECREF(module_name);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "AlphaBot2");
    Py_DECREF(dict);
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }

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
    Py_DECREF(object);
}

Remote::Remote()
{
    PyObject *module_name, *dict, *python_class;

    // Load the module object
    module_name = PyImport_ImportModule("Remote");
    if (module_name == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module_name);
    Py_DECREF(module_name);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "Remote");
    Py_DECREF(dict);
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }

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

void Remote::destroy()
{
    Py_DECREF(object);
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

Voice::Voice()
{
    PyObject *module_name, *dict, *python_class;

    // Load the module object
    module_name = PyImport_ImportModule("Voice");
    if (module_name == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module_name);
    Py_DECREF(module_name);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "Voice");
    Py_DECREF(dict);
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }

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

std::string Voice::voiceCommand()
{
    PyObject *result = PyObject_CallMethod(object, "voiceCommand", nullptr);
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

void Voice::destroy()
{
    Py_DECREF(object);
}

Sensor::Sensor()
{
    PyObject *module_name, *dict, *python_class;

    // Load the module object
    module_name = PyImport_ImportModule("Sensor");
    if (module_name == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to import the module.\n";
    }

    // dict is a borrowed reference.
    dict = PyModule_GetDict(module_name);
    Py_DECREF(module_name);
    if (dict == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the dictionary.\n";
    }

    // Builds the name of a callable class
    python_class = PyDict_GetItemString(dict, "Sensor");
    Py_DECREF(dict);
    if (python_class == nullptr)
    {
        PyErr_Print();
        std::cerr << "Fails to get the Python class.\n";
    }

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

std::string Sensor::sensorCommand()
{
    PyObject *result = PyObject_CallMethod(object, "sensorCommand", nullptr);
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

void Sensor::destroy()
{
    Py_DECREF(object);
}
