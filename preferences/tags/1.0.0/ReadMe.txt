Preferences provides a simple interface for persistent key-value storage in a location appropriate for the user's platform.

After creating a Preferences object, the schema of your preferences keys and values must be set up by a series of function calls. addInteger, addFloat, addBoolean, addString, and addData must be called for each key/value the Preferences object is to handle.

Once keys have been registered, you can call load() to read all of the specified values from disk; getInteger, getFloat, getBoolean, getString, and getData to access them; setInteger, setFloat, setBoolean, setString, and setData to modify them; and save() to write them back to disk. You can also use loadDefaultValue() to reset a single value to its default, or loadDefaultValues() to set all values to their defaults.

Example:

	Preferences * preferences;
	
	preferences = Preferences_create("example");
	
	preferences->addInteger(preferences, "integer", 1);
	preferences->addFloat(preferences, "float", 4.0f);
	preferences->addBoolean(preferences, "boolean", true);
	preferences->addString(preferences, "string", "Hello, world!");
	
	preferences->load(preferences);
	
	printf("integer: %d\n", preferences->getInteger(preferences, "integer"));
	printf("float: %f\n", preferences->getFloat(preferences, "float"));
	printf("boolean: %s\n", preferences->getBoolean(preferences, "boolean") ? "true" : "false");
	printf("string: %s\n", preferences->getString(preferences, "string"));
	
	preferences->setInteger(preferences, "integer", -50);
	preferences->setFloat(preferences, "float", -1.0f);
	preferences->setBoolean(preferences, "boolean", false);
	preferences->setString(preferences, "string", "foo");
	
	preferences->save(preferences);
	
	preferences->dispose(preferences);
