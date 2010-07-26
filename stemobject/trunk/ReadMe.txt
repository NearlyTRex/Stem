StemObject serves as the base for Stem's object system. The object system is very informal and done almost entirely by convention. The conventions are enumerated here:

	* Class and instance methods follow the naming convention ClassName_methodName().

	* All instance methods receive, as their first parameter, compat_type(ClassName *) selfPtr. This is the instance on which the method is being called. (See note on compat_type below)

	* All instance methods have a corresponding function pointer field in the object struct. These function pointers are set to their corresponding function implementations in <Name>_init().

	* All class header should contain the following line (adjusted for class name) before any #include statements for other Stem objects: typedef struct ClassName ClassName;

	* Object struct contents are #defined as a ClassName_structContents macro in the class's header. Subclasses (which should #define their struct contents in the same way) include their superclass's structContents as the first thing in their own structContents.

	* Method overriding is done by implementing a matching Subclass_method in the subclass, and setting the function pointer in the struct to point to that method in <Name>_init().

	* Calls to superclass instance methods take the form: SuperclassName_method(self);

	* All subclass objects must call their superclass's <Name>_init() method as the first thing in the subclass's <Name>_init().

	* All subclass objects must call their superclass's <Name>_dispose() method as the last thing in the subclass's <Name>_dispose().

	* All classes must contain the following methods:

		* <Name>_create(): class method which allocates, initializes, and returns an object instance. This method should set the object struct's "allocated" field to true so that the pointer will be freed when StemObject_dispose() is called on it.

		* <Name>_init(): class method which initializes the passed object's fields, including instance method function pointers

		* <Name>_dispose(): instance method which frees any memory allocated by the object, and frees the object itself if and only if it was created by calling <Name>_create()

	* Instance variable visibility is specified with the private_ivar and protected_ivar macros:

		* Instance variables declared with the private_ivar macro should only be accessed within the implementation of the particular class that declared them.

		* Instance variables declared with the protected_ivar macro should only be accessed within the implementation of the particular class that declared them or one of its subclasses.

		* All other instance variables are considered public and may be accessed by any code anywhere.

Due to the fact that subclassing is done by creating a new struct and appending fields to it, and that a superclass's function may operate on a subclass object, it is not possible to ensure that the correct type of object is passed as a function parameter at compile time. The compat_type macro exists as a convention for declaring the type of object expected, while allowing any pointer (which must be that type or a subclass thereof at runtime) to be passed. compat_type's argument is ignored, and it always expands to void *; the argument functions only as documentation in the header file containing it.