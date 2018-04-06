# QXCFG - Quick XML handling and XML-based Configuration

This library intends to facilitate and speed up the integration of an XML-based
configuration.

The configuration relies on a hierarchical structured XML document, where tags
can be chosen arbitrarily to be numeric or string properties.
Internally only the string object will be stored and casted to the expected
type (see example).

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

## Copyright
2018 DFKI GmbH Robotics Innovation Center
