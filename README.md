# QXCFG - Quick XML handling and XML-based Configuration

This library intends to facilitate and speed up the integration of an XML-based
configuration.

The configuration relies on a hierarchical structured XML document, where tags
can be chosen arbitrarily to be numeric or string properties.
Internally only the string object will be stored and casted to the expected
type (see example).

## Installation

This library depends upon the libraries:
 - boost
 - libxml2
 - base/logging (Rock's logging library)

For installation you can either install all dependencies manually and run the
following commands:
```
    mkdir build
    cd build
    cmake ..
    make
```

or use the existing Rock infrastructure:

```
    sudo apt install ruby wget
    mkdir your_workspace_dir
    cd your_workspace_dir
    wget http://www.rock-robotics.org/master/autoproj_bootstrap
    ruby autoproj_bootstrap
```

The build tooling:
```
    amake tools/qxcfg
```

## Tests
To enable test you have to set the define
ROCK_TEST_ENABLE=ON when configuring this library via cmake.

You can also your autoproj for this purpose via:
```
    autoproj test enable tools/qxcfg
```

Tests are implemented based on Boost Testing and can be run with
```
./build/test/test_suite
```
    
## Example

The configuration has to be a valid XML document:
```
<my-configuration>
    <property_a>a</property_a>
    <property_b>b</property_b>
    <property_c>
        <property_d>cd</property_d>
    </property_c>
    <numeric_properties>
        <int>121</int>
        <double>1.21</double>
    </numeric_properties>
    <bool_property>
        <true>true</true>
        <false>false</false>
        <nonsense>nonsense</nonsense>
    </bool_property>
</my-configuration>
```

Loading the configuration file and reading properties.
Note, that the root element is not used to access the properties, and only
children are considered configuration properties.

```
    #include <qxcfg/Configuration.hpp>
    
    qxcfg::Configuration configuration("my-configuration.xml");

    // Per default return string value
    std::string valueOfAString = configuration.getValue("property_a", "default_value");

    // Throw upon mismatching value types
    try {
        configuration.getValueAs<bool>("bool_property/nonsense");
    } catch(const std::runtime_error& e)
    {
        // casting value failed
    }

    // Set a default value, that is set when the property is not part of the
    // configuration file
    bool optionalProperty = configuration.getValueAs<bool>("optional_property",true);

```

That's it - hopefully simple and quick enough for you.

## License

This project is licensed under the [New BSD License](https://opensource.org/licenses/BSD-3-Clause).

## Copyright
Copyright (c) 2018  Thomas M Roehr, DFKI GmbH Robotics Innovation Center


