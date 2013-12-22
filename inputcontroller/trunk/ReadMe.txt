This library facilitates the translation of keyboard and gamepad events into configurable binary action triggers.

InputMap is a model object that stores a mapping from input events to action identifiers. It provides all needed facilities for managing bindings. Multiple bindings can be mapped to the same action, and multiple actions can be mapped to the same binding.

InputController receives raw input events, and uses an InputMap to translate them into action events dispatched from its EventDispatcher instance. This allows you to pay attention only to INPUT_CONTROLLER_EVENT_ACTION_* events for the purpose of updating your game simulation, without having to directly parse raw keyboard or gamepad events.

InputRecorder can be attached to an InputController to create an InputSession from actions as they occur. InputPlayback can be used to replay an InputSession via an InputController, simulating actions as if they were performed by a player. If your game simulation is deterministic, using InputPlayback will allow you to exactly recreate a complete play session. If information beyond input events is needed to set up a game session (such as random seeds, level selection, difficulty choice, etc.), it can be stored as part of the InputSession by serializing it and passing the serialized blob to InputRecorder in the replayStartupData parameter. When the InputSession is loaded again, the serialized replayStartupData will be made available and can be used to set up the session.

The data format used by InputSession is designed to be robust in the case of a crash during recording. This can be used to facilitate debugging, as a recorded game session that results in a crash can be used to reliably reproduce that crash. In order to take advantage of this, you must use InputRecorder#setFileOutput, which will continuously write event to disk as they are recorded. You should also make sure to instantiate InputRecorder before registering your own event listeners for INPUT_CONTROLLER_EVENT_ACTION_* so that InputRecorder will receive the events first.

Because of these special requirements, InputSession does not use Stem's serialization facilities, and instead uses a custom binary format:

format inputSession { // All integers little endian
	uint16 formatVersion; // INPUT_SESSION_FORMAT_VERSION
	uint32 replayStartupDataSize;
	uint8 replayStartupData[replayStartupDataSize];
	uint16 actionCount;
	cstring actionIDs[actionCount];
	struct {
		uint32 frameDelta;
		uint16 actionIndex; // Index into actionIDs[]
	} events[]; // All data past this point is assumed to be an event and parsed accordingly
};
